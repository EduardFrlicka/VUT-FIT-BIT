#include "crypto.h"

void apply_AES_function(AESfuncPtr_t func, const unsigned char *in, unsigned char *out, const AES_KEY *key, size_t datalen) {
    if (in != out)
        memcpy(out, in, datalen);
    for (unsigned char *end = out + datalen; out < end; out += AES_BLOCK_SIZE)
        (*func)(out, out, key);
}
