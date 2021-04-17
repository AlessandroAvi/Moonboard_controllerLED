// This file contains the functions declarations for the library that controls the keypad
#include "main.h"


#define 		no_bit		00000000
#define 		bit0 		00000001
#define 		bit1 		00000010
#define 		bit2 		00000100
#define 		bit3 		00001000
#define 		bit4 		00010000
#define 		bit5 		00100000
#define 		bit6 		01000000
#define 		bit7 		10000000


// Eats a letter and a number and return the position of the LED
// following the serial connection
uint8_t LED_findPos(char letter, uint8_t number);


// eats the ID of the problem and generates an array with
// colors encoded in correct places
void problem_genArray(uint32_t id, ProblemInfo *problem);


void LED_setAllBlack(uint8_t *LEDbuffer);

void LED_setAllWhite(uint8_t *LEDbuffer);
