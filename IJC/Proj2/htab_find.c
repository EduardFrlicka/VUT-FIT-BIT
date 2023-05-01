   // htab_find.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // Zadanie:
   //    V tabulce  t  vyhledá záznam odpovídající řetězci  key  a
   //     - pokud jej nalezne, vrátí iterátor na záznam
   //     - pokud nenalezne, vrátí iterátor htab_end(t)

#include "htab.h"
#include "htab_struct.h"

htab_iterator_t htab_find(htab_t *t, htab_key_t key)
{
    htab_iterator_t iterator;
    iterator.idx = htab_hash_fun(key) % t->arr_size;
    iterator.t = t;
    for (iterator.ptr = t->ptrArray[iterator.idx]; iterator.ptr != NULL; iterator.ptr = iterator.ptr->next)
    {
        if (!strcmp(iterator.ptr->key, key))
            return iterator;
    }
    return htab_end(t);
}