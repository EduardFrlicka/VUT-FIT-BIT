   // primes.c
   // Řešení IJC-DU1, příklad a), 19.3.2020
   // Autor: EduardFrlicka, FIT
   // subor po spusteni vypise poslednych 10 prvocisel z intervalu 0 - MAX
   // pouziva moduly bitset a eratosthenes
   // prelozene pomocou gcc (GCC) 9.2.0


#include "eratosthenes.h"
#include <assert.h>
#include <time.h>
#define MAX 500000000

int main(){
    clock_t start = clock();
    bitset_create(primes, MAX);
    Eratosthenes(MAX,primes);
    int j = -1;
    unsigned long cisla[10];
    for(bitset_index_t i = MAX-1; j<10 && i<MAX ; i--)
        if(bitset_getbit(primes, i)==0){
            cisla[++j]=i;
            }
    for(; j>=0;j--)
    printf("%lu\n",cisla[j]);
    fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);
    return 0;
}