#ifndef _REL_FUNC
#define _REL_FUNC

#include "file.h"
#include "rel.h"

#define getAttrs(fd, attrs, n_attrs, max) getAttrs((fd), (attrs), (n_attrs), (max), COM_ATTR_LEN)

int relF_reflexive(FILE *fd, file_t *file);
int relF_symetric(FILE *fd, file_t *file);
int relF_antisymetric(FILE *fd, file_t *file);
int relF_transitive(FILE *fd, file_t *file);
int relF_function(FILE *fd, file_t *file);
int relF_domain(FILE *fd, file_t *file);
int relF_codomain(FILE *fd, file_t *file);
int relF_injective(FILE *fd, file_t *file);
int relF_surjective(FILE *fd, file_t *file);
int relF_bijective(FILE *fd, file_t *file);

#endif