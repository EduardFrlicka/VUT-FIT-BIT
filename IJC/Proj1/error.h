   // error.h
   // Řešení IJC-DU1, příklad a), 19.3.2020
   // Autor: EduardFrlicka, FIT
   // subor obsahuje deklaracie funkcii pre error.c
   // prelozene pomocou gcc (GCC) 9.2.0

#include <stdio.h>
#include <stdarg.h> 

void warning_msg(const char *fmt, ...);
void error_exit(const char *fmt, ...);

