#include "server.h"

#define POLL_TIMEOT 5000

pcap_t *listener;
struct bpf_program filter;

transmission_t *head = NULL; /*first element in linked list*/

transmission_t *getTransmission(uint64_t hash) {
    for (transmission_t *ptr = head; ptr; ptr = ptr->next)
        if (ptr->hash == hash)
            return ptr;
    return NULL;
}

int writeOut(transmission_t *transmission) {
    if (!transmission->file || !transmission->offset_buff) {
        WARNING("cannot write.");
        return 0;
    }
    struct pollfd fd = {(int)(intptr_t)(transmission->file), POLL_OUT, 0};
    int prc = poll(&fd, 1, POLL_TIMEOT);
    if (prc <= 0)
        ERROR(ERR_INTERNAL, "File buffer full");

    uint16_t written = fwrite(transmission->buff, 1, transmission->offset_buff, transmission->file);
    if (written != transmission->offset_buff)
        ERROR(ERR_INTERNAL, "fwrite wrote %u bytes, was supposed to write %u bytes.", written, transmission->offset_buff);
    transmission->offset_buff = 0;
    return 0;
}

int removeTransmission(transmission_t *transmission) {
    for (transmission_t **ptr = &head; *ptr; ptr = &((*ptr)->next)) {
        if (*ptr != transmission)
            continue;
        transmission_t *tmp = *ptr;
        *ptr = tmp->next;
        fclose(tmp->file);
        free(tmp);
        return 0;
    }
    ERROR(ERR_INTERNAL, "did not find transmission to remove.");
    return 0;
}

void intHandler(int num __attribute__((unused))) {
    fflush(stdout);

    for (transmission_t *ptr = head, *next = NULL; ptr; ptr = next) {
        next = ptr->next;
        writeOut(ptr);
        if (ptr->file)
            fclose(ptr->file);
        free(ptr);
    }
    pcap_breakloop(listener);
}

int handleInitialPacket(const u_char *packet, transmission_t *transmission, tSTPhead *proto) {
    char *filename = (char *)packet;
    if (transmission) {
        WARNING("Hash collision. File \"%s\" will be ignored.", filename);
        return 0;
    }

    transmission_t *newTransmission = (transmission_t *)calloc(sizeof(transmission_t), 1);
    NULLCHECK(newTransmission);
    AES_set_decrypt_key((const unsigned char *)AES_USER_KEY, 128, &(newTransmission->KeyDec));

    newTransmission->file = fopen(filename, "wb");
    if (!newTransmission->file)
        ERROR(ERR_PERMISSION, "File \"%s\" could not be opened.", filename);
    printf("Started to recive file %s\n", filename);
    newTransmission->hash = ntohll(proto->fileHash);
    newTransmission->next = head;
    newTransmission->nextSeq++;
    head = newTransmission;
    return 0;
    // printf("%p\n", transmission);
}

int handleDataPacket(u_char *packet, transmission_t *transmission, tSTPhead *proto) {
    if (!transmission) {
        WARNING("Hash not found. Ignoring packet");
        return 0;
    }
    transmission->nextSeq++;
    // u_char *data = packet;
    uint16_t datalen = ntohs(proto->datalen);
    apply_AES_function(AES_decrypt, packet, packet, &(transmission->KeyDec), datalen);

    do {
        uint16_t cpyLen = (transmission->offset_buff + datalen > BUFSIZ) ? (BUFSIZ - transmission->offset_buff) : datalen;
        memcpy(transmission->buff + transmission->offset_buff, packet, cpyLen);
        transmission->offset_buff += cpyLen;
        if (transmission->offset_buff == BUFSIZ) /*Should never be greater than BUFSIZ*/
            writeOut(transmission);
        packet += cpyLen;
        datalen -= cpyLen;
    } while (datalen);
    return 0;
}

int handleTerminalPacket(transmission_t *transmission) {
    writeOut(transmission);
    removeTransmission(transmission);
    printf("recieved\n");
    return 0;
}

