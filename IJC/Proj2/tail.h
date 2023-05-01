   // tail.h
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0

#ifndef __tail__
#define __tail__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int exists(const char *fileName);
char *loadLine(FILE *filePtr);
char **getLastLines(FILE *fielPtr, int lineCount);
int isNotLine(char *line, int size, int sizeToRead);
int abs(int number);

#endif