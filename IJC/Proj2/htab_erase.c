   // htab_erase.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // Zadanie:
   //   ruší zadaný záznam v tabulce (dávejte pozor na zneplatnění it touto
   //   operací - je to potenciální problém i v C++)

#include "htab.h"
#include "htab_struct.h"

void htab_erase(htab_t *t, htab_iterator_t it)
{
    if (it.ptr == NULL)
        return;
    free((void *)it.ptr->key);
    if (it.t->ptrArray[it.idx] == it.ptr)
        it.t->ptrArray[it.idx] = it.ptr->next;
    else
        for (htab_item_t *ptr = it.t->ptrArray[it.idx]; ptr != NULL; ptr = ptr->next)
            if (ptr->next == it.ptr)
            {
                ptr->next = it.ptr->next;
                break;
            }
    t->size--;
    free(it.ptr);
}