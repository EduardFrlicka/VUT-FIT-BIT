#include "client.h"

#define POLL_TIMEOT 5000

void printProgressBar(size_t seq, size_t exp, char *filename, uint16_t identifier);

int send_file(char *file_name, char *server_addr) {
    struct addrinfo hints, *serverinfo;
    memset(&hints, 0, sizeof(hints));

    int res;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_RAW;

    if ((res = getaddrinfo(server_addr, NULL, &hints, &serverinfo)) != 0)
        ERROR(ERR_INTERNAL, "%s", gai_strerror(res));

    int protocol = serverinfo->ai_family == AF_INET ? (int)IPPROTO_ICMP : (int)IPPROTO_ICMPV6;

    int sock = socket(serverinfo->ai_family, serverinfo->ai_socktype, protocol);
    if (sock == -1) {
        free(serverinfo);
        ERROR(ERR_INTERNAL, "Failed to open socket");
    }

    FILE *file = fopen(file_name, "r");
    if (!file) {
        close(sock);
        free(serverinfo);
        ERROR(ERR_INTERNAL, "File ptr null.");
    }

    fseek(file, 0, SEEK_END);
    size_t goal = ftell(file) / DATA_READ_LIMIT + 1;
    fseek(file, 0, SEEK_SET);

    time_t t = time(NULL);

    file_name = basename(file_name);
    uint16_t fileNameLen = strlen(file_name) + 1; /** Length of sending filename.*/
    /* + 1 because of 0 byte which will make things easier on server side */

    u_char *hashCalcBlock = (u_char *)calloc(fileNameLen + sizeof(time_t), 1);
    NULLCHECK(hashCalcBlock);
    memcpy(hashCalcBlock, &t, sizeof(time_t));

    /* filename will be at the and of allocated memory block */
    memcpy(hashCalcBlock + sizeof(time_t), file_name, fileNameLen);

    /* hash calculation */
    uint64_t fileHash = sdbm(hashCalcBlock, fileNameLen + sizeof(time_t));

    free(hashCalcBlock);

    tSTPhead proto;
    proto.datalen = htons(fileNameLen);
    proto.fileHash = htonll(fileHash);

    send_packet((u_char *)file_name, fileNameLen, sock, serverinfo, 0, &proto, CODE_INITIAL);

    size_t read = 0;
    u_char line[DATA_READ_LIMIT];

    AES_KEY encKey;
    res = AES_set_encrypt_key((const unsigned char *)AES_USER_KEY, 128, &encKey);
    assert(res == 0);
    AES_KEY decKey;
    res = AES_set_decrypt_key((const unsigned char *)AES_USER_KEY, 128, &decKey);
    assert(res == 0);

    size_t seq = 1;

    for (; (read = fread(line, 1, DATA_READ_LIMIT, file)); ++seq) {

        uint16_t datalen = read + (AES_BLOCK_SIZE - read) % AES_BLOCK_SIZE;
        u_char *data = (u_char *)calloc(datalen, 1);
        NULLCHECK(data);
        printProgressBar(seq, goal, file_name, fileHash);
        apply_AES_function(AES_encrypt, line, data, &encKey, read);

        proto.datalen = htons(read);
        send_packet(data, datalen, sock, serverinfo, seq, &proto, CODE_DATA);
        free(data);
    }

#ifdef PROGRESS
    printf("\n");
#endif

    proto.datalen = htons(0);
    send_packet(NULL, 0, sock, serverinfo, seq, &proto, CODE_TERMINAL);

    free(serverinfo);
    close(sock);
    fclose(file);
    return 0;
}

