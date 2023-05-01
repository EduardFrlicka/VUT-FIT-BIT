#include <stdio.h>
#include <stdlib.h>
#define _ASCII_COLORS
#include "error.h"

#define ERR_ARGUMENT 11
#define ERR_INPUT 12
#define SUCCESS 0

#define LINE_LEN 102

#define ASSERT(expr)                                                                                                                                                                                   \
    do {                                                                                                                                                                                               \
        int res = (expr);                                                                                                                                                                              \
        if (res)                                                                                                                                                                                       \
            return res;                                                                                                                                                                                \
    } while (0)

size_t stringlen(char *str);

int parse(char *str, size_t *num) {
    char *endptr;
    if (*str == '-')
        ERROR(ERR_ARGUMENT, "Expected positive number, got %s", str);
    *num = strtoul(str, &endptr, 10);
    if (*endptr)
        ERROR(ERR_ARGUMENT, "Expected number, got \"%s\"", str);
    return *endptr ? ERR_ARGUMENT : 0;
}

int strcpm(char *str1, char *str2) {
    while (*str1 && *str2 && *str1++ == *str2++)
        ;
    return *str1 || *str2;
}

size_t bitCount(__uint64_t num) {
    size_t res = 0;
    while (num) {
        res += num & 1;
        num >>= 1;
    }
    return res;
}

int rule1(char *str, size_t param) {
    __uint8_t flags = 0; // 0b00000001 - capital letter 0b00000010 - non capital letter
    for (; *str && flags < 3; str++) {
        flags |= *str >= 'A' && *str <= 'Z';
        flags |= (*str >= 'a' && *str <= 'z') << 1;
    }
    return flags == 3;
}

int rule2(char *str, size_t param) {
    param = param > 4 ? 4 : param;
    __uint8_t flags = 0; // 0b00000001 - capital letter | 0b00000010 - non capital letter | 0b00000100 - number | 0b00001000 - special character
    for (; *str; str++) {
        flags |= *str >= 'A' && *str <= 'Z';
        flags |= (*str >= 'a' && *str <= 'z') << 1;
        flags |= (*str >= '0' && *str <= '9') << 2;
        flags |= ((*str >= 32 && *str <= 47) || (*str >= 58 && *str <= 64) || (*str >= 91 && *str <= 96) || (*str >= 123)) << 3;
    }
    return ((flags & 3) == 3) && bitCount(flags) >= param;
}

int rule3(char *str, size_t param) {
    char last = 0;
    __uint8_t len = 0, flag = 0;

    for (char *tmp = str; *tmp && !flag; tmp++) {
        len = (*tmp == last) ? len + 1 : 1;
        last = *tmp;
        flag |= len >= param;
    }

    return !flag && rule2(str, param);
}

int stringncmp(char *str1, char *str2, int n) {
    // printf("%s, %s\n", str1, str2);
    char *end = str1 + n;
    while (*str2 && *str1 && str1 != end && *str1 == *str2) ++str1, ++str2;
    // printf("%c %c\n", *str1, *str2);
    // printf("%d\n", str1-end);
    return end == str1;
}

size_t stringlen(char *str) {
    size_t res = 0;
    while (*str++) res++;
    return res;
}

int rule4(char *str, size_t param) {
    if (stringlen(str) <= param)
        return rule3(str, param);

    for (char *patternStart = str; *(patternStart + param); patternStart++) {
        for (char *ptrStr = patternStart + 1; *(ptrStr + param - 1); ptrStr++) {
            if (stringncmp(patternStart, ptrStr, param))
                return 0;
        }
    }

    // printf("%s\n", str);

    return rule3(str, param);
}

typedef struct stats {
    __uint64_t nchars[2];
    size_t min;
    size_t passCnt;
    size_t charCnt;
} stats_t;

void statsCount(stats_t *stats, char *str) {
    size_t charCnt = 0;
    for (char *tmp = str; *tmp; ++tmp) {
        ++charCnt;
        stats->nchars[(*tmp) / 64] |= 1 << ((*tmp) % 64);
    }
    // printf("%d, %d, %d\n%lX%lX", *tmp, (*tmp) / 64, ((*tmp) % 64), stats->nchars[0],stats->nchars[1]);
    stats->charCnt += charCnt;
    stats->passCnt++;
    if (stats->min == 0)
        stats->min = charCnt;
    if (stats->min > charCnt)
        stats->min = charCnt ? charCnt : stats->min;
    // printf("stats: \"%s\", %d %d\n", str, charCnt, stats->min);
}

void statsPrint(stats_t *stats) {
    __uint32_t nChars = bitCount(stats->nchars[0]) + bitCount(stats->nchars[1]);
    float avg = (float)stats->charCnt / stats->passCnt;
    printf("Statistika:\n");
    printf("Ruznych znaku: %u\n", nChars);
    printf("Minimalni delka: %lu\n", stats->min);
    printf("Prumerna delka: %.1f\n", avg);
}

int main(int argc, char *argv[]) {

    if (argc < 3 || argc > 4)
        ERROR(ERR_ARGUMENT, "Chybny pocet argumentov.");

    size_t level = 0, param = 0;

    ASSERT(parse(argv[1], &level));
    ASSERT(parse(argv[2], &param));

    if (level < 1 || level > 4)
        ERROR(ERR_ARGUMENT, "Argument level out of range. allowed 1-4, got %lu", level);

    if (param < 1)
        ERROR(ERR_ARGUMENT, "Argument param out of range. allowed non zero positive numbers, got %lu", param);

    if (argc == 4 && strcpm(argv[3], "--stats"))
        ERROR(ERR_ARGUMENT, "Expected \"--stats\", got \"%s\"", argv[3]);

    stats_t stats = {.nchars = {0, 0}, .charCnt = 0, .min = 0, .passCnt = 0};

    int (*rules[4])(char *, size_t) = {rule1, rule2, rule3, rule4};
    int (*rule)(char *, size_t) = rules[level - 1];

    char line[LINE_LEN];

    while (fgets(line, LINE_LEN, stdin)) {
        char *str = line;
        while (*str != '\n' && *str) ++str;
        *str = 0;
        // printf("len: %d\n", str - line);
        if (str - line > 100)
            ERROR(ERR_INPUT, "Password too long");
        if ((*rule)(line, param))
            printf("%s\n", line);
        if (argc == 4)
            statsCount(&stats, line);
    }
    if (argc == 4)
        statsPrint(&stats);
}