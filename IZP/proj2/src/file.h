#ifndef _FILE_H
#define _FILE_H

#include "error.h"
#include "structs.h"
#include "rel.h"
#include "set.h"
#include "com.h"

#include <stdint.h>
#include <stdio.h>

#define LINE_UNI 'U'
#define LINE_SET 'S'
#define LINE_REL 'R'
#define LINE_INT 'N'
#define LINE_BOOL 'B'



/**
 * @brief initialize file_t structure
 * 
 * @return file_t *
 */
file_t *fileInit();

/**
 * @brief load file into file_t structure
 * 
 * @param fd file for loading
 * @param file struct where will be file loaded
 * @return 0 on success, error code on failure
 */
int fileLoad(FILE *fd, file_t *file);

/**
 * @brief print file_t structure
 * 
 * @param file structure to print
 */
void filePrint(file_t *file);

/**
 * @brief free whole file_t structure, including all containing elements
 * 
 * @param file  
 * @return 0 on success, error code on failure
 */
void fileFree(file_t *file);

#endif