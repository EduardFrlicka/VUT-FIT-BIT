#include "error.h"
#include "readelf.h"


int main(int argc, char *argv[]) {
    if (argc != 2)
        ERROR(ERR_ARGUMENT, "Missing argument");

    int res = parse_file(argv[1]);
    
    return res;
}
