// This file contains the functions declarations for the library that controls the keypad
#include "main.h"
#include <stdbool.h>
#include "createProblem.h"

#define MAX_LED 198
#define ROW_NUM 18
#define COL_NUM 11

extern int datasentflag;


// Eats a letter and a number and return the position of the LED
// following the serial connection
uint8_t LED_findPos(char letter, uint8_t number);


// eats the ID of the problem and generates an array with
// colors encoded in correct places
void problem_genArray(struct Problem *p, uint32_t id);


void LED_setAllBlack();

void LED_setAllWhite();

void LED_setColor(int LEDnum, int red, int green, int blue);

void WS2811_Send(void);

void send_DMA(int green, int red, int blue);