int handleICMP(const u_char *packet, uint16_t length __attribute__((unused))) {
    struct icmphdr *icmp_header = (struct icmphdr *)packet;
    packet += sizeof(struct icmphdr);

    tSTPhead *proto = (tSTPhead *)packet;
    packet += sizeof(tSTPhead);

    uint16_t id = ntohs(icmp_header->un.echo.id);
    uint16_t seq = ntohs(icmp_header->un.echo.sequence);
    // printf("id: %X, seq: %u\n", id, seq);

    if (id != ECHO_IDENTIFICATOR) {
        DEBUG("Skipping packet, wrong identificator");
        return 0;
    }

    transmission_t *recvTransmission = getTransmission(ntohll(proto->fileHash));

    if (recvTransmission && recvTransmission->nextSeq != seq) {
        if (recvTransmission->nextSeq > seq) {
            WARNING("Wrong seq number, exp: %u, got: %u, skipping packet.", recvTransmission->nextSeq, seq);
            return 0;
        } else {
            WARNING("Wrong seq number, exp: %u, got: %u, data missing.", recvTransmission->nextSeq, seq);
            recvTransmission->nextSeq = seq;
        }
    }

    switch (icmp_header->code) {
    case CODE_INITIAL:
        return handleInitialPacket(packet, recvTransmission, proto);
    case CODE_DATA:
        return handleDataPacket((u_char *)packet, recvTransmission, proto);
        break;
    case CODE_TERMINAL:
        return handleTerminalPacket(recvTransmission);
        break;

    default:
        WARNING("Unknown code, skipping packet");
        break;
    }
    return 0;
}

int handleAny(const u_char *packet) {
    struct sll_header *header = (struct sll_header *)packet;

    uint16_t protocol = ntohs(header->sll_protocol);
    uint16_t len;
    const u_char *stripped;

    packet += sizeof(struct sll_header);

    switch (protocol) {
    case ETHERTYPE_IP: {
        struct ip *ip_head = (struct ip *)packet;
        uint8_t header_len = sizeof(struct ip);
        len = ntohs(ip_head->ip_len) - header_len;
        stripped = packet + header_len;
        if (checksum((uint16_t *)stripped, len)) {
            WARNING("Wrong checksum, skipping packet");
            return 0;
        }
        break;
    }
    case ETHERTYPE_IPV6: {
        struct ip6_hdr *ipv6_head = (struct ip6_hdr *)packet;
        len = ntohs(ipv6_head->ip6_ctlun.ip6_un1.ip6_un1_plen);
        uint8_t header_len = sizeof(struct ip6_hdr);
        stripped = packet + header_len;

        ipv6_head->ip6_ctlun.ip6_un1.ip6_un1_hlim = ipv6_head->ip6_ctlun.ip6_un1.ip6_un1_nxt;
        ipv6_head->ip6_ctlun.ip6_un1.ip6_un1_nxt = 0;
        ipv6_head->ip6_ctlun.ip6_un1.ip6_un1_flow = 0;

        if (checksum((uint16_t *)packet, len + header_len)) {
            WARNING("Wrong checksum, skipping packet");
            return 0;
        }
        break;
    }

    default:
        WARNING("Unknown protocol, skipping packet");
        return 0;
    }
    return handleICMP(stripped, len);
}

void mypcap_handler(u_char *args, const struct pcap_pkthdr *header __attribute__((unused)), const u_char *packet) {
    if (args) {
        WARNING("Not programmed");
        return;
    }

    handleAny(packet);
    // printPacket(packet, header->caplen);
}

int serverListen() {

    char errbuf[PCAP_ERRBUF_SIZE]; // constant defined in pcap.h
    bpf_u_int32 mask, netaddr;

    signal(SIGINT, intHandler);

    // listenSock = socket(AF_PACKET, SOCK_RAW, 0);

    // if (listenSock < 0)
    //     ERROR(ERR_INTERNAL, "Failed to open socket");

    char *device = NULL;

    pcap_lookupnet(device, &netaddr, &mask, errbuf); // -1

    listener = pcap_open_live(device, BUFSIZ, 1, 1000, errbuf); // NULL

    if (pcap_compile(listener, &filter, "icmp[icmptype]==icmp-echo or icmp6[icmptype]==icmp6-echo", 0, netaddr) == -1)
        ERROR(ERR_INTERNAL, "compile failed"); //-1

    pcap_setfilter(listener, &filter); // -1

    pcap_loop(listener, -1, mypcap_handler, (u_char *)device); //-1

    pcap_close(listener);
    pcap_freecode(&filter);

    return 0;
}
