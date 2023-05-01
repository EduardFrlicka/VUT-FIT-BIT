// wordcount.c
// Řešení IJC-DU2
// Datum:    23.04.2020
// Autor:    Eduard Frlicka, FIT
// Přeloženo:    gcc (GCC) 9.3.0
// Zadanie:
//      Program počítá četnost slov ve vstupním textu,
//      slovo je cokoli oddělené "bílým znakem"

#include "htab.h"
#include "io.h"
#define MAX_WORD_LENGTH 127

int main()
{
    bool error = true;
    htab_t *m = htab_init(500);
    char word[MAX_WORD_LENGTH];
    int len = 0;
    while ((len = get_word(word, MAX_WORD_LENGTH, stdin)) != EOF)
    {
        if (len == 0)
            continue;
        if (len == MAX_WORD_LENGTH&&error)
        {
            fprintf(stderr, "Warning: Word was too long\n");
            error=false;
        }
        htab_iterator_t it = htab_lookup_add(m, word);
        if (!htab_iterator_valid(it))
            continue;
        htab_iterator_set_value(it, htab_iterator_get_value(it) + 1); // počítání výskytů slova (zvýší hodnotu pro
                                                                      // zadaný klíč/slovo pokud záznam existuje,
                                                                      // jinak vytvoří nový záznam s hodnotou 0 a
                                                                      // tu operace ++ zvýší na 1)
    }
    htab_iterator_t it = htab_begin(m);
    while (htab_iterator_valid(it))
    {
        printf("%s\t%d\n", htab_iterator_get_key(it), htab_iterator_get_value(it));
        it = htab_iterator_next(it);
    }

    htab_free(m);
}

#ifdef HASHTEST
size_t htab_hash_fun(const char *str)
{
    unsigned long hash = 5381;
    char c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
#endif