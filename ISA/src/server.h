#ifndef SERVER_H
#define SERVER_H

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include "crypto.h"
#include "error.h"
#include "packet.h"

#include <inttypes.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/ip_icmp.h>
#include <pcap.h>
#include <pcap/pcap.h>
#include <pcap/sll.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

/**
 * @brief Structure representing and storing all necessary data for incoming transmisson
 *
 */
typedef struct transmission {
    uint64_t hash;             /*Hash: primary key*/
    uint16_t nextSeq;          /* expected sequence number*/
    uint16_t offset_buff;      /*number of chars already stored in buffer*/
    FILE *file;                /*Pointer to file for writing*/
    u_char buff[BUFSIZ];       /*Buffer for writing. Size of "BUFSIZ" from <stdio.h>, should be ideal size for writing*/
    AES_KEY KeyDec;            /*decryption key used by AES_decrypt function*/
    struct transmission *next; /*pointer to next transmission*/
} transmission_t;

int serverListen();
void intHandler(int);
int writeOut(transmission_t *transmission);

#endif