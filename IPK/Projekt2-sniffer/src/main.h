#include <iostream>
#include <pcap/pcap.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h> // for exit()
#include <string>

#include <arpa/inet.h> // for inet_ntoa()
#include <net/ethernet.h>
#include <netinet/ip.h>      //Provides declarations for ip header
#include <netinet/ip_icmp.h> //Provides declarations for icmp header
#include <netinet/tcp.h>     //Provides declarations for tcp header
#include <netinet/udp.h>     //Provides declarations for udp header
#include <sys/socket.h>

#include <netinet/ip6.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#define HELP "For more help use %s -h or %s --help\n", argv[0], argv[0]

#define TCP_PROTOCOL_N 6
#define UDP_PROTOCOL_N 17

void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer);
void printInterfaces();
void usage();

enum protocol {
    tcp = 1,
    udp = 2,
    icmp = 4,
    arp = 8,
};

pcap_t *device;