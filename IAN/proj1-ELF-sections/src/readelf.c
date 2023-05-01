#include "readelf.h"

// lot of this code was inspired or copied from: https://atakua.org/old-wp/wp-content/uploads/2015/03/libelf-by-example-20100112.pdf

int parse_file(char *filename) {
    int file;
    Elf *elf;
    GElf_Ehdr header;
    Elf_Scn *scn;
    GElf_Shdr shdr;
    Elf_Data *symtable;
    GElf_Phdr phdr;
    GElf_Sym sym;
    size_t n, shstrndx, i, start, end;
    Elf_Data *data;
    char *name, perm_X, perm_R, perm_W;

    file = open(filename, O_RDONLY);
    if (!file) {
        ERROR(ERR_FILE, "File %s does not exist, or low permissions", filename);
    };

    if (elf_version(EV_CURRENT) == EV_NONE) {
        ERROR(ERR_FILE, "elf version not set");
    }

    elf = elf_begin(file, ELF_C_READ_MMAP_PRIVATE, NULL);

    if (!elf)
        ERROR(ERR_FILE, "%s", elf_errmsg(-1));

    if (elf_kind(elf) != ELF_K_ELF)
        ERROR(ERR_FILE, "\"%s\" is not an ELF object.", filename);

    if (elf_getphdrnum(elf, &n) != 0)
        ERROR(ERR_FILE, "elf_getphdrnum () failed: %s.", elf_errmsg(-1));

    if (elf_getshdrstrndx(elf, &shstrndx) != 0)
        ERROR(ERR_FILE, "elf_getshdrstrndx () failed: %s.", elf_errmsg(-1));

    if (n == 0)
        printf("There are no program headers in this file.\n");
    else
        printf("Segment Type         Perm Sections\n");

    for (i = 0; i < n; i++) {
        if (gelf_getphdr(elf, i, &phdr) != &phdr)
            ERROR(ERR_FILE, "getphdr () failed: %s.", elf_errmsg(-1));

        printf("%02ld      ", i);

#define C(V)                                                                                                                                                                                           \
    case PT_##V:                                                                                                                                                                                       \
        printf("%-12s ", #V);                                                                                                                                                                          \
        break

        switch (phdr.p_type) {
            C(NULL);
            C(LOAD);
            C(DYNAMIC);
            C(INTERP);
            C(NOTE);
            C(SHLIB);
            C(PHDR);
            C(TLS);
            C(SUNWBSS);
            C(SUNWSTACK);
        default:
            printf("0x%-10X ", phdr.p_type);
            break;
        }
#undef C

        perm_X = '-';
        perm_R = '-';
        perm_W = '-';

        if (phdr.p_flags & PF_X)
            perm_X = 'X';
        if (phdr.p_flags & PF_R)
            perm_R = 'R';
        if (phdr.p_flags & PF_W)
            perm_W = 'W';

        printf("%c%c%c  ", perm_R, perm_W, perm_X);

        scn = NULL;
        start = phdr.p_offset;
        end = start + phdr.p_filesz;

        while ((scn = elf_nextscn(elf, scn)) != NULL) {
            if (gelf_getshdr(scn, &shdr) != &shdr)
                ERROR(ERR_FILE, "getshdr () failed: %s.", elf_errmsg(-1));

            if ((name = elf_strptr(elf, shstrndx, shdr.sh_name)) == NULL)
                ERROR(ERR_FILE, "elf_strptr () failed: %s.", elf_errmsg(-1));

            if (shdr.sh_type == SHT_NOBITS) {
                if (start <= shdr.sh_offset && start + phdr.p_memsz >= shdr.sh_offset + shdr.sh_size)
                    printf("%s ", name);
            } else {
                if (start <= shdr.sh_offset && end >= shdr.sh_offset + shdr.sh_size)
                    printf("%s ", name);
            }
        }

        printf("\n");
    }

    elf_end(elf);
    close(file);

    return 0;
}
