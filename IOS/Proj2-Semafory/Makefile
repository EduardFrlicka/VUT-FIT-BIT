flags=-g -Wall -Wextra -pedantic -std=gnu99 -lpthread -lrt

CC=gcc

.PHONY: all clean

all: proj2

%.o:%.c proj2.h
	$(CC) $< -o $@  -c $(flags)

proj2: proj2.o 
	$(CC) $^ -o $@ $(flags)
	
clean: 
	rm -f ./*.o