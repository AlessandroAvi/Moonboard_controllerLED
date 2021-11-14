#include <stdio.h>
#include <string.h>


typedef struct{
    int moveSize;
    char moveNumbers[160];
    char moveLetters[160];
    char name[100];
    char grade[11];
    char startFinish[160];
}Problem;

void problem_fetch(Problem *p, int id);
