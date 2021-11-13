// This file contains the functions declarations for the library that controls the LED and the bluetooth devide
#include "main.h"
#include <stdbool.h>
#include "CreateProblem.h"


// *****************************
// KEYPAD CODE


#define MAX_LED 198				// number of LEDS that are mounted on the moonboard
#define ROW_NUM 18				// number of LEDS for each row
#define COL_NUM 11				// number of LEDS for each column

extern int datasentflag;		// flag that is triggered when the data is sent as a PWM

extern int msgLen;				// debug message buffer length
extern char msgDebug[128];		// debug message buffer


// Transforms the couple letter-number of a hold in a number that represents a specific LED
uint8_t LED_findPos(char letter, uint8_t number);


// Reads the info from the struct and saves the correct color in the PWM array
void problem_genArray(Problem * p);


// Fills the LED color matrix with 0 everywhere (LEDS are all black)
void LED_setAllBlack();


// Fills the LED color matrix with 255 everywhere (LEDS are all white)
void LED_setAllWhite();


// Fills the LED color matrix with random numbers everywhere
void LED_setAllRand();


// Sets the color of a specific LED
void LED_setColor(int LEDnum, int red, int green, int blue);


// Sets the color of a specific hold
void LED_testHold(char letter, int number);


// Sets random colors for half of the LEDS
void LED_randSetRand();


// Generated the PWM signal and sends it to the LED strip
void WS2811_Send(void);






// *****************************
// BLUETOOTH CODE

extern uint8_t READ_ID;				// Flag used in the bluetooth communication (reads the type of info contained in the msg)

extern uint8_t buffer_index;	// Index for iterating over the receive message

extern uint8_t blt_rx[6];		// Flags that leep track fo what has been received

extern char id_buffer[1];	// Buffer that holds the type of the info
extern char buffer[50];			// Buffer that holds the info sent from android app



char name_buff[50];
char grade_buff[50];
char moveLetter_buff[50];
char moveNumber_buff[50];
char startFinish_buff[50];
char nHolds_buff[50];

// Called once in a while in the interrupt handler, saves the message in the correct container
void MessageHandler(Problem * p);
