#ifndef PACKET_H
#define PACKET_H

#include <assert.h>
// #include <cstdint>
#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netinet/icmp6.h>

#define ECHO_IDENTIFICATOR 0xEF00
#define MAX_PACKET_SIZE 1500

typedef unsigned char u_char;

enum{
    initialPacket = 0x2,
    #define CODE_INITIAL initialPacket
    dataPacket = 0x4,
    #define CODE_DATA dataPacket
    terminalPacket = 0x8
    #define CODE_TERMINAL terminalPacket
};


/** Fictional transmission protocol header */
struct SecretTransmisionProtocolHeader {
    uint64_t fileHash; /** hash calculated from time and filename */
    uint16_t datalen;  /** number of bytes before encryption */
} __attribute__((packed));

typedef struct SecretTransmisionProtocolHeader tSTPhead;

uint64_t htonll(uint64_t x);
uint64_t ntohll(uint64_t x);
uint16_t checksum(uint16_t *data, size_t datalen);
void printPacket(const u_char *data, uint32_t caplen);
uint64_t sdbm(u_char *ptr, uint16_t len);

#endif