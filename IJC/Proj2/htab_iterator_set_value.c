   // htab_iterator_set_value.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // Zadanie:
   //   přepisuje hodnotu, cíl musí existovat

#include "htab.h"
#include "htab_struct.h"

htab_value_t htab_iterator_set_value(htab_iterator_t it, htab_value_t val)
{
    it.ptr->value = val;
    return it.ptr->value;
}