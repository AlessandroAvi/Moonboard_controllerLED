#include "LED.h"
#include "gpio.h"
#include "tim.h"
#include <stdio.h>
#include <ctype.h>
#include "debug.h"
#include "lcd16x2_i2c.h"
#include <stdlib.h>


// PWM arrays that contain the duty cycles for the PWM
uint32_t pwmData_low[24*100+80];
uint32_t pwmData_high[24*100+80];
int datasentflag = 0;				// Flag that knows if the PWM duty cycles have been sent

uint8_t LED_Data[MAX_LED][4];		// Array that contains the colors for each single led

Problem * problem_Struc;

int msgLen = 0;
char msgDebug[128];


// Bluetooth definitions

uint8_t BLUETOOTH_FLAG = 0;

uint8_t DISCO_FLAG = 0;

char buffer[600];

char nMoves_buffer[1];


// *******************************************************************
// 					MANIPULATION OF THE LED MATRIX
// *******************************************************************


uint8_t LED_findPos(char letter, uint8_t number){

	uint8_t position;

	if (number==1){
		return 0;
	}

	switch(letter){
	case('A'):
		position = number-1;
		break;
	case('B'):
		position = 36-number;
		break;
	case('C'):
		if(number < 17){
			position = 34+number-1;
		} else {
			position = 34+number;
		}
		break;
	case('D'):
		position = 71-number;
		break;
	case('E'):
		position = 69+number;
		break;
	case('F'):
		if(number>11){
			position = 106-number;
		}else{
			position = 107-number;
		}
		break;
	case('G'):
		position = 104+number;
		break;
	case('H'):
		position = 142-number;
		break;
	case('I'):
		if(number>5){
			position = 140+number;
		}else{
			position = 139+number;
		}
		break;
	case('J'):
		position = 177-number;
		break;
	case('K'):
		position = 174+number;
		break;
	default:
		position=0;
		break;
	}


	return position;
}


uint8_t convert_digit(char dig1, char dig2){

	uint8_t res=0;

	if(dig1 == '1'){
		res = 10;
	}

	if(dig2 == '1'){
		res += 1;
	}else if(dig2 == '2'){
		res += 2;
	}else if(dig2 == '3'){
		res += 3;
	}else if(dig2 == '4'){
		res += 4;
	}else if(dig2 == '5'){
		res += 5;
	}else if(dig2 == '6'){
		res += 6;
	}else if(dig2 == '7'){
		res += 7;
	}else if(dig2 == '8'){
		res += 8;
	}else if(dig2 == '9'){
		res += 9;
	}else if(dig2 == '0'){
		res += 0;
	}

	return res;
}

void problem_genArray(Problem * p){

	uint16_t LedPos = 0;
	char     lettArr[p->moveSize];
	uint8_t  numArr[p->moveSize];
	uint8_t  typeArr[p->moveSize];

	uint8_t number_iter=0;

	LED_setAllBlack();	// put to 0 all the colors for each LED

	for(int j=0; j<p->moveSize; j++){
		lettArr[j] = p->moveLetters[j*2];	// extract letter of hold
		typeArr[j] = p->startFinish[j*2];	// extract type of hold

		if(p->moveNumbers[number_iter+1] != ','){
			numArr[j] = convert_digit(p->moveNumbers[number_iter], p->moveNumbers[number_iter+1]);
			number_iter += 3;
		} else {
			numArr[j] = convert_digit('0', p->moveNumbers[number_iter]);
			number_iter += 2;
		}
	}


	// for each move define the color of the corresponding LED
	for(int i=0; i<p->moveSize; i++){

		// transform letter and number in the led position
		LedPos = LED_findPos(lettArr[i], numArr[i]);

		// light the led depending if it's start, top or else
		if(typeArr[i]=='s'){			// START, color green
			LED_Data[LedPos][1] = 0;
			LED_Data[LedPos][2] = 250;
			LED_Data[LedPos][3] = 0;
		}else if(typeArr[i]=='e'){		// TOP, color red
			LED_Data[LedPos][1] = 250;
			LED_Data[LedPos][2] = 0;
			LED_Data[LedPos][3] = 0;
		}else if(typeArr[i]=='d'){		// DEFAULT, color blue
			LED_Data[LedPos][1] = 0;
			LED_Data[LedPos][2] = 0;
			LED_Data[LedPos][3] = 250;
		}else if(typeArr[i]=='c'){		// easter egg, color pink
			LED_Data[LedPos][1] = 250;
			LED_Data[LedPos][2] = 192;
			LED_Data[LedPos][3] = 203;
		}
	}
}



