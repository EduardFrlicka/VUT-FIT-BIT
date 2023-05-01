#ifndef _COM_H
#define _COM_H

#include "error.h"
#include "structs.h"
#include "file.h"
#include "set_func.h"
#include "rel_func.h"

#define COM_LEN 50
#define COM_ATTR_LEN 50

int doCom(FILE *fd, file_t *file);

#endif