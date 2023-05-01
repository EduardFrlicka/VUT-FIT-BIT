CC=gcc
CFLAGS= -g -std=c11 -pedantic -Wall -Wextra



DEPS_ppm = ppm.h error.h bitset.h eratosthenes.h
OBJ_ppm = error.o ppm.o steg-decode.o eratosthenes.o

DEPS_primes = bitset.h eratosthenes.h error.h
OBJ_primes = primes.o eratosthenes.o error.o
OBJ_primes_i = primes-i.o eratosthenes-i.o bitset-i.o error-i.o

.PHONY: clean all run


all: steg-decode primes primes-i

io:

run: primes primes-i
	./primes
	./primes-i


%.o:%.c $(DEPS_ppm)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o:%.c $(DEPS_primes)
	$(CC) -c -o $@ $< $(CFLAGS) 

%-i.o:%.c $(DEPS_primes)
	$(CC) -c -o $@ $< ${<//-i} -DUSE_INLINE $(CFLAGS) 

primes: $(OBJ_primes)
	$(CC) -o $@ $^ $(CFLAGS) -lm

primes-i: $(OBJ_primes_i)
	$(CC) -o $@ $^ -DUSE_INLINE $(CFLAGS) -lm

steg-decode: $(OBJ_ppm)
	$(CC) -o $@ $^ $(CFLAGS) -lm


clean:
	rm -f ./*.o *~ core $(INCDIR)/*~ 