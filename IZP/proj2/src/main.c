#include "error.h"
#include "file.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void printErr(int code) {

    if (ERR_ARGUMENT & code)
        ERROR(code, "Wrong argument");

    if (ERR_ELEMENT & code)
        ERROR(code, "Wrong element");

    if (ERR_FORMAT & code)
        ERROR(code, "Wrong format of input file");

    if (ERR_INTERNAL & code)
        ERROR(code, "Some internal error or alloc failed");

    if (ERR_VALUE & code)
        ERROR(code, "Wrong value");
}

int workFile(const char *filename) {
    int res;
    file_t *file;
    FILE *fd;

    /*open file and check if file is succesfully opened*/
    fd = fopen(filename, "r");
    NULLCHECK(fd);

    /*allocating file structure*/
    file = fileInit();
    NULLCHECK(file);

    /*read and load file, then close file*/
    res = fileLoad(fd, file);
    fclose(fd);

    filePrint(file);

    /*free file struct*/
    fileFree(file);

    printErr(res);
    return res;
}

int main(int argc, char *argv[]) {
    ++argv;

    if (argc <= 1){
        ERROR(ERR_ARGUMENT, "Missing argumet");
        return ERR_ARGUMENT;
    }

    while (*argv) {
        char *filename = *argv;
        struct stat stats;

        if (stat(filename, &stats))
            ERROR(ERR_ARGUMENT, "\"%s\" - No such file or directory.", filename);

        if (S_ISDIR(stats.st_mode))
            ERROR(ERR_ARGUMENT, "\"%s\" - is name of directory.", filename);

        if (S_ISREG(stats.st_mode)) {
            return workFile(filename);
        }

        ++argv;
    }

    return ERR_ARGUMENT;
}
