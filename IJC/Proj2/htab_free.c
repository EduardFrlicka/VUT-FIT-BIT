   // htab_free.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // Zadanie:
   //   destruktor: zrušení tabulky (volá htab_clear())

#include "htab.h"
#include "htab_struct.h"

void htab_free(htab_t *t)
{
    htab_clear(t);
    free(t->ptrArray);
    free(t);
}