#ifndef _SET_H
#define _SET_H

#include "error.h"
#include "structs.h"
#include "help_func.h"

#include <stdint.h>
#include <stdio.h>
#include "string.h"


void setInsert(set_t **list, set_t *elem);

void setFree(set_t *list);

void setPrint(set_t *set);

int setLoad(FILE *fd, set_t **set);

set_t *setFind(set_t *set, char *value);

set_t *setGetCpyElm(set_t *src);

#endif