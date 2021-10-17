// This file contains the functions declarations for the library that controls the keypad
#include "main.h"
#include <stdbool.h>
#include "CreateProblem.h"

#define MAX_LED 198
#define ROW_NUM 18
#define COL_NUM 11

extern int datasentflag;

extern char buffer[50];
extern char name[50];
extern char grade[5];
char letter_move[50];
char number_move[50];
extern char isTop[50];
extern char nMoves[1];
extern uint8_t buffer_index;
extern uint8_t timer_count;

extern uint8_t name_flag, grade_flag, num_move_flag, let_move_flag, isTop_flag, nMove_flag;

extern int msgLen;
extern char msgDebug[128];


// Eats a letter and a number and return the position of the LED
// following the serial connection
uint8_t LED_findPos(char letter, uint8_t number);

// eats the ID of the problem and generates an array with
// colors encoded in correct places
void problem_genArray(Problem *p);

// Fills the LED color matrix with 0 everywhere
void LED_setAllBlack();

// Fills the LED color matrix with 255 everywhere
void LED_setAllWhite();

// Fills the LED color matrix with random numbers everywhere
void LED_setAllRand();

// Sets the color of a specific LED
void LED_setColor(int LEDnum, int red, int green, int blue);

// Sets the color of a specific Hold
void LED_testHold(char letter, int number);

void LED_randSetRand();

// Generated the PWM and sends the signal to the LED strip
void WS2811_Send(void);


// Handle the message received by the bluetooth
void MessageHandler();

void BluetoothReceived();

void passProlemPtr(Problem * ptr);
