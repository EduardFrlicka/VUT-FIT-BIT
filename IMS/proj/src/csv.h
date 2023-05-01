#pragma once
#include "args.h"
#include "automat.h"
#include <iostream>
#include <stdio.h>

typedef enum ePrintField {
    printIteration,
    printWolfs,
    printSheep,
    printEmpty,
    printCougar,
    printNull,
} tPrintField;

class csvWriter {
  public:
    csvWriter(CA *automat, args_t *);
    /**
     * @brief Construct a new csv Writer object
     * 
     * @param automat 
     * @param filename 
     * @param header 
     */
    csvWriter(CA *automat, char *filename, char *header);
    ~csvWriter();

    void log();

  private:
    void writeLine(char *, ...);
    void writeCell(char *, ...);
    void initFields(char* header);

    FILE *fileptr;
    char *filename;
    char *header;
    std::vector<tPrintField> fields;
    size_t line;

    CA *automat;

    bool comma;
};