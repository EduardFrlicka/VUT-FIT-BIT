   // htab_size.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // Zadanie:
   //   vrátí počet prvků tabulky (.size)
   
#include "htab.h"
#include "htab_struct.h"

size_t htab_size(const htab_t *t)
{
    return t->size;
}