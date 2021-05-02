#include "LED.h"
#include "gpio.h"
#include "tim.h"
#include <stdio.h>
#include <ctype.h>

bool TIM_INT = false;
uint32_t pwmData[24*MAX_LED+80];
uint8_t LED_Data[MAX_LED][4];
int datasentflag = 0;




// *******************************************************************
// 					MANIPULATION OF THE LED MATRIX
// *******************************************************************


uint8_t LED_findPos(char letter, uint8_t number){

	uint8_t position;


	switch(letter){
	case('A'):
		position = number;
		break;
	case('B'):
		position = (18*2)+1 -number;
		break;
	case('C'):
		position = (18*2)+number;
		break;
	case('D'):
		position = (18*4)+1 -number;
		break;
	case('E'):
		position = (18*4)+number;
		break;
	case('F'):
		position = (18*6)+1 -number;
		break;
	case('G'):
		position = (18*6)+number;
		break;
	case('H'):
		position = (18*8)+1 -number;
		break;
	case('I'):
		position = (18*8)+number;
		break;
	case('J'):
		position = (18*10)+1 -number;
		break;
	case('K'):
		position = (18*10)+number;
		break;

	}

	return position;
}


void problem_genArray(struct Problem *p, uint32_t id){

	uint16_t LedPos = 0;

	// put to 0 all the colors for each LED
	LED_setAllBlack();

	// create 3 arrays that contain letters and numbers and type
	char lettArr[p->moveSize];
	uint8_t numArr[p->moveSize];
	uint8_t typeArr[p->moveSize];

	for(int j=0; j<p->moveSize; j++){
		lettArr[j] = p->moveLetters[j*2];
		typeArr[j] = p->startFinish[j*2];

		//while(p->moveNumbers[i] != ','){

			//numArr[i]
	//	}
	}

	// for each move define the color of the corresponding LED
	for(int i=0; i<p->moveSize; i++){

		// transform letter and number in the led position
		LedPos = LED_findPos(lettArr[i], numArr[i]);

		// light the led depending if it's start, top or else
		if(typeArr[i]=='s'){			// START, color green
			LED_Data[LedPos][1] = 0;
			LED_Data[LedPos][2] = 0;
			LED_Data[LedPos][3] = 255;
		}else if(typeArr[i]=='e'){		// TOP, color red
			LED_Data[LedPos][1] = 255;
			LED_Data[LedPos][2] = 0;
			LED_Data[LedPos][3] = 0;
		}else if(typeArr[i]=='d'){		// DEFAULT, color white
			LED_Data[LedPos][1] = 255;
			LED_Data[LedPos][2] = 255;
			LED_Data[LedPos][3] = 255;
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
		LED_Data[i][1]=rand();
		LED_Data[i][2]=rand();
		LED_Data[i][3]=rand();
	}


}


// *******************************************************************
// 						GENERATION OF PWM
// *******************************************************************



// sets the color of one specific LED
void LED_setColor(int LEDnum, int red, int green, int blue){
	LED_Data[LEDnum][0] = LEDnum;
	LED_Data[LEDnum][1] = red;
	LED_Data[LEDnum][2] = green;
	LED_Data[LEDnum][3] = blue;
}







void WS2811_Send(void){

	uint16_t indx = 0;
	uint32_t color;

	// wait 50 PWM pulses as a RESET before lighting all LED
	for(int i=0; i<40; i++){
		pwmData[indx] = 0;
		indx++;
	}

	// read from the color matrix each RGB color for each LED
	for(int i=0; i<MAX_LED; i++){
		color = ((LED_Data[i][1]<<16) | (LED_Data[i][2]<<8) | LED_Data[i][3]);

		// check each bit of the color number and create corresponfing PWM
		for(int i=23; i>=0; i--){
			if(color & (1<<i)){
				pwmData[indx]=43;
			}else{
				pwmData[indx]=18;
			}
			indx++;
		}
	}

	// wait 50 PWM pulses as a RESET after lighting all LED
	for(int i=0; i<40; i++){
		pwmData[indx] = 0;
		indx++;
	}

	// start the transmission of the data just created
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)pwmData, indx);


	while(!datasentflag){};
	datasentflag=0;

}



// code for checking just the first LED
void send_DMA(int green, int red, int blue){

	uint32_t data = (green <<16) | (red <<8) | blue;

	for(int i=23; i>=0; i--){

		if(data & (1<<i)) pwmData[i] = 60;

		else pwmData[i] = 30;
	}

	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)pwmData, 3);
}

