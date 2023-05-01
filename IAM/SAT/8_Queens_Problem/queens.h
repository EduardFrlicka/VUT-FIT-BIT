#ifndef _QUEENS
#define _QUEENS

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int bishopClausulesCount(int N);
void rookClausules(int place, int N);
void bishopClausules(int place, int N);
int isNumber(char* str);


#endif