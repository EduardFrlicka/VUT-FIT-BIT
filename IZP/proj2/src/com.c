#include "com.h"

int doCom(FILE *fd, file_t *file) {
    char command[COM_LEN];
    int res;

    /*expecting that first char was allready read, folowing should be space*/
    res = fgetc(fd);
    if (res != ' ')
        return ERR_FORMAT;

    res = loadWord(fd, command, COM_LEN);
    if (res>0)
        return res;

    if (!strcmp("empty", command))
        return setF_empty(fd, file);

    if (!strcmp("card", command))
        return setF_card(fd, file);

    if (!strcmp("complement", command))
        return setF_complement(fd, file);

    if (!strcmp("union", command))
        return setF_union(fd, file);

    if (!strcmp("intersect", command))
        return setF_intersect(fd, file);

    if (!strcmp("minus", command))
        return setF_minus(fd, file);

    if (!strcmp("subseteq", command))
        return setF_subseteq(fd, file);

    if (!strcmp("subset", command))
        return setF_subset(fd, file);

    if (!strcmp("equals", command))
        return setF_equals(fd, file);

    if (!strcmp("reflexive", command))
        return relF_reflexive(fd, file);

    if (!strcmp("symmetric", command))
        return relF_symetric(fd, file);

    if (!strcmp("antisymmetric", command))
        return relF_antisymetric(fd, file);

    if (!strcmp("transitive", command))
        return relF_transitive(fd, file);

    if (!strcmp("function", command))
        return relF_function(fd, file);

    if (!strcmp("domain", command))
        return relF_domain(fd, file);

    if (!strcmp("codomain", command))
        return relF_codomain(fd, file);

    if (!strcmp("injective", command))
        return relF_injective(fd, file);

    if (!strcmp("surjective", command))
        return relF_surjective(fd, file);

    if (!strcmp("bijective", command))
        return relF_bijective(fd, file);

    return ERR_FORMAT;
}