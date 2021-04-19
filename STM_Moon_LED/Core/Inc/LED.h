// This file contains the functions declarations for the library that controls the keypad
#include "main.h"
#include "problems.h"
#include <stdbool.h>

#define MAX_LED 198
#define USE_BRIGHTNESS 1
#define ROW_NUM 18
#define COL_NUM 11

extern int datasentflag;



typedef struct ProblemInfo{

	char name[100];

	char move_char[50];
	uint8_t move_int[50];
	bool move_st[50];

}ProblemInfo;


void problem_search(ProblemInfo *problem, uint32_t id);






// Eats a letter and a number and return the position of the LED
// following the serial connection
uint8_t LED_findPos(char letter, uint8_t number);


// eats the ID of the problem and generates an array with
// colors encoded in correct places
void problem_genArray(ProblemInfo *problem, uint32_t id);


void LED_setAllBlack();

void LED_setAllWhite();





