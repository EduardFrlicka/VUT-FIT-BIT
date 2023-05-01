#include "proj2.h"
 
int main(int argc, char *argv[])
{
    srand(time(0));

    
    if (argc != 6){
        fprintf(stderr, "ERROR: wrong number of parameters\n");
        return 1;
        }
    int PI = parse(argv[1]);
    if(PI<0) {fprintf(stderr,"ERROR: value out of bounds\n"); return 2;}
    int IG = parse(argv[2]);
    if(IG<0||IG>2000) {fprintf(stderr,"ERROR: value out of bounds\n"); return 2;}
    int JG = parse(argv[3]);
    if(JG<0||JG>2000) {fprintf(stderr,"ERROR: value out of bounds\n"); return 2;}
    int IT = parse(argv[4]);
    if(IT<0||IT>2000) {fprintf(stderr,"ERROR: value out of bounds\n"); return 2;}
    int JT = parse(argv[5]);
    if(JT<0||JT>2000) {fprintf(stderr,"ERROR: value out of bounds\n"); return 2;}

    FILE *file = fopen("proj2.out", "w");
    if(file==NULL) {fprintf(stderr, "ERROR: faild to open file\n"); return 3;}

    law = sem_open(sem_name("law"), O_CREAT | O_EXCL, 0666, 0 /*počáteční hodnota*/);
    par_sem = sem_open(sem_name("par_semaphore"), O_CREAT | O_EXCL, 0666, 0 /*počáteční hodnota*/);
    write_sem = sem_open(sem_name("write_semaphore"), O_CREAT | O_EXCL, 0666, 1 /*počáteční hodnota*/);
    judge = sem_open(sem_name("judge"), O_CREAT | O_EXCL, 0666, 1 /*počáteční hodnota*/);
    imm = sem_open(sem_name("imm"), O_CREAT | O_EXCL, 0666, 0 /*počáteční hodnota*/);
    mymap(test);
    mymap(A);
    mymap(NE);
    mymap(NB);
    mymap(NC);

    *A = 1;
    *test = 0;

    //FILE *file = stdout;
    if (fork() == 0)
        doGenerator(file, PI, IG, IT);
    else{
         if (fork() == 0)
        doJudge(file, PI, JG, JT);
    }
    PI += 3;
    for (int i = 0; i < PI; i++)
        sem_wait(par_sem);

    //printf("END\n");
    //writeOut(file, "%d Parent: %d\n",(*test)++, getpid());

    unmap(A);
    unmap(test);
    unmap(NB);
    unmap(NC);
    unmap(NE);
    sem_close(law);
    sem_unlink(sem_name("law"));
    sem_close(par_sem);
    sem_unlink(sem_name("par_semaphore"));
    sem_close(write_sem);
    sem_unlink(sem_name("write_semaphore"));
    sem_close(judge);
    sem_unlink(sem_name("judge"));
    sem_close(imm);
    sem_unlink(sem_name("imm"));
    fclose(file);
    //printf("par end\n");
    return 0;
}


void writeOut(int NEc, int NCc, int NBc, bool print, FILE *file, const char *fmt, ...)
{

    va_list args;
    va_start(args, fmt);
    sem_wait(write_sem);
    *NE += NEc;
    *NC += NCc;
    *NB += NBc;
    fprintf(file, "%d\t:  ", (*A)++);
    vfprintf(file, fmt, args);
    if (print)
        fprintf(file, ": %d\t: %d\t: %d\n", (*NE), (*NC), (*NB));
    fflush(file);
    sem_post(write_sem);
    va_end(args);
}

//___________________________________________________________________________________________________________________________________________________________________
//                                                                                                                                                          IMMIGRANT
void doChild(FILE *file, int internalNumber, int IT)
{
    writeOut(0, 0, 0, false, file, "IMM %d\t: starts\n", internalNumber);

    // VSTUP
    sem_wait(judge);
    writeOut(1, 0, 1, true, file, "IMM %d\t: enters\t\t", internalNumber);
    sem_post(judge);

    msleep(IT);
    // REGISTRACIA
    writeOut(0, 1, 0, true, file, "IMM %d\t: checks\t\t", internalNumber);
    sem_post(imm);

    // SUHLAS
    sem_wait(law);
    writeOut(0, 0, 0, true, file, "IMM %d\t: wants certificate\t", internalNumber);
    msleep(IT);
    writeOut(0, 0, 0, true, file, "IMM %d\t: got certificate\t", internalNumber);

    // ODCHOD
    sem_wait(judge);
    writeOut(0, 0, -1, true, file, "IMM %d\t: leaves\t\t", internalNumber);
    sem_post(judge);

    sem_post(par_sem);
    //printf("IMM end\n");
    exit(0);
}

//___________________________________________________________________________________________________________________________________________________________________
//                                                                                                                                                          GENERATOR

void doGenerator(FILE *file, int PI, int IG, int IT)
{
    for (int i = 1; i <= PI; i++)
    {
        msleep(IG);
        if (fork() == 0)
            doChild(file, i, IT);
    }

    sem_post(par_sem);
    //printf("generator end\n");
    exit(0);
}

//___________________________________________________________________________________________________________________________________________________________________
//                                                                                                                                                              JUDGE

void doJudge(FILE *file, int PI, int JG, int JT)
{
    int POI = 0;
    while (POI < PI)
    {
        msleep(JG);

        //Entering
        writeOut(0, 0, 0, false, file, "JUDGE\t: wants to enter\n");
        sem_wait(judge);
        writeOut(0, 0, 0, true, file, "JUDGE\t: enters\t\t");
        // Waiting for registrations
        if (*NE != *NC)
            writeOut(0, 0, 0, true, file, "JUDGE\t: waits for imm\t");
        while (*NE != *NC)
        {
            //printf("baf:D\n");
            sem_wait(imm);
        }

        // Confirmation
        writeOut(0, 0, 0, true, file, "JUDGE\t: starts confirmation\t");
        int topass = *NC;
        POI += topass;
        *NE = 0;
        *NC = 0;
        msleep(JT);
        writeOut(0, 0, 0, true, file, "JUDGE\t: ends confirmation\t");

        // awakening IMM

        for (int i = 0; i < topass; i++)
        {
            sem_post(law);
        }

        // Leaving
        msleep(JT);
        writeOut(0, 0, 0, true, file, "JUDGE\t: leaves\t\t");
        sem_post(judge);
    }
    // ends
    writeOut(0, 0, 0, false, file, "JUDGE\t: finishes\n");
    //printf("Judge end\n");
    sem_post(par_sem);
    exit(0);
}

//___________________________________________________________________________________________________________________________________________________________________
//                                                                                                                                                               MAIN

int parse(char *arg){
    char * buff=NULL;
    int result = (int)strtol(arg, &buff,10);
    if(*buff!=0){
        fprintf(stderr,"ERROR: invalid argument\n");
        exit(1);
        }
    return result;
}
