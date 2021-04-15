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




// ### GPIO connections are:
//	C0 - row 1
//	C1 - row 2
//	C2 - row 3
//	C3 - row 4
//	C4 - col 1
//	C5 - col 2
//	C6 - col 3
//	C7 - col 4

// *********************************************


uint8_t decode_key(uint8_t row, uint8_t col){
	uint8_t key;

	switch(row){
		case(1):
			if (col==bit0) key=1;
			if (col==bit1) key=4;
			if (col==bit2) key=7;
			if (col==bit3) key=40;
			break;
		case(2):
			if (col==bit0) key=2;
			if (col==bit1) key=5;
			if (col==bit2) key=8;
			if (col==bit3) key=0;
			break;
		case(3):
			if (col==bit0) key=3;
			if (col==bit1) key=6;
			if (col==bit2) key=9;
			if (col==bit3) key=60;
			break;
		case(4):
			if (col==bit0) key=10;
			if (col==bit1) key=20;
			if (col==bit2) key=30;
			if (col==bit3) key=40;
			break;
		default:
			key=0;
	}

	return key;
}



// Perform a polling on each row in order to detect the selection of a key
uint8_t read_key(){

	uint8_t key =0;
	uint8_t col_read = 0;

	// stay in polling and wait for a button to be pressed
	while(true){

		// *** FIRST row
		GPIOC->ODR = bit0;						// write 4 status of row GPIO (enable GPIO of row1)
		col_read = (GPIOC->IDR & 0xF0)>>4;		// read 4 status of col GPIO
		if(col_read != no_bit) key = decode_key(1, col_read);		// decode which button has been pressed
		if(key!=0) return key;

		// *** SECOND row
		GPIOC->ODR = bit1;
		col_read = (GPIOC->IDR & 0xF0)>>4;
		if(col_read != no_bit) key = decode_key(2, col_read);
		if(key!=0) return key;

		// *** THIRD row
		GPIOC->ODR = bit2;
		col_read = (GPIOC->IDR & 0xF0)>>4;
		if(col_read != no_bit) key = decode_key(3, col_read);
		if(key!=0) return key;

		// *** FORTH row
		GPIOC->ODR = bit3;
		col_read = (GPIOC->IDR & 0xF0)>>4;
		if(col_read != no_bit) key = decode_key(4, col_read);
		if(key!=0) return key;
	}

	return 0;
}


uint32_t keypad_getNumber(){

	uint8_t arr[10];
	uint32_t number=0;
	uint8_t key;
	uint8_t i=0;

	char msg[100];
	int msg_len;

	PRINTF("\n\r The digits selected are: ");

	// until I press the ENTER button do:
	while(true){

		key = read_key();		// returns the digit input

		if(key==10){			// A, exit insertion number
			break;
		}else if(key==20){		// B, nothing

		}else if(key==30){		// C, nothing

		}else if(key==40){		// *, nothing

		}else if(key==50){		// #, nothing

		}else if(key==60){		// D, delete last digit
			i=-1;
			arr[i]=0;
			PRINTF("DEL ");
		}else{					// number keys
			arr[i]=key;
			PRINTF8("%d ", arr[i]);
			i+=1;
		}

		HAL_Delay(200);		// To avoid long press error
	}

	// Transform the array in number
	for(int j=i; j>0; j--){
		number += pow(10, j-1)*arr[i-j];
	}
	PRINTF32("\n\r Final number obtained is: %d", number);

	return number;
}
