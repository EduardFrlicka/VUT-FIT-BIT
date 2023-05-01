   // htab_free.c
   // Řešení IJC-DU2
   // Datum:    23.04.2020
   // Autor:    Eduard Frlicka, FIT
   // Přeloženo:    gcc (GCC) 9.3.0
   // Zadanie:
   //   V jazyku C napište program "tail.c", který  ze zadaného
   //   vstupního souboru vytiskne posledních  10 řádků. Není-li
   //   zadán vstupní soubor, čte ze stdin. Je-li programu zadán
   //   parametr -n číslo, bude se tisknout tolik posledních řádků,
   //   kolik je zadáno parametrem 'číslo' > 0.
   //   Případná chybová hlášení tiskněte do stderr. Příklady:
   //     tail soubor
   //     tail -n +3 soubor
   //     tail -n 20 <soubor
   //   [Poznámka: výsledky by měly být +-stejné jako u POSIX příkazu tail]
   //
   //   Je povolen implementační limit na délku řádku (např. 1023 znaků),
   //   v případě prvního překročení mezí hlaste chybu na stderr (řádně otestujte)
   //   a pokračujte se zkrácenými řádky (zbytek řádku přeskočit/ignorovat).

#include "tail.h"

int main(int argc, char *argv[]){
    char *fileName=NULL;
    int lineCount = 10;
    bool skipLines = false;
    for(int i = 1; i < argc; i++)
        if(strstr(argv[i],"-n")){
            if(++i<argc){
                if(argv[i][0]=='+')
                    skipLines=true;
                lineCount = abs((int)strtol(argv[i],NULL,10));
                if(lineCount==0)
                fprintf(stderr, "No number after -n (or zero)\n");
            }
            else{
            fprintf(stderr, "Nothing after -n \n");
            return 1;
            }   
        } 
        else
        {
            if(fileName==NULL){
                fileName = malloc(strlen(argv[i])+1);
                if(fileName!=NULL)
                    strcpy(fileName,argv[i]);
                else{
                    fprintf(stderr, "Could not alloc enough memory\n");
                    return 1;
                }
            }
            else
            fprintf(stderr,"Wrong parameter: %s, file already specified: %s\n",argv[i],fileName);
        }
    if(fileName!=NULL)
    if(!exists(fileName)){
        fprintf(stderr, "File does not exist, try to specify another file\n");
        free(fileName);
        return 2;
    }
    if(lineCount==0 && !skipLines) return 0;
   // Debug(fileName);
   // DebugN(lineCount);
    FILE *file;
    if(fileName==NULL)
    file=stdin;
    else
    file=fopen(fileName,"r");
    
//_____________________________________________________________________
    if(skipLines){
        for(int i = 1; i < lineCount; i++)
            free(loadLine(file));
        char * ptr = loadLine(file);
        while(ptr != NULL){
            printf("%s",ptr);
            free(ptr);
            ptr = loadLine(file);
        }
    }
    else{
            char **lines = getLastLines(file,lineCount);
            for(int i = 0; i < lineCount; i++)
                if(lines[i]!=NULL){
                    printf("%s",lines[i]);
                    free(lines[i]);
                    }
            free(lines);
    }
    
    free(fileName);
    if(fileName!=NULL)
    fclose(file);
    return 0;
}

int abs(int number){
    return number > 0 ? number : -number;
}

char **getLastLines(FILE *fielPtr, int lineCount){
    char **lines = malloc(lineCount * sizeof(char *));
    for(int i = 0; i < lineCount; i++)
    lines[i]=NULL;
    char *newLine=loadLine(fielPtr);
    while(newLine!=NULL){  
        free(lines[0]);
        for(int i = 1; i < lineCount;i++)
            lines[i-1]=lines[i];
        lines[lineCount-1]=newLine;
        newLine=loadLine(fielPtr);
    }
    return lines;
}

char *loadLine(FILE *filePtr){
    int size=100;
    int sizeToRead=100;
    char *line=NULL;

    while(isNotLine(line, size, sizeToRead) || size==sizeToRead){
        line=realloc(line,size);
        if(line==NULL){
            fprintf(stderr,"Could not alloc enough memory\n");
            return NULL;
        }
        if(fgets(&line[(size - sizeToRead) ? size - sizeToRead -1 : size - sizeToRead],(size - sizeToRead) ? sizeToRead+1 : sizeToRead,filePtr)==NULL){
                free(line);
                return NULL;
        }    
        size+=sizeToRead;    
    }
    return line;
}

int isNotLine(char *line, int size, int sizeToRead){
    if(line!=NULL){
        if((int)strlen(line)<size-sizeToRead-1) return 0;
        if(strstr(line,"\n"))
            return 0;
        else
            return 1;
    }
    return 0;
}

int exists(const char *fileName)
{
    FILE *file;
    if ((file = fopen(fileName, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}