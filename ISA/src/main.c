#include "client.h"
#include "error.h"
#include "server.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct __fileList {
    char *filename;
    struct __fileList *next;
} file_t;

int addFile(file_t **head, char *filename) {
    file_t *new = (file_t *)calloc(sizeof(file_t), 1);
    NULLCHECK(new);
    new->filename = filename;
    new->next = *head;
    *head = new;
    return 0;
}

int main(int argc __attribute__((unused)), char *argv[]) {
    char  *server_addr = NULL;
    file_t *head = NULL;
    int listen_opt = 0;

    if (getuid() != 0)
        ERROR(ERR_PERMISSION, "This command has to be run with superuser privileges (under the root user on most systems).");

    for (char **i = argv + 1; *i;) {
        if (!strcmp(*i, "-r")) {

            if (!*(i + 1))
                ERROR(ERR_ARGUMENT, "Argument \"%s\" expected value, no value given.", *i);

            for (++i; *i; ++i) {
                char *value = *i;
                struct stat path_stat;
                if (stat(value, &path_stat))
                    break;

                if (!S_ISREG(path_stat.st_mode))
                    ERROR(ERR_ARGUMENT, "Path \"%s\" is not file.", value);

                addFile(&head, value);
            }

            continue;
        }

        if (!strcmp(*i, "-s")) {
            char *value = *(i + 1);

            if (!value)
                ERROR(ERR_ARGUMENT, "Argument \"%s\" expected value, no value given.", *i);

            if (server_addr)
                ERROR(ERR_ARGUMENT, "Multiple use of argument \"%s\" is forbidden.", *i);

            server_addr = value;
            i += 2;
            continue;
        }

        if (!strcmp(*i, "-l")) {
            if (head || server_addr)
                WARNING("Argument \"%s\" should be used alone.", *i);

            if (listen_opt)
                WARNING("Multiple use of argument \"%s\" is redundant", *i);

            listen_opt = 1;
            ++i;
            continue;
        }

        ERROR(ERR_ARGUMENT, "Argument \"%s\" was not recognized.", *i);
    }

    if (listen_opt)
        return serverListen();

    if (!head || !server_addr)
        ERROR(ERR_ARGUMENT, "Argument \"%s\" missing.", server_addr ? "-r" : "-s");

    while (head) {
        if (access(head->filename, R_OK))
            ERROR(ERR_PERMISSION, "File \"%s\" does not exist.", head->filename);
        int res = send_file(head->filename, server_addr);
        if (res)
            return res;
        file_t *tmp = head;
        head = tmp->next;
        free(tmp);
    }

    return 0;
    // return test();

    // return send_file(filename, server_addr);
}
