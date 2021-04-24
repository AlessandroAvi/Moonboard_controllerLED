#include <stdio.h>
#include <string.h>


typedef struct Problem{
    int moveSize;
    char* moveNumbers[15];
    char* moveLetters[15];
    char* name[20];
    char* grade[3];
    char* startFinish[15];
};

void createProblem(struct Problem *p, int id);
