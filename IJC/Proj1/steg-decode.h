   // steg-decode.h
   // Řešení IJC-DU1, příklad a), 19.3.2020
   // Autor: EduardFrlicka, FIT
   // subor obsahuje deklaracie funkcii pre steg-decode.c
   // prelozene pomocou gcc (GCC) 9.2.0

#include "ppm.h"
#include "eratosthenes.h"

typedef struct char_array{char *head;
unsigned size;
} char_array_t;

void decode(struct ppm *PPM);
char_array_t *array_init();
void array_append(char_array_t *array, char value);
void array_free(char_array_t *array);