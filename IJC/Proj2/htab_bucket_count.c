   // htab_bucket_count.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // Zadanie:
   //   vrátí počet prvků pole (.arr_size)

#include "htab.h"
#include "htab_struct.h"

size_t htab_bucket_count(const htab_t *t)
{
    return t->arr_size;
} 