   // ppm.c
   // Řešení IJC-DU1, příklad a), 19.3.2020
   // Autor: EduardFrlicka, FIT
   // subor obsahuje definicie funkcii pre pracu so subormi .ppm
   // prelozene pomocou gcc (GCC) 9.2.0

#include "ppm.h"


struct ppm * ppm_read(const char * filename){
    FILE *PPM_src = fopen(filename,"r");
    if(PPM_src==NULL) error_exit("Cannot open file");
    char buff[3];
    unsigned int x = 0 , y = 0,MAX_VALUE;

    int read = fscanf(PPM_src,"%2s%u%u%u",buff,&x,&y,&MAX_VALUE);
    printf("%2s %u %u %u pka \n",buff,x,y,MAX_VALUE);
    getc(PPM_src);
    if(x==0||y==0||read!=4)error_exit("Wrong format, bad header in ppm file");
    if(strcmp(buff,"P6"))error_exit("Wrong format, no P6");
    if(MAX_VALUE!=255)error_exit("Wrong format, max value not 255");
    struct ppm *tmp = malloc(sizeof(struct ppm) + x * y * 3 * sizeof(char));
    if(tmp==NULL)error_exit("Allocation failed\n");
    tmp->xsize = x;
    tmp->ysize = y;

    size_t result = fread(tmp->data,sizeof(char),(tmp->xsize) * (tmp->ysize) * 3,PPM_src);
    if(getc(PPM_src)!=EOF)error_exit("File not fully read, file was longer\n");
    if(result<x*y*3)error_exit("File was not fully read, read only %ul bytes\n",result);
    fclose(PPM_src);
    return tmp;

}

void ppm_free(struct ppm *p){
    free(p);
}