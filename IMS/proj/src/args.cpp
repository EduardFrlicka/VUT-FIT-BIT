#include "args.h"
#include <unistd.h>

#define BUFFER_SIZE 1024

int load_conf(char *file_name, args_t *args);
bool is_blank(char *str);
uint convert(float num);
int load_float(char *line, uint *arg);
int load_int(char *line, int *arg);
bool is_keyword(char *line, const char *keyword);
bool is_blank(char *str);
int parse_console_args(char **argv, args_t *args);

char *program_name;

void print_usage() {
    printf("Usage: %s [OPTION]...\n", program_name);
    printf("  -h, --help                  Print this message and exit.\n");
    printf("  -f FILE, --config-file FILE Loads simulation config from FILE\n");
    printf("  -s FILE, --save-file FILE   Saves data about each iteration in csv FILE (defaults to simulation.csv)\n");
    printf("  --header HEADER             Sets header for csv file (defaults to: \"iteration, wolfs, sheep, cougar, empty\" )\n");
    printf("                              Sets path FILE as config file for configuration of simulation.\n");
    printf("  -c, --colors                Enables usage of ascii colors. Works only in terminal\n");
    printf("  -p,                         Enables printing matrix each iteration. Works only in terminal\n");
}

uint convert(float num) {
    return (uint)(num * EXPONENT);
}

int load_args(char **argv, args_t *args) {
    args->initial_wolf_count = 20;
    args->mutation_rate_wolf = 5;
    args->fitness_treshold_wolf = 7;
    args->reproduction_age_wolf = 4;
    args->reproduction_rate_wolf = 3;
    args->hold_wolf = 3;

    args->initial_cougar_count = 20;
    args->mutation_rate_cougar = 5;
    args->fitness_treshold_cougar = 7;
    args->reproduction_age_cougar = 4;
    args->reproduction_rate_cougar = 3;
    args->hold_cougar = 3;

    args->initial_sheep_count = 500;
    args->mutation_rate_sheep = 0;
    args->fitness_treshold_sheep = 3;
    args->reproduction_age_sheep = 2;
    args->reproduction_rate_sheep = 1;

    args->x = 30;
    args->y = 30;

    args->iterations = 5000;
    args->seed = time(nullptr);
    args->sleep = 100;

    args->config_file = NULL;
    args->save_file = NULL;
    args->header = NULL;
    args->colors = false;
    args->print = false;

    args->tty = isatty(fileno(stdout));

    program_name = argv[0];

    parse_console_args(argv + 1, args);

    if (args->config_file)
        load_conf(args->config_file, args);
    return 0;
}

int parse_console_args(char **argv, args_t *args) {
    while (*argv) {
        if (!strcmp(*argv, "-f") || !strcmp(*argv, "--config-file")) {
            ++argv;
            if (!*argv)
                ERROR(1, "parameter \"%s\" expected value.", *(argv - 1));

            args->config_file = *argv;
            ++argv;
            continue;
        }

        if (!strcmp(*argv, "-c") || !strcmp(*argv, "--colors")) {
            if (args->tty)
                args->colors = true;
            ++argv;
            continue;
        }

        if (!strcmp(*argv, "-h") || !strcmp(*argv, "--help")) {
            print_usage();
            exit(0);
            continue;
        }

        if (!strcmp(*argv, "-s") || !strcmp(*argv, "--save-file")) {
            ++argv;
            if (!*argv)
                ERROR(1, "parameter \"%s\" expected value.", *(argv - 1));

            args->save_file = *argv;
            ++argv;
            continue;
        }

        if (!strcmp(*argv, "--header")) {
            ++argv;
            if (!*argv)
                ERROR(1, "parameter \"%s\" expected value.", *(argv - 1));

            args->header = *argv;
            ++argv;
            continue;
        }

        if (!strcmp(*argv, "-p")) {
            ++argv;
            args->print = true;
            continue;
        }

        __ERROR_PRINT("Parameter \"%s\" is unknown.", *argv);
        print_usage();
        exit(1);
    }
    return 0;
}

