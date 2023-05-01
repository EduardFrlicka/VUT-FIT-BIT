#include "help_func.h"

char *banned_words[] = {
    "false",     "true",     "empty",        "card",       "complement", "union",  "intersect", "minus",     "subseteq",   "subset",    "equals",
    "reflexive", "symetric", "antisymetric", "transitive", "function",   "domain", "codomain",  "injective", "surjective", "bijective", NULL,
};

int isBanned(char *word) {
    char **baned;
    for (baned = banned_words; *baned; baned++) {
        if (!strcmp(*baned, word))
            return ERR_ELEMENT;
    }
    return 0;
}

int loadWord(FILE *fd, char *word, uint32_t n) {
    int c;
    char *end;
    end = word + n;

    c = fgetc(fd);
    while (isalpha(c) && word <= end) {
        *word++ = c;
        c = fgetc(fd);
    }
    if (word > end)
        return ERR_ELEMENT;
    *word = 0;

    return c == ' ' ? 0 : c;
}

int loadNum(FILE *fd, int *num, const uint32_t n) {
    int c;
    char _word[n];
    char *word = _word;
    char *end = word + n;

    c = fgetc(fd);
    while (isdigit(c) && word < end) {
        *word++ = c;
        c = fgetc(fd);
    }
    if (word == end || word == _word)
        return ERR_ELEMENT;
    *word = 0;

    *num = strtol(_word, &end, 10);

    if (*end)
        return ERR_ELEMENT;

    return c == ' ' ? 0 : c;
}

int getAttrs(FILE *fd, int *attrs, uint16_t n_attrs, int max, uint16_t attr_len) {
    int res;
    int attr;

    for (uint16_t i = 1; i <= n_attrs; ++i) {
        res = loadNum(fd, &attr, attr_len);

        attr--; /*decrementing because lines indexing is from 0*/

        if (attr >= max)
            return ERR_VALUE;

        *attrs++ = attr;

        if (res != (i == n_attrs ? '\n' : 0))
            return res;
    }
    return 0;
}