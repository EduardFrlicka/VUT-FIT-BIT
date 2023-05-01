   // htab_iterator_get_value.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // Zadanie:
   //   vrací hodnotu, cíl musí existovat

#include "htab.h"
#include "htab_struct.h"

htab_value_t htab_iterator_get_value(htab_iterator_t it)
{
    return it.ptr->value;
}