int load_conf(char *file_name, args_t *args) {

    FILE *file = fopen(file_name, "r");
    if (!file)
        return 1;

    char buffer[BUFFER_SIZE];

    while (!feof(file)) {
        fgets(buffer, BUFFER_SIZE, file);

        if (is_blank(buffer))
            continue;

        /*
            SIMULATION
        */

        /* wolf */
        if (is_keyword(buffer, "initial_wolf_count"))
            load_int(buffer, &(args->initial_wolf_count));

        if (is_keyword(buffer, "mutation_rate_wolf"))
            load_int(buffer, &(args->mutation_rate_wolf));

        if (is_keyword(buffer, "fitness_treshold_wolf"))
            load_int(buffer, &(args->fitness_treshold_wolf));

        if (is_keyword(buffer, "reproduction_age_wolf"))
            load_int(buffer, &(args->reproduction_age_wolf));

        if (is_keyword(buffer, "reproduction_rate_wolf"))
            load_int(buffer, &(args->reproduction_rate_wolf));

        if (is_keyword(buffer, "hold_wolf"))
            load_int(buffer, &(args->hold_wolf));

        /* cougar */
        if (is_keyword(buffer, "initial_cougar_count"))
            load_int(buffer, &(args->initial_cougar_count));

        if (is_keyword(buffer, "mutation_rate_cougar"))
            load_int(buffer, &(args->mutation_rate_cougar));

        if (is_keyword(buffer, "fitness_treshold_cougar"))
            load_int(buffer, &(args->fitness_treshold_cougar));

        if (is_keyword(buffer, "reproduction_age_cougar"))
            load_int(buffer, &(args->reproduction_age_cougar));

        if (is_keyword(buffer, "reproduction_rate_cougar"))
            load_int(buffer, &(args->reproduction_rate_cougar));

        if (is_keyword(buffer, "hold_cougar"))
            load_int(buffer, &(args->hold_cougar));

        /* sheep */
        if (is_keyword(buffer, "initial_sheep_count"))
            load_int(buffer, &(args->initial_sheep_count));

        if (is_keyword(buffer, "mutation_rate_sheep"))
            load_int(buffer, &(args->mutation_rate_sheep));

        if (is_keyword(buffer, "fitness_treshold_sheep"))
            load_int(buffer, &(args->fitness_treshold_sheep));

        if (is_keyword(buffer, "reproduction_age_sheep"))
            load_int(buffer, &(args->reproduction_age_sheep));

        if (is_keyword(buffer, "reproduction_rate_sheep"))
            load_int(buffer, &(args->reproduction_rate_sheep));

        /*
            GENERAL
        */

        if (is_keyword(buffer, "x"))
            load_int(buffer, &(args->x));

        if (is_keyword(buffer, "y"))
            load_int(buffer, &(args->y));

        if (is_keyword(buffer, "iterations"))
            load_int(buffer, &(args->iterations));

        if (is_keyword(buffer, "seed"))
            load_int(buffer, &(args->seed));

        if (is_keyword(buffer, "sleep"))
            load_int(buffer, &(args->sleep));

        // print_conf(args);
    }
    return 0;
}

int load_float(char *line, uint *arg) {
    char *newline;
    char *endptr;
    float num;

    while (!isspace(*line++))
        ;

    newline = strchr(line, '\n');
    if (newline)
        *newline = 0;

    num = strtod(line, &endptr);

    if (*endptr)
        return 1;

    if (num < 0)
        return 1;

    *arg = convert(num);

    return 0;
}

int load_int(char *line, int *arg) {
    char *newline;
    char *endptr;
    int num;

    while (!isspace(*line))
        ++line;

    newline = strchr(line, '\n');
    if (newline)
        *newline = 0;

    num = strtol(line, &endptr, 10);

    if (*endptr)
        return 1;

    *arg = num;

    return 0;
}

bool is_keyword(char *line, const char *keyword) {
    size_t len = strlen(keyword);
    return strncmp(line, keyword, len) == 0;
}

bool is_blank(char *str) {
    while (isspace(*str++))
        ;
    return *--str == 0;
}

void print_conf(args_t *args) {
    printf("\n");
    printf("x: %d\n", args->x);
    printf("y: %d\n", args->y);

    printf("\n");
    printf("iterations: %d\n", args->iterations);
    printf("seed: %d\n", args->seed);
    printf("sleep: %d\n", args->sleep);

    // printf("\n");
    // printf("config file: %s\n", args->config_file);

    printf("\n");
    printf("initial_wolf_count: %d\n", args->initial_wolf_count);
    printf("mutation_rate_wolf: %d\n", args->mutation_rate_wolf);
    printf("fitness_treshold_wolf: %d\n", args->fitness_treshold_wolf);
    printf("reproduction_age_wolf: %d\n", args->reproduction_age_wolf);
    printf("reproduction_rate_wolf: %d\n", args->reproduction_rate_wolf);
    printf("hold_wolf: %d\n", args->hold_wolf);
    
    printf("initial_cougar_count: %d\n", args->initial_cougar_count);
    printf("mutation_rate_cougar: %d\n", args->mutation_rate_cougar);
    printf("fitness_treshold_cougar: %d\n", args->fitness_treshold_cougar);
    printf("reproduction_age_cougar: %d\n", args->reproduction_age_cougar);
    printf("reproduction_rate_cougar: %d\n", args->reproduction_rate_cougar);
    printf("hold_cougar: %d\n", args->hold_cougar);
    
    printf("initial_sheep_count: %d\n", args->initial_sheep_count);
    printf("mutation_rate_sheep: %d\n", args->mutation_rate_sheep);
    printf("fitness_treshold_sheep: %d\n", args->fitness_treshold_sheep);
    printf("reproduction_age_sheep: %d\n", args->reproduction_age_sheep);
    printf("reproduction_rate_sheep: %d\n", args->reproduction_rate_sheep);
}
