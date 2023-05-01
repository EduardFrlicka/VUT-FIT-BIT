#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h> 
#include <stdbool.h>
#include <time.h>

#define sem_name(name) "xfrlic00_ios_sem-" name

#define mymap(pointer)                                                                                         \
    {                                                                                                          \
        (pointer) = mmap(NULL, sizeof(*(pointer)), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); \
    }
#define unmap(pointer)                        \
    {                                         \
        munmap((pointer), sizeof((pointer))); \
    }
#define msleep(time) \
    if ((time) != 0) \
    usleep((rand() % time) * 1000)

int parse(char *arg);
void doJudge(FILE *file, int PI, int JG, int JT);
void doGenerator(FILE *file, int PI, int IG, int IT);
void doChild(FILE *file, int internalNumber, int IT);
void writeOut(int NEc, int NCc, int NBc, bool print, FILE *file, const char *fmt, ...);

sem_t *law = NULL;
sem_t *par_sem = NULL;
sem_t *imm = NULL;
sem_t *write_sem = NULL;
sem_t *judge = NULL;
int *test = NULL;
int *A = NULL;
int *NE = NULL, *NC = NULL, *NB = NULL;