   // htab_clear.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // Zadanie:
   //   zrušení všech položek, tabulka zůstane prázdná

#include "htab.h"
#include "htab_struct.h"

void htab_clear(htab_t *t)
{
    htab_iterator_t it = htab_begin(t);
    while (htab_iterator_valid(it))
    {
        htab_erase(t,it);
        it = htab_begin(t);
    }

} 