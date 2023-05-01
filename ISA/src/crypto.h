#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/aes.h>
#include <string.h>

#define AES_BLOCK_SIZE 16
#define AES_USER_KEY "xfrlic00xfrlic00"

typedef void (*AESfuncPtr_t)(const unsigned char *, unsigned char *, const AES_KEY *);

void apply_AES_function(AESfuncPtr_t func, const unsigned char *in, unsigned char *out, const AES_KEY *key, size_t datalen);

#endif