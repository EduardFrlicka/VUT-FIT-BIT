#include "rel.h"

void relSetBit(uint32_t x, uint32_t y, rel_t *rel, uint8_t value) {
    uint64_t index;
    uint8_t shift;

    index = x * rel->size + y;
    shift = index % __CHAR_BIT__;
    if (value)
        rel->table[index / __CHAR_BIT__] |= 1 << shift;
    else
        rel->table[index / __CHAR_BIT__] &= ~(1 << shift);
}

int relGetBit(uint32_t x, uint32_t y, rel_t *rel) {
    uint64_t index;
    uint8_t shift;
    uint8_t value;

    index = x * rel->size + y;
    shift = index % __CHAR_BIT__;
    value = rel->table[index / __CHAR_BIT__];
    return (value >> shift) & 1;
}

rel_t *relInit(uint32_t size) {
    rel_t *rel;

    /*allocating relation structure*/
    rel = calloc(1, sizeof(rel_t));
    if (!rel)
        return NULL;
    /*allocating table inside relation structure*/
    rel->table = calloc((size * size / __CHAR_BIT__) + 1, 1);
    if (!rel->table) {
        free(rel);
        return NULL;
    }

    /*allocating labels array inside relation structure*/
    rel->labels = calloc(size, sizeof(char *));
    if (!rel->table) {
        free(rel->table);
        free(rel);
        return NULL;
    }
    rel->size = size;

    return rel;
}

int relLoad(FILE *fd, set_t *uni, rel_t **rel) {
    int res;
    char word[MAX_ELEMENT_SIZE + 1];
    set_t *elem;
    uint32_t x, y;
    uint32_t size;

    size = 0;
    for (set_t *ptr = uni; ptr; ptr = ptr->next)
        ++size;

    *rel = relInit(size);
    NULLCHECK(*rel);

    /*expecting that first char was allready read, folowing should be space or newline*/
    res = fgetc(fd);

    while (res == ' ') {
        /*relation element starts with '('*/
        res = fgetc(fd);
        if (res != '(')
            return ERR_ELEMENT;

        /*loading x value*/
        res = loadWord(fd, word, MAX_ELEMENT_SIZE);
        res |= isBanned(word);
        /*laodWord returns 0 if las character was ' '*/
        if (res)
            return ERR_ELEMENT;

        /*lookup in universum*/
        elem = setFind(uni, word);
        NULLCHECK(elem);
        x = elem->id;

        /*checking bounds*/
        if (elem->id >= (*rel)->size)
            return ERR_INTERNAL;
        (*rel)->labels[elem->id] = elem->value;

        /*loading y value*/
        res = loadWord(fd, word, MAX_ELEMENT_SIZE);
        res |= isBanned(word);

        /*relation element ends with '('*/
        if (res != ')')
            return ERR_ELEMENT;

        /*lookup in universum*/
        elem = setFind(uni, word);
        NULLCHECK(elem);
        y = elem->id;

        /*checking bounds*/
        if (elem->id >= (*rel)->size)
            return ERR_INTERNAL;
        (*rel)->labels[elem->id] = elem->value;

        if (relGetBit(x, y, *rel))
            return ERR_ELEMENT;

        relSetBit(x, y, *rel, 1);

        /*reading next char, if space, continue reading next element, else end while*/
        res = fgetc(fd);
    }

    /*last character chould be endlin, if not, line has bad format*/
    return res == '\n' ? 0 : res;
}

void relPrint(rel_t *rel) {
    uint32_t x;
    uint32_t y;
    for (x = 0; x < rel->size; ++x)
        for (y = 0; y < rel->size; ++y)
            if (relGetBit(x, y, rel))
                printf(" (%s %s)", rel->labels[x], rel->labels[y]);
    printf("\n");
}

void relFree(rel_t *rel) {
    if (!rel)
        return;
    free(rel->labels);
    free(rel->table);
    free(rel);
}