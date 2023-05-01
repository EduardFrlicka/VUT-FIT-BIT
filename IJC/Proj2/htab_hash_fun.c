   // htab_hash_fun.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // hash function

#include "htab.h"

size_t htab_hash_fun(const char *str) {
    uint32_t h=0;     // musí mít 32 bitů
    const unsigned char *p;             
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h;
    }