void LED_setAllBlack(){

	for(int i=0; i<MAX_LED; i++){
		LED_Data[i][1]=0;
		LED_Data[i][2]=0;
		LED_Data[i][3]=0;
	}
}



void LED_setAllWhite(){

	for(int i=0; i<MAX_LED; i++){
		LED_Data[i][1]=255;
		LED_Data[i][2]=255;
		LED_Data[i][3]=255;
	}
}



void LED_setAllRand(){

	for(int i=0; i<MAX_LED; i++){
		LED_Data[i][1]=rand();
		LED_Data[i][2]=rand();
		LED_Data[i][3]=rand();
	}
}


// sets the color of one specific LED
void LED_setColor(int LEDnum, int red, int green, int blue){
	LED_Data[LEDnum][0] = LEDnum;
	LED_Data[LEDnum][1] = red;
	LED_Data[LEDnum][2] = green;
	LED_Data[LEDnum][3] = blue;
}


void LED_testHold(char letter, int number){
	LED_setAllBlack();
	uint8_t LedPos = LED_findPos(letter, number);

	LED_Data[LedPos][1] = 250;
	LED_Data[LedPos][2] = 250;
	LED_Data[LedPos][3] = 250;
}


void LED_randSetRand(){

	LED_setAllBlack();

	for(int i=0; i<MAX_LED; i++){

		LED_Data[i][1]=rand();
		LED_Data[i][2]=rand();
		LED_Data[i][3]=rand();

	}
}




// *******************************************************************
// 						GENERATION OF PWM
// *******************************************************************


void WS2811_Send(void){

	uint16_t indx_low = 0;
	uint16_t indx_high = 0;
	uint32_t color;

	// wait 50 PWM pulses as a RESET before lighting all LED
	for(int i=0; i<40; i++){
		pwmData_low[indx_low] = 0;
		pwmData_high[indx_high] = 0;

		indx_low++;
		indx_high++;
	}

	// read from the color matrix each RGB color for each LED
	for(int i=0; i<MAX_LED; i++){
		color = ((LED_Data[i][1]<<16) | (LED_Data[i][2]<<8) | LED_Data[i][3]);

		if(i <= 94){	// LOW PWM DATA

			for(int i=23; i>=0; i--){
				if(color & (1<<i)){
					pwmData_low[indx_low]=43;
				}else{
					pwmData_low[indx_low]=18;
				}
				indx_low++;
			}

		} else {		// HIGH PWM DATA

			for(int i=23; i>=0; i--){
				if(color & (1<<i)){
					pwmData_high[indx_high]=43;
				}else{
					pwmData_high[indx_high]=18;
				}
				indx_high++;
			}

		}


	}

	// wait 50 PWM pulses as a RESET after lighting all LED
	for(int i=0; i<40; i++){
		pwmData_low[indx_low] = 0;
		pwmData_high[indx_high] = 0;

		indx_low++;
		indx_high++;
	}

	// start the transmission of the data just created
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)pwmData_low, indx_low);
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t *)pwmData_high, indx_high);


	while(!datasentflag){};
	datasentflag=0;

}






// *******************************************************************
// 						BLUETOOTH RECEIVER
// *******************************************************************

void MessageHandler(Problem * p){

	char spaces_msg[100];
	for (int i=0; i<100; i++){
		spaces_msg[i] = ' ';
	}

	// NAME
	strcpy(p->name,"           ");
	for(int i=0; i<11; i++){
		p->name[i] = buffer[i+2];
	}

	// GRADE
	strcpy(p->grade, "          ");
	for(int i=0; i<3; i++){
		p->grade[i] = buffer[100+i];
	}

	// N OF HOLDS
	p->moveSize = buffer[200]*10+buffer[201];

	// LETTERS OF EACH HOLD
	strcpy(p->moveLetters, spaces_msg);
	for(int i=0; i<p->moveSize*2; i++){
		p->moveLetters[i] = buffer[300+i];
	}

	// NUMBERS OF EACH HOLD
	strcpy(p->moveNumbers, spaces_msg);
	for(int i=0; i<100; i++){
		p->moveNumbers[i] = buffer[400+i];
	}

	// TYPE OF EACH HOLD
	strcpy(p->startFinish, spaces_msg);
	for(int i=0; i<p->moveSize*2; i++){
		p->startFinish[i] = buffer[500+i];
	}


}


