#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

#define ERR 2
#define FAILURE 1
#define SUCCESS 0

#define ERR_ARGUMENT 0x8 /*bad argument from console*/
#define ERR_FORMAT 0x10   /*When wrong format of data in file occurs*/
#define ERR_ELEMENT 0x20  /*wrong format of element in file*/
#define ERR_VALUE 0x40 /*wrong value of element in file*/

#define ERR_INTERNAL 99 /*Failed allocation, file opening, or just some internal error*/

#ifdef _ASCII_COLORS

#define ASCII_RED "\033[31m"
#define ASCII_BLACK "\033[30m"
#define ASCII_GREEN "\033[32m"
#define ASCII_YELLOW "\033[33m"
#define ASCII_BLUE "\033[34m"
#define ASCII_MAGENTA "\033[35m"
#define ASCII_CYAN "\033[36m"
#define ASCII_WHITE "\033[37m"
#define ASCII_RESET "\033[0m"

#define ASCII_BOLD "\033[01m"

#else

#define ASCII_RED
#define ASCII_BLACK
#define ASCII_GREEN
#define ASCII_YELLOW
#define ASCII_BLUE
#define ASCII_MAGENTA
#define ASCII_CYAN
#define ASCII_WHITE
#define ASCII_RESET

#define ASCII_BOLD

#endif

#define ERROR(code, ...)                                                                                                                                                                               \
    do {                                                                                                                                                                                               \
        fprintf(stderr, "[" ASCII_BOLD ASCII_RED "  ERROR  " ASCII_RESET "] %s: ", __func__);                                                                                                          \
        fprintf(stderr, __VA_ARGS__);                                                                                                                                                                  \
        fprintf(stderr, "\n");                                                                                                                                                                         \
    } while (0)

#ifndef LOG_LEVEL_ERROR
#define WARNING(...)                                                                                                                                                                                   \
    do {                                                                                                                                                                                               \
        fprintf(stderr, "[" ASCII_BOLD ASCII_YELLOW " WARNING " ASCII_RESET "] %s: ", __func__);                                                                                                       \
        fprintf(stderr, __VA_ARGS__);                                                                                                                                                                  \
        fprintf(stderr, "\n");                                                                                                                                                                         \
    } while (0)

#ifndef LOG_LEVEL_WARNING
#define DEBUG(...)                                                                                                                                                                                     \
    do {                                                                                                                                                                                               \
        fprintf(stderr, "[" ASCII_BOLD "  DEBUG  " ASCII_RESET "] %s: ", __func__);                                                                                                                    \
        fprintf(stderr, __VA_ARGS__);                                                                                                                                                                  \
        fprintf(stderr, "\n");                                                                                                                                                                         \
    } while (0)
#else
#define DEBUG(...)
#endif
#else
#define WARNING(...)
#define DEBUG(...)
#endif

#define NULLCHECK(ptr)                                                                                                                                                                                 \
    do {                                                                                                                                                                                               \
        if (!(ptr))                                                                                                                                                                                    \
            return ERR_INTERNAL;                                                                                                                                                                       \
    } while (0)

#endif
