   // steg-decode.c
   // Řešení IJC-DU1, příklad a), 19.3.2020
   // Autor: EduardFrlicka, FIT
   // subor obsahuje funkcie pre dekodovanie tajnej spravy
   // pre citanie pouziva modul ppm
   // spravu sklada po bitoch z LSb z vybranych bajtov
   // vybrane bajty su bajty s indexom ktore je prvocislo, pocinajuc 23
   // prelozene pomocou gcc (GCC) 9.2.0

#include "steg-decode.h"

int main(int argc, char *argv[]){
    if(argc > 2) warning_msg("Too many arguments\n");
    if(argc < 2) error_exit("No argument\n");
    struct ppm *PPM;
    PPM = ppm_read(argv[1]);
    decode(PPM);
    ppm_free(PPM);
    return 0;
}

char_array_t *array_init(){
    char_array_t *struct_ptr = malloc(sizeof(char_array_t));
    char *char_ptr = malloc(1);
    if(struct_ptr==NULL||char_ptr==NULL)error_exit("Allocation failed\n");
    struct_ptr->head=char_ptr;
    struct_ptr->size=1;
    return struct_ptr;
}

void array_append(char_array_t *array, char value){
    array->head = realloc(array->head,++(array->size));
    array->head[array->size-2]=value;
    array->head[array->size-1]=0;
}

void array_free(char_array_t *array){
    free(array->head);
    free(array);
}

void decode(struct ppm *PPM){
    char_array_t *Message = array_init();
    bitset_alloc(Bytes,PPM->xsize*PPM->ysize*3);
    Eratosthenes(PPM->xsize*PPM->ysize*3,Bytes);
    unsigned char c = 0;
    int j=0;
    for(unsigned i = 23; i<PPM->xsize*PPM->ysize*3; i++){
        if(bitset_getbit(Bytes,i)==0) {
            //printf("%d %u ",i,(unsigned)(char)PPM->data[i]);
            if(PPM->data[i] & 1){ 
                c = (char)(c | ((char)(1 << (j%8)))); 
            } 
            else { 
                c = (char)(c & (~((char)(1 << (j%8))))); 
            }
            j++;
           // printf("%d \n",PPM->data[i] & 1);
            if(j==8){
                //printf("%c\n",c);
                array_append(Message,c);
                if(c==0){
                    printf("%s",Message->head); 
                    array_free(Message);  
                    bitset_free(Bytes); 
                    return;
                }
                j=0;
                c=0;
            }
        }
    }
    bitset_free(Bytes);
    array_free(Message);  
    error_exit("No zero char found in message");
    return;
}