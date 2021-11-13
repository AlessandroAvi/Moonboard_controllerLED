// This file contains the functions for the control of the keypad.
// The idea is to link the 8 output pins of the keypad to GPIOC pins
// Raise high one signal row at a time and read the signal coming from the column
// First 4 pins of GPIOC are dedicated to rows, last 4 are dedicated to columns
// ROWS -> LSB          COLS -> MSB

#include "keypad.h"
#include "stdbool.h"
#include "usart.h"
#include <math.h>
#include <stdio.h>
#include "debug.h"
#include "lcd16x2_i2c.h"
#include "i2c.h"


// ### Keypad layout
// 1  2  3  A
// 4  5  6  B
// 7  8  9  C
// *  0  #  D

// A (10)= ENTER (number input is finished)
// B (20)=
// C (30)=
// * (40)=
// # (50)=
// D (60)= DELETE (delete the last digit)

// In this case the GPIO pins that should be connected to the keyboard are:

// C7  -> row1
// A9  -> row2
// A8  -> row3
// B10 -> row4

// B4  -> col1
// B5  -> col2
// B3  -> col3
// A10 -> col4

// DEFINE 99 AS THE BAD VALUE THAT SHOULD NOT BE CONSIDERED

// ********************************************************

//#define debug_output



uint8_t read_GPIO(){

	if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)){			// if GPIO is low
		while(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4));	// wait till GPIO return high
		return 1;
	}
	if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)){
		while(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5));
		return 2;
	}
	if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)){
		while(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3));
		return 3;
	}
	if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)){
		while(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10));
		return 4;
	}

	return 99;
}



void write_GPIO(uint8_t riga){

	GPIO_PinState state_row1 = GPIO_PIN_SET;
	GPIO_PinState state_row2 = GPIO_PIN_SET;
	GPIO_PinState state_row3 = GPIO_PIN_SET;
	GPIO_PinState state_row4 = GPIO_PIN_SET;

	if(riga==1){
		state_row1 = GPIO_PIN_RESET;
	}else if(riga==2){
		state_row2 = GPIO_PIN_RESET;
	}else if(riga==3){
		state_row3 = GPIO_PIN_RESET;
	}else if(riga==4){
		state_row4 = GPIO_PIN_RESET;
	}

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, state_row1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, state_row2);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, state_row3);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, state_row4);
}



uint8_t decode_key(uint8_t row, uint8_t col){
	uint8_t key;

	switch(row){
		case(1):
			if (col==1) key=1;
			if (col==2) key=2;
			if (col==3) key=3;
			if (col==4) key=10;
			break;
		case(2):
			if (col==1) key=4;
			if (col==2) key=5;
			if (col==3) key=6;
			if (col==4) key=20;
			break;
		case(3):
			if (col==1) key=7;
			if (col==2) key=8;
			if (col==3) key=9;
			if (col==4) key=30;
			break;
		case(4):
			if (col==1) key=40;
			if (col==2) key=0;
			if (col==3) key=50;
			if (col==4) key=60;
			break;
		default:
			key=99;
			break;
	}

	return key;
}



// Perform a polling on each row in order to detect the selection of a key
uint8_t read_key(){

	uint8_t key = 99;     		// 99 is the DISCARD number
	uint8_t col_read = 99;

	// stay in polling and wait for a button to be pressed
	while(true){

		// *** FIRST row
		write_GPIO(1);								// write 4 status of row GPIO (enable GPIO of row1)
		col_read = read_GPIO();						// read 4 status of col GPIO
		if(col_read != 99) {
			key = decode_key(1, col_read);		// decode which button has been pressed
			return key;
		}

		// *** SECOND row
		write_GPIO(2);
		col_read = read_GPIO();
		if(col_read != 99){
			key = decode_key(2, col_read);
			return key;
		}

		// *** THIRD row
		write_GPIO(3);
		col_read = read_GPIO();
		if(col_read != 99){
			key = decode_key(3, col_read);
			return key;
		}

		// *** FORTH row
		write_GPIO(4);
		col_read = read_GPIO();
		if(col_read != 99){
			key = decode_key(4, col_read);
			return key;
		}
	}

	return 99;
}



uint32_t keypad_getNumber(){

	uint8_t inputDigit[10];
	uint32_t number=0;
	uint8_t key;
	uint8_t i=0;

#ifdef debug_output
	PRINTF("\n\r The digits selected are: ");
#endif

	lcd16x2_i2c_printf("-> A to select");
	lcd16x2_i2c_2ndLine();
	lcd16x2_i2c_printf("ID:  ");
	// until I press the ENTER button do:
	while(true){

		key = read_key();		// returns the digit input

		if(key==99){
			lcd16x2_i2c_printf("Error...");
		}else if(key==10){			// A, exit insertion number
			lcd16x2_i2c_printf(";");
			break;
		}else if(key==20){		// B, nothing
			lcd16x2_i2c_printf("B");
		}else if(key==30){		// C, easter egg
			lcd16x2_i2c_printf("C");
			return 0;
		}else if(key==40){		// *, nothing
			lcd16x2_i2c_printf("*");
			PRINTF("* ");
		}else if(key==50){		// #, nothing
			lcd16x2_i2c_printf("#");
			PRINTF("# ");
		}else if(key==60){		// D, disco mode
			lcd16x2_i2c_printf("D");
			return 10000;
		}else if(key>=0 && key<=9){	// number keys
			inputDigit[i] = key;
			lcd16x2_i2c_printf("%d", inputDigit[i]);
			i+=1;
		}

		key=99;
	}

	// Transform the array in number
	for(int j=i; j>0; j--){
		number += pow(10, j-1)*inputDigit[i-j];
	}

#ifdef debug_output
	PRINTF32("\n\r     Final number obtained is: %d", number);
#endif
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
	return number;
}








