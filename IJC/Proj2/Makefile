CC=gcc
FLAGS=-g -std=c11 -pedantic -Wall -Wextra 

FLAGS_lib=-g -std=c11 -pedantic -Wall -Wextra -fPIC

deps_lib=htab.h htab_struct.h
objs_lib=htab_lib.o htab_clear_lib.o htab_hash_fun_lib.o htab_iterator_next_lib.o htab_end_lib.o htab_hash_fun_lib.o htab_iterator_set_value_lib.o htab_lib.o htab_erase_lib.o htab_init_lib.o htab_lookup_add_lib.o htab_begin_lib.o htab_find_lib.o htab_iterator_get_key_lib.o htab_size_lib.o htab_bucket_count_lib.o htab_free_lib.o htab_iterator_get_value_lib.o



.PHONY: all

all: tail wordcount wordcount-dynamic

io: io.c
	$(CC) -o $@ $< $(FLAGS)

tail: tail.o
	$(CC) -o $@ $< $(FLAGS)

tail.o: tail.c tail.h
	$(CC) -c -o $@ $< $(FLAGS)

wordcount.o:wordcount.c 
	$(CC) -c -o $@ $< $(FLAGS)

%_lib.o:%.c $(deps_lib)
	$(CC) -c -o $@ $< $(FLAGS_lib)

libhtab.a: $(objs_lib)
	ar rcs $@ $^

libhtab.so: $(objs_lib)
	$(CC) -shared $^ -o $@

wordcount: wordcount.o io.o libhtab.a 
	$(CC) -o $@ $^ $(FLAGS)

wordcount-dynamic: wordcount.o io.o libhtab.so
	$(CC) -o $@ $^ $(FLAGS)

clean:
	rm -f ./*.o *~ 