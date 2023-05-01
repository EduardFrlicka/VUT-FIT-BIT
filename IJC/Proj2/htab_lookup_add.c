   // htab_lookup_add.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // Zadanie:
   //   V tabulce  t  vyhledá záznam odpovídající řetězci  key  a
   //     - pokud jej nalezne, vrátí iterátor na záznam
   //     - pokud nenalezne, automaticky přidá záznam a vrátí iterátor
   //   Poznámka: Dobře promyslete chování této funkce k parametru key.
   //   Poznámka: podobně se chová C++ operátor [] pro std::unordered_map

#include "htab.h"
#include "htab_struct.h"

htab_iterator_t htab_lookup_add(htab_t *t, htab_key_t key)
{
    htab_iterator_t iterator = htab_find(t, key);
    if (htab_iterator_valid(iterator))
        return iterator;
    size_t hash = htab_hash_fun(key);
    iterator.idx = hash % t->arr_size;
    iterator.ptr = t->ptrArray[iterator.idx];

    htab_item_t *newItem = (htab_item_t *)malloc(sizeof(htab_item_t));
    if (newItem == NULL)
        return htab_end(t);

    newItem->key = (char *)malloc(strlen(key) + 1);
    if (newItem->key == NULL)
        return htab_end(t);

    strcpy((char *)newItem->key, key);
    newItem->value = 0;
    newItem->next = NULL;

    if (iterator.ptr != NULL)
    {
        while (iterator.ptr->next != NULL)
            iterator.ptr = iterator.ptr->next;
        iterator.ptr->next = newItem;
    }
    else
    {
        t->ptrArray[iterator.idx] = newItem;
    }
    iterator.ptr = newItem;
    t->size++;
    return iterator;
}