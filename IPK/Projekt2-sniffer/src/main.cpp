#include "main.h"

void usage() {
    printf("./ipk-sniffer [-i [I] | --interface [I]] {-p ­­port} {[--tcp|-t] [--udp|-u] [--arp] [--icmp] } {-n num}\n\n"
           "kde\n\n"
           "- i eth0(právě jedno rozhraní, na kterém se bude poslouchat.Nebude - li tento parametr uveden, či bude - li uvedené jen - i bez hodnoty, vypíše se seznam aktivních rozhraní)\n"
           "- p 23 (bude filtrování paketů na daném rozhraní podle portu; nebude - li tento parametr uveden, uvažují se všechny porty; pokud je parametr uveden, může se daný port vyskytnout jak v "
           "source, tak v destination části)\n"
           "- t nebo --tcp(bude zobrazovat pouze TCP pakety)\n"
           "- u nebo --udp(bude zobrazovat pouze UDP pakety)\n"
           "--icmp(bude zobrazovat pouze ICMPv4 a ICMPv6 pakety)\n"
           "--arp(bude zobrazovat pouze ARP rámce)\n"
           "Pokud nebudou konkrétní protokoly specifikovány, uvažují se k tisknutí všechny(tj.veškerý obsah, nehledě na protokol)\n"
           "- n 10 (určuje počet paketů, které se mají zobrazit; pokud není uvedeno, uvažujte zobrazení pouze jednoho paketu)\n"
           "argumenty mohou být v libovolném poradi\n");
}

// 2021-03-19T18:42:52.362+01:00
// 2021-04-10T20:18:36.086+02:00

std::string time_RFC3339(const timeval date) {
    using namespace std;
    using namespace std::chrono;

    tm *gt;
    gt = gmtime(&(date.tv_sec));
    int tmp = gt->tm_hour;
    tm *lt;
    lt = localtime(&(date.tv_sec));
    const auto millis = date.tv_usec / 1000;
    const auto offset = ((0 == lt->tm_hour) ? 24 - tmp : lt->tm_hour - tmp);

    stringstream ss;
    ss << put_time(lt, "%FT%T") << '.' << setfill('0') << setw(3) << millis << (offset >= 0 ? "+" : "-") << setw(2) << offset << ":00";
    return ss.str();
}

void ctrl_c_handler(int) {
    pcap_breakloop(device);
    exit(0);
}

int main(int argc, char *argv[]) {
    int index = 1;
    char *interface = NULL;
    int protocol = 0;
    int number_of_packets = 0;
    char *port = NULL;

    char err[PCAP_ERRBUF_SIZE];
    struct bpf_program fp;
    bpf_u_int32 maskp;
    bpf_u_int32 netp;
    std::string filter;

    signal(SIGINT, ctrl_c_handler);

    // parsing arguments

    while (argc > index) {
        std::string arg = argv[index];
        if (arg == "-h" || arg == "--help") {
            usage();
            return 0;
        }

        if (arg == "-i" || arg == "--interface") {
            if (interface != NULL) {
                fprintf(stderr, "ERROR: [-i [I]|--interface [I]] option can be used just once\n" HELP);
                return 1;
            }
            if (argc == ++index) {
                printInterfaces();
                return 0;
            }
            interface = argv[index++];
            continue;
        }

        if (arg == "-p") {
            if (port) {
                fprintf(stderr, "ERROR: {-p port} option can be used just once\n" HELP);
                return 1;
            }

            char *endptr = NULL;
            strtol(argv[++index], &endptr, 10);
            port = argv[index];
            if (*endptr) {
                fprintf(stderr, "ERROR: {-p port} option: value is not a number\n" HELP);
                return 1;
            }
            ++index;
            continue;
        }

        if (arg == "-n") {
            if (number_of_packets) {
                fprintf(stderr, "ERROR: {-n num} option can be used just once\n" HELP);
                return 1;
            }
            char *endptr = NULL;
            number_of_packets = strtol(argv[++index], &endptr, 10);
            if (*endptr) {
                fprintf(stderr, "ERROR: {-n num} option: value is not a number\n" HELP);
                return 1;
            }
            ++index;
            continue;
        }

        if (arg == "-t" || arg == "--tcp") {
            protocol |= tcp;
            ++index;
            continue;
        };

        if (arg == "-u" || arg == "--udp") {
            protocol |= udp;
            ++index;
            continue;
        };

        if (arg == "--icmp") {
            protocol |= icmp;
            ++index;
            continue;
        };

        if (arg == "--arp") {
            protocol |= arp;
            ++index;
            continue;
        };

        fprintf(stderr, "ERROR: <%s> is unknown argument\n", argv[index]);
        fprintf(stderr, HELP);
        return 1;
    }

    if (interface == NULL) {
        printInterfaces();
        return 0;
    }

    // seting filter
    if (protocol & tcp) {
        filter += filter.length() == 0 ? "tcp" : " or tcp";
        filter += port ? (" port " + (std::string)port) : "";
    }
    if (protocol & udp) {
        filter += filter.length() == 0 ? "udp" : " or udp";
        filter += port ? (" port " + (std::string)port) : "";
    }
    if (protocol & icmp) {
        filter += filter.length() == 0 ? "icmp or icmp6" : " or icmp or icmp6";
    }
    if (protocol & arp) {
        filter += filter.length() == 0 ? "arp" : " or arp";
    }

    // seting default filter
    if (filter.length() == 0)
        filter = "tcp or udp or icmp or icmp6 or arp";

    // check if interface set by user is legit
    if (pcap_lookupnet(interface, &netp, &maskp, err)) {
        fprintf(stderr, "%s\n", err);
        return 1;
    };

    // opening device
    device = pcap_open_live(interface, BUFSIZ, 1, 20, err);
    if (device == NULL) {
        fprintf(stderr, "%s\n", err);
        return 1;
    }

    // setting filter in pcap
    if (pcap_compile(device, &fp, filter.c_str(), 0, netp) == -1) {
        fprintf(stderr, "Error calling pcap_compile\n");
        return 1;
    }
    if (pcap_setfilter(device, &fp) == -1) {
        fprintf(stderr, "Error setting filter\n");
        return 1;
    }

    // starting loop
    if (pcap_loop(device, number_of_packets, process_packet, NULL) < 0) {
        return 1;
    }

    return 0;
}

