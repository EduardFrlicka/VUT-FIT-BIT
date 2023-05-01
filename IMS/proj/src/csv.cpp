#include "csv.h"
#include <map>
#include <stdarg.h>

#define DEFAULT_FILENAME (char *)"simulation.csv"
#define DEFAULT_HEADER (char *)"iteration, wolfs, sheep, cougar, empty"

csvWriter::csvWriter(CA *automat, args_t *args) : csvWriter(automat, args->save_file, args->header) {
}

csvWriter::csvWriter(CA *automat, char *filename, char *header) {
    this->filename = DEFAULT_FILENAME;
    this->header = DEFAULT_HEADER;
    this->line = 0;

    this->automat = automat;

    if (filename)
        this->filename = filename;

    if (header)
        this->header = header;

    this->fileptr = fopen(this->filename, "w");
    if (!this->fileptr)
        ERROR(ERR_INTERNAL, "File %s could not be opened.", this->filename);

    this->initFields(this->header);

    this->writeLine(this->header);
}

csvWriter::~csvWriter() {
    if (this->fileptr)
        fclose(this->fileptr);
    printf("closed %s\n", this->filename);
}

void csvWriter::initFields(char *header) {
    char *field;
    char *tmp;
    tPrintField value;
    std::string key;

    /* making copy of header */
    tmp = (char *)calloc(strlen(header) + 1, 1);
    strcpy(tmp, header);

    /* translation dictionary */
    std::map<std::string, tPrintField> dict = {
        {"iteration", printIteration}, {"wolfs", printWolfs}, {"sheep", printSheep}, {"cougar", printCougar}, {"empty", printEmpty},
    };

    /* spliting header into tokens */
    field = strtok(tmp, " ,");
    while (field != NULL) {
        key = std::string(field);
        value = printNull;

        if (dict.find(key) != dict.end())
            value = dict[field];
        this->fields.push_back(value);

        field = strtok(NULL, " ,");
    }
}

void csvWriter::log() {
    for (auto i : this->fields) {
        switch (i) {
        default:
            this->writeCell("default");
            break;

        case printIteration:
            this->writeCell("%lu", this->line);
            break;

        case printWolfs:
            this->writeCell("%d", this->automat->wolf_count());
            break;

        case printSheep:
            this->writeCell("%d", this->automat->sheep_count());
            break;

        case printCougar:
            this->writeCell("%d", this->automat->cougar_count());
            break;

        case printEmpty:
            this->writeCell("%d", this->automat->empty_count());
            break;

        case printNull:
            this->writeCell("");
            break;
        }
    }
    this->writeLine("");
}

void csvWriter::writeLine(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(this->fileptr, fmt, args);
    fprintf(this->fileptr, "\n");
    va_end(args);
    this->line++;
    this->comma = false;
}

void csvWriter::writeCell(char *fmt, ...) {
    if (this->comma)
        fprintf(this->fileptr, ",");
    this->comma = true;
    va_list args;
    va_start(args, fmt);
    vfprintf(this->fileptr, fmt, args);
    va_end(args);
}
