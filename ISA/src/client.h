#ifndef CLIENT_H
#define CLIENT_H

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include "crypto.h"
#include "error.h"
#include "packet.h"

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <libgen.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define DATA_READ_LIMIT 1024

int send_file(char *file_name, char *server_addr);
int send_packet(u_char *data, uint16_t datalen, int socket, struct addrinfo *serverinfo, uint16_t seq, tSTPhead *proto, uint8_t code);

#endif