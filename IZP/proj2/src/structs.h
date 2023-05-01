#ifndef _STRUCTS_H
#define _STRUCTS_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint32_t size;
    uint8_t *table;
    char **labels;
} rel_t;

typedef struct set {
    struct set *next;
    uint32_t id;
    char value[];
} set_t;

typedef struct {
    int type;
    union {
        rel_t *rel;
        set_t *set;
        int value;
    } un;
} line_t;

typedef struct {
    uint32_t alllocated;
    uint32_t index;
    enum {
        load_universe,
        // #define LOAD_UNI load_universe

        load_data,
        // #define LOAD_DATA load_data

        load_coms
        // #define LOAD_UNI load_coms

    } state;
    line_t **lines;
} file_t;

#endif