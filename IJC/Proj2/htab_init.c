   // htab_init.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // Zadanie:
   //   konstruktor: vytvoření a inicializace tabulky
   //       numb = počet prvků pole (.arr_size)

#include "htab.h"
#include "htab_struct.h"

htab_t *htab_init(size_t n)
{
    if (n <= 0)
        return NULL;
    htab_t *newTable = (htab_t *)malloc(sizeof(htab_t));

    if (newTable == NULL)
        return NULL;

    newTable->arr_size = n;
    newTable->size = 0;
    newTable->ptrArray = (htab_item_t **)malloc(n * sizeof(htab_item_t));
    if (newTable->ptrArray == NULL)
        return NULL;
    for (size_t i = 0; i < n; i++)
        newTable->ptrArray[i] = NULL;
    return newTable;

} // konstruktor tabulky