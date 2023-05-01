CC=gcc
CFLAGS=-std=c11 -pedantic -Wall -Wextra
DEPS=queens.h
OBJS=queens.o

.PHONY: clean all run

all: queens

run: queens
	./queens 5

%.o:%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

queens: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f ./*.o *~ core $(INCDIR)/*~ 

ver:
	@echo $(MAKE_VERSION)