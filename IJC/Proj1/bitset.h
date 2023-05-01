   // bitset.h
   // Řešení IJC-DU1, příklad a), 19.3.2020
   // Autor: EduardFrlicka, FIT
   // subor obsahuje nadefinovane makra a inline funkcie pre pracu s bitovymi poliami.
   // funkcie a makra kontroluju medze pola.
        

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "error.h"
#include <assert.h>

typedef unsigned long bitset_index_t;
typedef unsigned long bitset_t;


#define forc(max_iters) for(bitset_index_t i = 0; i < max_iters; i++)

#define compute_index(velikost,typ) (1 + (velikost)/(CHAR_BIT*sizeof(typ)))
#define compute_index_mod(velikost,typ) ((velikost)%(CHAR_BIT*sizeof(typ)))

#define bitset_create(jmeno_pole, velikost) \
        bitset_t jmeno_pole[1 + compute_index(velikost,bitset_t)] = {(velikost)};\
        static_assert(sizeof(jmeno_pole)*CHAR_BIT>=velikost+sizeof(bitset_t)*CHAR_BIT,"len ak test")

#define bitset_alloc(jmeno_pole, velikost) \
        bitset_t *(jmeno_pole) = malloc((1 + compute_index(velikost,bitset_t))*sizeof(bitset_t)); \
        (jmeno_pole)[0]=(velikost); \
                if(jmeno_pole==NULL)error_exit("bitset_alloc: Chyba alokace paměti"); \
        for(bitset_index_t i = 1; i <= compute_index(velikost,bitset_t); i++) (jmeno_pole)[i]=0;

#ifdef USE_INLINE
inline bitset_index_t bitset_size(bitset_t *jmeno_pole){
    return *jmeno_pole;

}
inline int bitset_getbit(bitset_t *jmeno_pole,bitset_index_t index){
     return (bitset_size(jmeno_pole) > (index)) ? ((jmeno_pole[1 + index/(CHAR_BIT*sizeof(bitset_t))] & ((bitset_t) 1 << (index % (CHAR_BIT * sizeof(bitset_t))))) != 0) : (error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", (unsigned long)(index), (unsigned long)(jmeno_pole)[0]),1);
}
inline void bitset_setbit(bitset_t *jmeno_pole, bitset_index_t index, int vyraz){
    if(bitset_size(jmeno_pole) > (index)){ 
        if(vyraz){ 
            jmeno_pole[1 + index/(CHAR_BIT*sizeof(bitset_t))] = (jmeno_pole)[1 + index/(CHAR_BIT*sizeof(bitset_t))] | ((bitset_t)1 << (index % (CHAR_BIT * sizeof(bitset_t)))); 
            } 
            else { 
                (jmeno_pole)[1 + index/(CHAR_BIT*sizeof(bitset_t))] = (jmeno_pole)[1 + index/(CHAR_BIT*sizeof(bitset_t))] & (~((bitset_t)1 << (index % (CHAR_BIT * sizeof(bitset_t))))); 
            } 
        }
        else error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu", (unsigned long)(index), (unsigned long)(jmeno_pole)[0]);
}
inline void bitset_free(bitset_t *jmeno_pole){
    free(jmeno_pole);
}
#else

#define bitset_free(jmeno_pole) \
        free(jmeno_pole);

#define bitset_size(jmeno_pole) \
        (jmeno_pole)[0]

#define bitset_setbit(jmeno_pole,index,vyraz) \
        if(bitset_size(jmeno_pole) > (unsigned long)(index)){ \
            if(vyraz){ \
                (jmeno_pole)[compute_index(index,bitset_t)] = (jmeno_pole)[compute_index(index,bitset_t)] | ((unsigned long)1 << compute_index_mod(index,bitset_t)); \
            } \
            else { \
                (jmeno_pole)[compute_index(index,bitset_t)] = (jmeno_pole)[compute_index(index,bitset_t)] & (~((unsigned long)1 << compute_index_mod(index,bitset_t))); \
            } \
        } \
        else error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu", (unsigned long)(index), (unsigned long)(jmeno_pole)[0]);
#define bitset_getbit(jmeno_pole,index) \
        ((bitset_size(jmeno_pole) > (index)) ? (((jmeno_pole)[compute_index(index,bitset_t)] & ((unsigned long) 1 << compute_index_mod(index,bitset_t))) != 0) : (error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", (unsigned long)(index), (unsigned long)(jmeno_pole)[0]),1))

#endif


