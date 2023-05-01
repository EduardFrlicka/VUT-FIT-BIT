   // htab_end.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // Zadanie:
   //   vrací iterátor označující (neexistující) první záznam za koncem

#include "htab.h"
#include "htab_struct.h"

htab_iterator_t htab_end(const htab_t *t)
{
    htab_iterator_t it;
    it.t = t;
    it.idx = t->arr_size;
    it.ptr = NULL;
    return it;

}