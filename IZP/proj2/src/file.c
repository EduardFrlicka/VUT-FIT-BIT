#include "file.h"

/**
 * @brief load line and append to file->lines array
 *
 * @param fd file from which will line be loaded
 * @param file struct where will be line loaded
 * @return 0 on success, error code on failure
 */
int loadLine(FILE *fd, file_t *file);

file_t *fileInit() {
    file_t *file;
    line_t **tmp;

    /*allocating file structure*/
    file = calloc(1, sizeof(file_t));
    if (!file)
        return NULL;

    file->alllocated = 1;
    file->index = 0;

    /*allocating array of lines*/
    tmp = realloc(file->lines, file->alllocated * sizeof(line_t **));
    if (!tmp) {
        free(file);
        return NULL;
    }
    file->lines = tmp;

    return file;
}

int loadLine(FILE *fd, file_t *file) {
    int c = fgetc(fd);
    int res;

    /*if there is no place in file, realloc*/
    if (file->alllocated <= file->index) {
        file->alllocated *= 2;
        line_t **tmp = realloc(file->lines, file->alllocated * sizeof(line_t **));
        NULLCHECK(tmp);
        file->lines = tmp;
    }

    line_t *line = calloc(sizeof(line_t), 1);
    NULLCHECK(line);
    file->lines[file->index] = line;

    switch (c) {
    case EOF:
        free(line);
        return EOF;

    case 'S':
    case 's':
        if (file->state > load_data)
            return ERR_FORMAT;
        file->state = load_data;
        line->type = LINE_SET;
        res = setLoad(fd, &(line->un.set));
        res |= setSubstEq((line->un.set), file->lines[0]->un.set) ? 0 : ERR_ELEMENT;
        break;

    case 'r':
    case 'R':
        if (file->state > load_data)
            return ERR_FORMAT;
        file->state = load_data;
        line->type = LINE_REL;
        res = relLoad(fd, file->lines[0]->un.set, &(line->un.rel));
        break;

    case 'c':
    case 'C':
        if (file->state > load_coms)
            return ERR_FORMAT;
        file->state = load_coms;
        res = doCom(fd, file);
        break;

    default:
        printf("Line err format\n");
        res = ERR_FORMAT;
        break;
    }

    file->index++;
    return res;
}

int fileLoad(FILE *fd, file_t *file) {
    int res;

    /*load first character and check if it is U (on first line should be defined universum)*/
    int c = fgetc(fd);
    if (c != 'u' && c != 'U')
        return ERR_FORMAT;

    /*load universum*/
    line_t *line = calloc(sizeof(line_t), 1);
    NULLCHECK(line);
    file->lines[file->index++] = line;
    line->type = LINE_UNI;

    file->state = load_universe;

    res = setLoad(fd, &(line->un.set));

    /*while everything is ok, load all other lines*/
    while (!res)
        res = loadLine(fd, file);

    if (file->state < load_coms)
        return ERR_FORMAT;
    /*close file and return 0 if everything ok, else return error code*/
    return res == EOF ? 0 : res;
}

void filePrint(file_t *file) {
    line_t *line;
    for (__u_int i = 0; i < file->index; ++i) {
        line = file->lines[i];
        switch (line->type) {
        case LINE_REL:
            printf("R");
            relPrint(line->un.rel);
            break;
        case LINE_SET:
            printf("S");
            setPrint(line->un.set);
            break;
        case LINE_UNI:
            printf("U");
            setPrint(line->un.set);
            break;

        case LINE_BOOL:
            if (line->un.value)
                printf("true\n");
            else
                printf("false\n");
            break;

        case LINE_INT:
            printf("%d\n", line->un.value);
            break;

        default:
            // printf("default\n");
            break;
        }
    }
}

void fileFree(file_t *file) {
    line_t *line;
    // int res = 0;
    for (__u_int i = 0; i < file->index; ++i) {
        line = file->lines[i];
        switch (line->type) {
        case LINE_REL:
            relFree(line->un.rel);
            free(line);
            break;
        case LINE_SET:
            setFree(line->un.set);
            free(line);
            break;
        case LINE_UNI:
            setFree(line->un.set);
            free(line);
            break;

        default:
            free(line);
            // res = ERR_INTERNAL;
            break;
        }
    }
    free(file->lines);
    free(file);
    // return res;
}
