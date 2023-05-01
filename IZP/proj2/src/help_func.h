#ifndef _HELP_FUNC_H
#define _HELP_FUNC_H

#include "error.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifndef MAX_ELEMENT_SIZE
#define MAX_ELEMENT_SIZE 30
#endif

int loadWord(FILE *fd, char *word, uint32_t n);
int loadNum(FILE *fd, int *word, uint32_t n);

int getAttrs(FILE *fd, int *attrs, uint16_t n_attrs, int max, uint16_t attr_len);
int isBanned(char *word);

#endif