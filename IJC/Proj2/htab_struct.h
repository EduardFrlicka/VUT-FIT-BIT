   // htab_free.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // destruktor pre htab, uvolni najprv pamät vsetkych prvkov, potom aj samotnu tabulku

#include "htab.h"

#ifndef __htab_struct__
#define __htab_struct__

//  Tabulka:
struct htab
{
    size_t size;
    size_t arr_size;
    htab_item_t **ptrArray;
};          

// Iterátor do tabulky:
struct htab_item
{
    htab_key_t key;
    htab_value_t value;
    struct htab_item *next;
};
#endif