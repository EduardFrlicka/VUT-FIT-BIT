#ifndef _SET_FUNC
#define _SET_FUNC

#include "file.h"
#include "set.h"

#define getAttrs(fd, attrs, n_attrs, max) getAttrs((fd), (attrs), (n_attrs), (max), COM_ATTR_LEN)

int setF_empty(FILE *fd, file_t *file);
int setF_card(FILE *fd, file_t *file);
int setF_complement(FILE *fd, file_t *file);
int setF_union(FILE *fd, file_t *file);
int setF_intersect(FILE *fd, file_t *file);
int setF_minus(FILE *fd, file_t *file);
int setF_subseteq(FILE *fd, file_t *file);
int setF_subset(FILE *fd, file_t *file);
int setF_equals(FILE *fd, file_t *file);

int setSubstEq(set_t *A, set_t *B);

#endif