   // error.c
   // Řešení IJC-DU1, příklad a), 19.3.2020
   // Autor: EduardFrlicka, FIT
   // subor obsahuje definicie funkcii pre vypis chybovych hlaseni a varovani
   // prelozene pomocou gcc (GCC) 9.2.0
#include "error.h"
#include <stdlib.h>
#include <string.h>

void error_exit(const char *fmt, ...){
    va_list args;
    va_start (args, fmt);
    fprintf(stderr,"CHYBA: ");
    vfprintf(stderr, fmt ,args);
    va_end (args);
    exit(1);
}

void warning_msg(const char *fmt, ...){
    va_list args;
    va_start (args, fmt);
    fprintf(stderr,"CHYBA: ");
    vfprintf(stderr, fmt ,args);
    va_end (args);
}
