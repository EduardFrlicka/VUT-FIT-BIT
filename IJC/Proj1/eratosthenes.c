   // eratosthenes.c
   // Řešení IJC-DU1, příklad a), 19.3.2020
   // Autor: EduardFrlicka, FIT
   // subor obsahuje funkciu pre vypocet prvocisel od 0 po N
   // pouziva funkcie / makra z modulu bitset
   // prelozene pomocou gcc (GCC) 9.2.0
#include "eratosthenes.h"

void Eratosthenes(bitset_index_t max, bitset_t* cisla){
    bitset_setbit(cisla,0,1);
    bitset_setbit(cisla,1,1);
    for(bitset_index_t i = 2; i <= (int)sqrt(max);i++){
        for(bitset_index_t j = i * 2; j<max; j+=i){
                if(bitset_getbit(cisla,j)==0){
                    bitset_setbit(cisla,j,1);
                }
        }
    }
    return;
}