// io.c
// Řešení IJC-DU2
// Datum:    23.04.2020
// Autor:    Eduard Frlicka, FIT
// Přeloženo:    gcc (GCC) 9.3.0
// Zadanie:
//
//  Napište funkci
//  
//  int get_word(char *s, int max, FILE *f);
//  
//  která čte jedno slovo ze souboru f do zadaného pole znaků
//  a vrátí délku slova (z delších slov načte prvních max-1 znaků,
//  a zbytek přeskočí). Funkce vrací EOF, pokud je konec souboru.
//  Umístěte ji do zvláštního modulu "io.c" (nepatří do knihovny).
//  Poznámka: Slovo je souvislá posloupnost znaků oddělená isspace znaky.

#ifndef __io__
#define __io__

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int get_word(char *s, int max, FILE *f);

#endif