// finding and printing all avaliable devices (-i option)
void printInterfaces() {
    pcap_if_t *alldevsp, *device;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&alldevsp, errbuf)) {
        fprintf(stderr, "Error finding devices : %s", errbuf);
        exit(1);
    }

    for (device = alldevsp; device != NULL; device = device->next) {
        printf("%s\n", device->name);
    }
}

// function for printing raw data from packet
void printPacket(const u_char *data, bpf_u_int32 caplen) {
    for (unsigned int i = 0; i < caplen; i += 16) {
        printf("0x%04X:  ", i);
        int j = 0;

        // hex data
        for (; j < 16 && i + j < caplen; j++)
            printf("%02X ", data[i + j]);

        // padding
        for (; j < 16; j++)
            printf("   ");
        printf(" ");

        // ascii print
        for (j = 0; j < 16 && i + j < caplen; j++)
            printf("%c", isprint(data[i + j]) ? data[i + j] : '.');
        printf("\n");
    }
}

void print_header(struct ether_header *ethHeader) {
    switch (ntohs(ethHeader->ether_type)) {
    case ETHERTYPE_IP: {
        struct ip *header = (struct ip *)((char *)ethHeader + sizeof(ether_header));
        auto size_ip = header->ip_hl * 4;
        if (size_ip < 20) {
            printf("\n   * Invalid IP header length: %u bytes\n", size_ip);
            return;
        }

        char port_src[20] = "";
        char port_dst[20] = "";

        // setting ports

        switch (header->ip_p) {
        case IPPROTO_TCP: {
            struct tcphdr *tcp = (struct tcphdr *)((char *)header + size_ip);
            sprintf(port_src, " : %d", tcp->th_sport);
            sprintf(port_dst, " : %d", tcp->th_dport);
            break;
        }
        case IPPROTO_UDP: {
            struct udphdr *udp = (struct udphdr *)((char *)header + size_ip);
            sprintf(port_src, " : %d", ntohs(udp->source));
            sprintf(port_dst, " : %d", ntohs(udp->dest));
            break;
        }
        default:
            break;
        }

        // printing adress

        printf("%s%s >", inet_ntoa((header->ip_src)), port_src);
        printf(" %s%s", inet_ntoa((header->ip_dst)), port_dst);
        break;
    }
    case ETHERTYPE_IPV6: {
        printf(" ipv6 ");
        // struct ip6_hdr *header = (struct ip6_hdr *)((char *)ethHeader + sizeof(ether_header));
        // printf("") header->ip6_src;
        break;
    }
    default:
        printf(" default %d", ntohs(ethHeader->ether_type));
        break;
    }
}

// packet handler
void process_packet(u_char *, const struct pcap_pkthdr *header, const u_char *data) {
    // printing time
    std::cout << time_RFC3339(header->ts) << " ";

    // printing header
    struct ether_header *ethHeader = (ether_header *)data;
    print_header(ethHeader);

    std::cout << ", length " << header->len << " btyes" << std::endl << std::endl;

    // printing raw packet data
    printPacket(data, header->caplen);

    printf("\n\n");
}