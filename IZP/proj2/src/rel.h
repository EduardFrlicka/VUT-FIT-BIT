#ifndef _REL_H
#define _REL_H

#include "error.h"
#include "set.h"
#include "structs.h"

#include <stdint.h>
#include <stdio.h>

// void relSetBit(uint32_t x, uint32_t y, rel_t *set, uint8_t value);
// int relGetBit(uint32_t x, uint32_t y, rel_t *set);

int relLoad(FILE *fd, set_t *uni, rel_t **rel);
void relPrint(rel_t *rel);
void relSetBit(uint32_t x, uint32_t y, rel_t *rel, uint8_t value);
int relGetBit(uint32_t x, uint32_t y, rel_t *rel);
void relFree(rel_t *rel);

#endif