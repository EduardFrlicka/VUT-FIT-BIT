#include "packet.h"

#if __BIG_ENDIAN__
uint64_t htonll(uint64_t x) {
    return x;
}
uint64_t ntohll(uint64_t x) {
    return x;
}
#else
uint64_t htonll(uint64_t x) {
    return (((uint64_t)htonl(x & 0xFFFFFFFF)) << 32) + htonl(x >> 32);
}
uint64_t ntohll(uint64_t x) {
    return (((uint64_t)ntohl(x & 0xFFFFFFFF)) << 32) + ntohl(x >> 32);
}
#endif

uint16_t checksum(uint16_t *data, size_t datalen) {
    uint32_t sum = 0;

    while (datalen >= 2) {
        sum += *data++;
        datalen -= 2;
    }

    if (datalen)
        sum += *(uint8_t *)data;

    while (sum >> 16)
        sum = (sum >> 16) + (sum & 0xffff);

    assert(sum >> 16 == 0);
    return ~sum;
}

void printPacket(const u_char *data, uint32_t caplen) {
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

/** Hash function
 * @param ptr is pointer to bytes, for hashing
 * @param len number of bytes, for hashing
 * 
 * @ref inspired http://www.cse.yorku.ca/~oz/hash.html */
uint64_t sdbm(u_char *ptr, uint16_t len) {
    register uint64_t hash = 0;

    for (u_char *end = ptr + len; ptr < end; ++ptr)
        hash = *ptr + (hash << 6) + (hash << 16) - hash;

    return hash;
}