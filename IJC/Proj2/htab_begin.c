   // htab_begin.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // Zadanie:
   //   vrací iterátor odkazující na první záznam

#include "htab.h"
#include "htab_struct.h"

htab_iterator_t htab_begin(const htab_t *t)
{
    htab_iterator_t iterator;
    iterator.t = t;
    size_t i = 0;
    while (i < t->arr_size && t->ptrArray[i] == NULL)
        i++;
    if (i == t->arr_size)
        return htab_end(t);
    iterator.idx = i;
    iterator.ptr = t->ptrArray[i];

    return iterator;
}