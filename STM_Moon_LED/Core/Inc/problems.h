// This file contains the functions declarations for the library that controls the keypad
#include "main.h"
#include "stdbool.h"
#include "string.h"


struct ProblemInfo{

	//string name;

	char move_char[50];
	uint8_t move_int[50];
	bool move_st[50];

}problemInfo;



void problem_search(ProblemInfo *p, uint32_t id);

