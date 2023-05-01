#include "rel_func.h"

int relFunction(rel_t *rel) {
    int value;
    for (uint32_t x = 0; x < rel->size; x++) {
        value = 1;
        for (uint32_t y = 0; y < rel->size; y++)
            if (relGetBit(x, y, rel))
                --value;
        if (value < 0)
            break;
    }
    return value < 0 ? 0 : 1;
}

int relF_reflexive(FILE *fd, file_t *file) {
    int res;
    int attr;
    line_t *line;
    rel_t *rel;
    int value;

    line = file->lines[file->index];

    res = getAttrs(fd, &attr, 1, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr]->type != LINE_REL)
        return ERR_VALUE;
    rel = file->lines[attr]->un.rel;

    value = 1;
    for (uint32_t i = 0; i < rel->size; i++)
        if (!relGetBit(i, i, rel))
            value = 0;

    line->type = LINE_BOOL;
    line->un.value = value;

    return res;
}

int relF_symetric(FILE *fd, file_t *file) {
    int res;
    int attr;
    line_t *line;
    rel_t *rel;
    int value;

    line = file->lines[file->index];

    res = getAttrs(fd, &attr, 1, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr]->type != LINE_REL)
        return ERR_VALUE;
    rel = file->lines[attr]->un.rel;

    value = 1;

    for (uint32_t x = 0; x < rel->size; x++)
        for (uint32_t y = 0; y < rel->size; y++)
            if (relGetBit(x, y, rel))
                if (!relGetBit(y, x, rel))
                    value = 0;

    line->type = LINE_BOOL;
    line->un.value = value;

    return res;
}

int relF_antisymetric(FILE *fd, file_t *file) {
    int res;
    int attr;
    line_t *line;
    rel_t *rel;
    int value;

    line = file->lines[file->index];

    res = getAttrs(fd, &attr, 1, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr]->type != LINE_REL)
        return ERR_VALUE;
    rel = file->lines[attr]->un.rel;
    value = 1;

    for (uint32_t x = 0; x < rel->size; x++)
        for (uint32_t y = 0; y <= x; y++)
            if (relGetBit(x, y, rel) && relGetBit(y, x, rel))
                if (x != y)
                    value = 0;

    line->type = LINE_BOOL;
    line->un.value = value;

    return res;
}

int relF_transitive(FILE *fd, file_t *file) {
    int res;
    int attr;
    line_t *line;
    rel_t *rel;
    int value;

    line = file->lines[file->index];

    res = getAttrs(fd, &attr, 1, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr]->type != LINE_REL)
        return ERR_VALUE;
    rel = file->lines[attr]->un.rel;
    value = 1;

    for (uint32_t x = 0; x < rel->size; x++)
        for (uint32_t y = 0; y < rel->size; y++)
            for (uint32_t z = 0; z < rel->size; z++)
                if (relGetBit(x, y, rel) && relGetBit(y, z, rel))
                    if (!relGetBit(x, z, rel))
                        value = 0;

    line->type = LINE_BOOL;
    line->un.value = value;

    return res;
}

int relF_function(FILE *fd, file_t *file) {
    int res;
    int attr;
    line_t *line;
    rel_t *rel;
    int value;

    line = file->lines[file->index];

    res = getAttrs(fd, &attr, 1, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr]->type != LINE_REL)
        return ERR_VALUE;
    rel = file->lines[attr]->un.rel;
    value = relFunction(rel);

    line->type = LINE_BOOL;
    line->un.value = value;

    return res;
}

int relF_domain(FILE *fd, file_t *file) {
    int res;
    int attr;
    line_t *line;
    rel_t *rel;
    set_t *set;
    set_t *tmp;

    line = file->lines[file->index];

    res = getAttrs(fd, &attr, 1, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr]->type != LINE_REL)
        return ERR_VALUE;
    rel = file->lines[attr]->un.rel;

    set = NULL;

    for (uint32_t x = 0; x < rel->size; x++)
        for (uint32_t y = 0; y < rel->size; y++)
            if (relGetBit(x, y, rel)) {
                tmp = setFind(file->lines[0]->un.set, rel->labels[x]);
                NULLCHECK(tmp);
                tmp = setGetCpyElm(tmp);
                NULLCHECK(tmp);
                setInsert(&set, tmp);
                break;
            }

    line->type = LINE_SET;
    line->un.set = set;

    return res;
}

int relF_codomain(FILE *fd, file_t *file) {
    int res;
    int attr;
    line_t *line;
    rel_t *rel;
    set_t *set;
    set_t *tmp;

    line = file->lines[file->index];

    res = getAttrs(fd, &attr, 1, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr]->type != LINE_REL)
        return ERR_VALUE;
    rel = file->lines[attr]->un.rel;

    set = NULL;

    for (uint32_t y = 0; y < rel->size; y++)
        for (uint32_t x = 0; x < rel->size; x++)
            if (relGetBit(x, y, rel)) {
                tmp = setFind(file->lines[0]->un.set, rel->labels[y]);
                NULLCHECK(tmp);
                tmp = setGetCpyElm(tmp);
                NULLCHECK(tmp);
                setInsert(&set, tmp);
                break;
            }

    line->type = LINE_SET;
    line->un.set = set;

    return res;
}

int relF_injective(FILE *fd, file_t *file) {
    int res;
    int attr;
    line_t *line;
    rel_t *rel;
    int value;

    line = file->lines[file->index];

    res = getAttrs(fd, &attr, 1, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr]->type != LINE_REL)
        return ERR_VALUE;
    rel = file->lines[attr]->un.rel;

    value = relFunction(rel);
    if (!value) {
        line->type = LINE_BOOL;
        line->un.value = value;
        return res;
    }

    for (uint32_t y = 0; y < rel->size; y++) {
        value = 1;
        for (uint32_t x = 0; x < rel->size; x++) {
            if (relGetBit(x, y, rel))
                --value;
        }
        if (value < 0)
            break;
    }

    line->type = LINE_BOOL;
    line->un.value = !value;

    return res;
}

int relF_surjective(FILE *fd, file_t *file) {
    int res;
    int attr;
    line_t *line;
    rel_t *rel;
    int value;

    line = file->lines[file->index];

    res = getAttrs(fd, &attr, 1, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr]->type != LINE_REL)
        return ERR_VALUE;
    rel = file->lines[attr]->un.rel;

    value = relFunction(rel);
    if (!value) {
        line->type = LINE_BOOL;
        line->un.value = value;
    }

    for (uint32_t y = 0; y < rel->size; y++) {
        value = 0;
        for (uint32_t x = 0; x < rel->size; x++)
            if (relGetBit(x, y, rel)) {
                ++value;
                break;
            }
        if (value == 0)
            break;
    }

    line->type = LINE_BOOL;
    line->un.value = value;

    return res;
}

int relF_bijective(FILE *fd, file_t *file) {
    int res;
    int attr;
    line_t *line;
    rel_t *rel;
    int value;

    line = file->lines[file->index];

    res = getAttrs(fd, &attr, 1, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr]->type != LINE_REL)
        return ERR_VALUE;
    rel = file->lines[attr]->un.rel;

    value = relFunction(rel);
    if (!value) {
        line->type = LINE_BOOL;
        line->un.value = value;
        return res;
    }

    for (uint32_t y = 0; y < rel->size; y++) {
        value = 0;
        for (uint32_t x = 0; x < rel->size; x++)
            if (relGetBit(x, y, rel))
                ++value;
        if ((value == 0) || (value > 1))
            break;
    }

    line->type = LINE_BOOL;
    line->un.value = (value > 1) ? 0 : value;

    return res;
}