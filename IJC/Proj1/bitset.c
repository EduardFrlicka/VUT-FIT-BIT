   // bitset.c
   // Řešení IJC-DU1, příklad a), 19.3.2020
   // Autor: EduardFrlicka, FIT
   // prelozene pomocou gcc (GCC) 9.2.0


#include "bitset.h"
#ifdef USE_INLINE
extern int bitset_getbit(bitset_t *jmeno_pole,bitset_index_t index);
extern void bitset_setbit(bitset_t *jmeno_pole, bitset_index_t index, int vyraz);
extern bitset_index_t bitset_size(bitset_t *jmeno_pole);
extern void bitset_free(bitset_t *jmeno_pole);
#endif