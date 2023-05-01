#include "set.h"

void setInsert(set_t **list, set_t *elem) {
    static uint64_t id = 0;
    while (*list && strcmp((*list)->value, elem->value) < 0)
        list = &((*list)->next);
    elem->id = id++;
    elem->next = *list;
    *list = elem;
}

set_t *setGetCpyElm(set_t *src) {
    set_t *new;
    new = (set_t *)calloc(MAX_ELEMENT_SIZE + 1 + sizeof(set_t), sizeof(char));
    if (!new)
        return NULL;
    strcpy(new->value, src->value);

    return new;
}

set_t *setFind(set_t *set, char *value) {
    while (set && strcmp(set->value, value) < 0)
        set = set->next;
    if (!set)
        return set;
    return strcmp(set->value, value) ? NULL : set;
}

void setFree(set_t *list) {
    set_t *tmp;
    while (list) {
        tmp = list;
        list = list->next;
        free(tmp);
    }
}

void setPrint(set_t *set) {
    for (set_t *elem = set; elem; elem = elem->next)
        printf(" %s", elem->value);
    printf("\n");
}

int setLoad(FILE *fd, set_t **set) {
    /*expecting that first char was allready read, folowing should be space*/
    int res;
    set_t *element;

    res = fgetc(fd);
    if (res == ' ')

        do {
            element = (set_t *)calloc(MAX_ELEMENT_SIZE + 1 + sizeof(set_t), sizeof(char));
            NULLCHECK(element);
            res = loadWord(fd, element->value, MAX_ELEMENT_SIZE);
            res |= isBanned(element->value);
            // printf(" %s", element->value);
            if (setFind(*set, element->value)) {
                free(element);
                return ERR_ELEMENT;
            }

            setInsert(set, element);
        } while (!res);

    return res == '\n' ? 0 : res;
}