#pragma once

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "error.h"

#define EXPONENT 100000

typedef unsigned int uint;

typedef struct {
    int hold_wolf;
    int fitness_treshold_wolf;
    int mutation_rate_wolf;
    int reproduction_age_wolf;
    int reproduction_rate_wolf;
    int initial_wolf_count;

    int hold_cougar;
    int fitness_treshold_cougar;
    int mutation_rate_cougar;
    int reproduction_age_cougar;
    int reproduction_rate_cougar;
    int initial_cougar_count;

    int fitness_treshold_sheep;
    int mutation_rate_sheep;
    int reproduction_age_sheep;
    int reproduction_rate_sheep;
    int initial_sheep_count;

    int x;
    int y;

    int iterations;
    int seed;
    int sleep;

    char *config_file;
    char *save_file;
    char *header;

    bool colors;
    bool tty;
    bool print;
} args_t;

void print_conf(args_t *args);
int load_args(char **argv, args_t *);