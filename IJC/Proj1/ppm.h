   // ppm.h
   // Řešení IJC-DU1, příklad a), 19.3.2020
   // Autor: EduardFrlicka, FIT
   // subor obsahuje deklaracie funkcii pre ppm.c
   // prelozene pomocou gcc (GCC) 9.2.0
   
#include <string.h>
#include <stdio.h>
#include "error.h"
#include <stdlib.h>

     struct ppm {
        unsigned xsize;
        unsigned ysize;
        char data[];    // RGB bajty, celkem 3*xsize*ysize
     };

     struct ppm * ppm_read(const char * filename);
     void ppm_free(struct ppm *p);