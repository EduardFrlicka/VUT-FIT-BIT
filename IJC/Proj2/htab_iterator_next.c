   // htab_iterator_next.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // Zadanie:
   //   posun iterátoru na další záznam v tabulce (nebo na htab_end(t))

#include "htab.h"
#include "htab_struct.h"

htab_iterator_t htab_iterator_next(htab_iterator_t it)
{
    if (it.ptr->next != NULL)
    {
        it.ptr = it.ptr->next;
        return it;
    }
    else
    {
        for (size_t i = it.idx + 1; i < it.t->arr_size; i++)
            if (it.t->ptrArray[i] != NULL)
            {
                it.idx = i;
                it.ptr = it.t->ptrArray[i];
                return it;
            }
        return htab_end(it.t);
    }
}