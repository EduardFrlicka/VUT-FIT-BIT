   // htab_iterator_get_key.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // Zadanie:
   //   vrací klíč, cíl musí existovat

#include "htab.h"
#include "htab_struct.h"

htab_key_t htab_iterator_get_key(htab_iterator_t it)
{
    return it.ptr->key;
}