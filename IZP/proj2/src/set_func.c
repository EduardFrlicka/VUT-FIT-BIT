#include "set_func.h"

int setMinus(set_t *A, set_t *B, set_t **res) {
    set_t *tmp;

    while (A) {
        while (B && (strcmp(A->value, B->value) > 0)) {
            B = B->next;
        }
        if (B && (strcmp(A->value, B->value) < 0)) {
            tmp = setGetCpyElm(A);
            NULLCHECK(tmp);
            setInsert(res, tmp);
        }

        if (!B) {
            tmp = setGetCpyElm(A);
            NULLCHECK(tmp);
            setInsert(res, tmp);
        }
        A = A->next;
    }
    return 0;
}

int setSubstEq(set_t *A, set_t *B) {

    set_t *tmp;
    for (; A; A = A->next) {
        tmp = setFind(B, A->value);
        if (tmp)
            continue;
        return 0;
    }
    return 1;
}

int setF_empty(FILE *fd, file_t *file) {
    int attr;
    int res;
    int value;
    line_t *line;

    line = file->lines[file->index];

    res = getAttrs(fd, &attr, 1, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr]->type != LINE_SET)
        return ERR_VALUE;

    value = file->lines[attr]->un.set == NULL;
    line->type = LINE_BOOL;
    line->un.value = value;

    return 0;
}

int setF_card(FILE *fd, file_t *file) {
    int res;
    int attr;
    line_t *line;
    int value;

    line = file->lines[file->index];

    res = getAttrs(fd, &attr, 1, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr]->type != LINE_SET)
        return ERR_VALUE;

    value = 0;
    for (set_t *ptr = file->lines[attr]->un.set; ptr; ptr = ptr->next)
        ++value;

    line->type = LINE_INT;
    line->un.value = value;

    return res;
}

int setF_complement(FILE *fd, file_t *file) {
    int res;
    int attr;
    line_t *line;
    set_t *value;

    line = file->lines[file->index];

    res = getAttrs(fd, &attr, 1, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr]->type != LINE_SET)
        return ERR_VALUE;

    value = NULL;
    res |= setMinus(file->lines[0]->un.set, file->lines[attr]->un.set, &value);

    line->type = LINE_SET;
    line->un.set = value;

    return res;
}

int setF_union(FILE *fd, file_t *file) {
    int res;
    int attr[2];
    line_t *line;
    set_t *value;
    set_t *tmp;
    set_t *A;
    set_t *B;

    line = file->lines[file->index];

    res = getAttrs(fd, attr, 2, file->index);
    if (res > 0)
        return res;

    value = NULL;

    if (file->lines[attr[0]]->type != LINE_SET)
        return ERR_VALUE;
    if (file->lines[attr[1]]->type != LINE_SET)
        return ERR_VALUE;

    A = file->lines[attr[0]]->un.set;
    B = file->lines[attr[1]]->un.set;

    while (A) {
        tmp = setGetCpyElm(A);
        NULLCHECK(tmp);
        setInsert(&value, tmp);
        A = A->next;
    }

    for (; B; B = B->next) {
        if (!value) {
            tmp = setGetCpyElm(B);
            NULLCHECK(tmp);
            setInsert(&value, tmp);
            continue;
        }

        tmp = setFind(value, B->value);
        if (tmp)
            continue;
        tmp = setGetCpyElm(B);
        NULLCHECK(tmp);
        setInsert(&value, tmp);
    }

    line->type = LINE_SET;
    line->un.set = value;

    return res;
}

int setF_intersect(FILE *fd, file_t *file) {
    int res;
    int attr[2];
    line_t *line;
    set_t *value;
    set_t *tmp;
    set_t *A;
    set_t *B;

    line = file->lines[file->index];

    res = getAttrs(fd, attr, 2, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr[0]]->type != LINE_SET)
        return ERR_VALUE;
    if (file->lines[attr[1]]->type != LINE_SET)
        return ERR_VALUE;

    value = NULL;

    A = file->lines[attr[0]]->un.set;
    B = file->lines[attr[1]]->un.set;

    while (A) {

        while (B && (strcmp(A->value, B->value) > 0)) {
            B = B->next;
        }
        if (B && (strcmp(A->value, B->value) == 0)) {
            tmp = setGetCpyElm(A);
            NULLCHECK(tmp);
            setInsert(&value, tmp);
        }

        A = A->next;
    }

    line->type = LINE_SET;
    line->un.set = value;

    return res;
}

int setF_minus(FILE *fd, file_t *file) {
    int res;
    int attr[2];
    line_t *line;
    set_t *value;
    set_t *A;
    set_t *B;

    line = file->lines[file->index];

    res = getAttrs(fd, attr, 2, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr[0]]->type != LINE_SET)
        return ERR_VALUE;
    if (file->lines[attr[1]]->type != LINE_SET)
        return ERR_VALUE;

    value = NULL;

    A = file->lines[attr[0]]->un.set;
    B = file->lines[attr[1]]->un.set;

    res |= setMinus(A, B, &value);

    line->type = LINE_SET;
    line->un.set = value;

    return res;
}

int setF_subseteq(FILE *fd, file_t *file) {
    int res;
    int attr[2];
    line_t *line;
    int value;
    set_t *A;
    set_t *B;

    line = file->lines[file->index];

    res = getAttrs(fd, attr, 2, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr[0]]->type != LINE_SET)
        return ERR_VALUE;
    if (file->lines[attr[1]]->type != LINE_SET)
        return ERR_VALUE;

    A = file->lines[attr[0]]->un.set;
    B = file->lines[attr[1]]->un.set;

    value = setSubstEq(A, B);

    line->type = LINE_BOOL;
    line->un.value = value;

    return res;
}

int setF_subset(FILE *fd, file_t *file) {
    int res;
    int attr[2];
    line_t *line;
    int value1, value2;
    set_t *A;
    set_t *B;

    line = file->lines[file->index];

    res = getAttrs(fd, attr, 2, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr[0]]->type != LINE_SET)
        return ERR_VALUE;
    if (file->lines[attr[1]]->type != LINE_SET)
        return ERR_VALUE;

    A = file->lines[attr[0]]->un.set;
    B = file->lines[attr[1]]->un.set;

    value1 = setSubstEq(A, B);
    value2 = setSubstEq(B, A);

    DEBUG("%d %d", value1, value2);

    line->type = LINE_BOOL;
    line->un.value = value1 && !value2;

    return res;
}

int setF_equals(FILE *fd, file_t *file) {
    int res;
    int attr[2];
    line_t *line;
    int value1, value2;
    set_t *A;
    set_t *B;

    line = file->lines[file->index];

    res = getAttrs(fd, attr, 2, file->index);
    if (res > 0)
        return res;

    if (file->lines[attr[0]]->type != LINE_SET)
        return ERR_VALUE;
    if (file->lines[attr[1]]->type != LINE_SET)
        return ERR_VALUE;

    A = file->lines[attr[0]]->un.set;
    B = file->lines[attr[1]]->un.set;

    value1 = setSubstEq(A, B);
    value2 = setSubstEq(B, A);

    line->type = LINE_BOOL;
    line->un.value = value1 && value2;

    return res;
}