int send_packet(u_char *data, uint16_t datalen, int socket, struct addrinfo *serverinfo, uint16_t seq, tSTPhead *proto, uint8_t code) {
    size_t packet_len = datalen + sizeof(struct icmphdr) + sizeof(tSTPhead);
    u_char *packet = (u_char *)calloc(packet_len, 1);
    NULLCHECK(packet);

    // printf("sending: %lu, datalen: %u, randomvalue: %d\n", packet_len, datalen, (-datalen) % (-AES_BLOCK_SIZE));

    struct icmphdr *icmp_header = (struct icmphdr *)packet;
    icmp_header->code = code;
    icmp_header->type = serverinfo->ai_family == AF_INET ? ICMP_ECHO : 128;
    icmp_header->un.echo.id = htons(ECHO_IDENTIFICATOR);
    icmp_header->un.echo.sequence = htons(seq);

    // tSTPhead *STPhead = (tSTPhead *)(packet + sizeof(struct icmphdr));
    // STPhead->datalen = htons(datalen);
    // STPhead->fileHash = htonll(hash);

    memcpy(packet + sizeof(struct icmphdr), proto, sizeof(tSTPhead));

    if (data && datalen)
        memcpy(packet + sizeof(struct icmphdr) + sizeof(tSTPhead), data, datalen);

    icmp_header->checksum = checksum((uint16_t *)packet, packet_len);

    assert(packet_len < MAX_PACKET_SIZE);

    struct pollfd fd = {socket, POLLOUT, 0};
    int prc = poll(&fd, 1, POLL_TIMEOT);
    if (prc <= 0)
        ERROR(ERR_INTERNAL, "Socket timeot");

    int res = sendto(socket, packet, packet_len, 0, (struct sockaddr *)(serverinfo->ai_addr), serverinfo->ai_addrlen);

    if (res < 0)
        ERROR(ERR_INTERNAL, "sendto errno: %d, %s", errno, strerror(errno));

    free(packet);

    uint8_t exp_type = 129;
    ssize_t min_packet_len = sizeof(struct icmphdr) + sizeof(tSTPhead);
    if (serverinfo->ai_family == AF_INET) {
        min_packet_len += sizeof(struct ip);
    }

    packet = (u_char *)calloc(min_packet_len, 1);
    NULLCHECK(packet);
    icmp_header = (struct icmphdr *)packet;

    if (serverinfo->ai_family == AF_INET) {
        icmp_header = (struct icmphdr *)(packet + sizeof(struct ip));
        exp_type = ICMP_ECHOREPLY;
    }

    while (1) {

        fd = (struct pollfd){socket, POLLIN, 0};
        prc = poll(&fd, 1, POLL_TIMEOT);

        if (prc < 0)
            ERROR(ERR_INTERNAL, "poll returned negative value");
        if (prc == 0) {
            WARNING("Poll timeout, no response.");
            free(packet);
            return 0;
        }

        ssize_t recv_len = recv(socket, packet, min_packet_len, 0);

        if (recv_len < 0) {
            WARNING("recv error");
            free(packet);
            return 0;
        }

        if (recv_len < min_packet_len) {
            continue;
        }

        if (icmp_header->type != exp_type) {
            continue;
        }
        if (icmp_header->un.echo.id != htons(ECHO_IDENTIFICATOR)) {
            continue;
        }
        if (icmp_header->un.echo.sequence != htons(seq)) {
            continue;
        }

        break;
    }

    free(packet);

    return 0;
}

#ifdef PROGRESS
void printProgressBar(size_t seq, size_t exp, char *filename, uint16_t identifier) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    uint width = w.ws_col;

    char start[] = " [ ";
    char end[] = " ] ";
    char point[] = ">";

    uint leftPad = 8;
    uint startLen = sizeof(start) - 1;
    uint endLen = sizeof(end) - 1;
    uint pointLen = sizeof(point) - 1;

    char c = '#';
    char f = '-';

    char *line = calloc(width + 1, 1);
    char *ptr = line, *endPtr = line + width + 1;
    ptr += snprintf(ptr, endPtr - ptr, "Sending file: %s, identifier: %u ", filename, identifier);

    int progressLen = width - (ptr - line) - leftPad - startLen - endLen - pointLen;

    int progress = progressLen * seq / exp;

    ptr += snprintf(ptr, endPtr - ptr, "%s", start);

    if (seq >= exp) {
        memset(ptr, c, progressLen + pointLen);
        ptr += progressLen + pointLen;
    } else {
        memset(ptr, c, progress);
        ptr += progress;
        memcpy(ptr, point, pointLen);
        ptr += pointLen;
        memset(ptr, f, progressLen - progress);
        ptr += progressLen - progress;
    }

    ptr += snprintf(ptr, endPtr - ptr, "%s %5.1f%% ", end, (float)100 * seq / exp);

    printf("\033[?25l\r%s\033[?25h", line);

    free(line);
}
#else
void printProgressBar(size_t seq __attribute__((unused)), size_t exp __attribute__((unused)), char *filename __attribute__((unused)), uint16_t identifier __attribute__((unused))) {
}
#endif