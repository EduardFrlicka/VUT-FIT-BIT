#include "error.h"
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <gelf.h>
#include <libelf.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <unistd.h>

typedef unsigned char uchar;

int parse_file(char *filename);