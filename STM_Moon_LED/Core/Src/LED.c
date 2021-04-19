#include "LED.h"
#include "gpio.h"
#include "tim.h"
#include <stdio.h>
#include <ctype.h>


uint16_t pwmData[(24*MAX_LED)+50];
uint8_t LED_Data[ROW_NUM*COL_NUM][4];
uint8_t LED_Mod[ROW_NUM*COL_NUM][4];
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


void problem_genArray(ProblemInfo *problem, uint32_t id){

	uint16_t movesNum = 0;
	uint16_t LedPos = 0;

	// fill the struct with the info of the boulder searched
	problem_search(problem, id);

	// find the number of holds in the problem
	while(problem->move_int[movesNum] !=0){
		movesNum++;
	}

	// put to 0 all the colors for each LED
	LED_setAllBlack();

	// for each move define the color of the corresponding LED
	for(int i=0; i<movesNum; i++){

		// transform letter and number in the led position
		LedPos = LED_findPos(problem->move_char[i], problem->move_int[i]);

		// light the led depending if it's start, top or else
		if(i<=1 && problem->move_st[i]==true){		// start, color green
			LED_Data[LedPos][1] = 0;
			LED_Data[LedPos][2] = 0;
			LED_Data[LedPos][3] = 255;
		}else if(i>1 && problem->move_st[i]==true){	// top, color red
			LED_Data[LedPos][1] = 255;
			LED_Data[LedPos][2] = 0;
			LED_Data[LedPos][3] = 0;
		}else{										// middle, color white
			LED_Data[LedPos][1] = 255;
			LED_Data[LedPos][2] = 255;
			LED_Data[LedPos][3] = 255;
		}

	}

}



void LED_setAllBlack(uint8_t *LEDbuffer){

	for(int i=0; i<ROW_NUM*COL_NUM; i++){
		LED_Data[i][1]=0;
		LED_Data[i][2]=0;
		LED_Data[i][3]=0;
	}

}



void LED_setAllWhite(uint8_t *LEDbuffer){

	for(int i=0; i<ROW_NUM*COL_NUM; i++){
		LED_Data[i][1]=255;
		LED_Data[i][2]=255;
		LED_Data[i][3]=255;
	}


}


// *******************************************************************
// 						GENERATION OF PWM
// *******************************************************************



// sets the color of the LED
void LED_setColor(int LEDnum, int red, int green, int blue){
	LED_Data[LEDnum][0] = LEDnum;
	LED_Data[LEDnum][1] = green;
	LED_Data[LEDnum][2] = red;
	LED_Data[LEDnum][3] = blue;
}



void LED_setBrightness(int brightness){

#if USE_BRIGHTNESS
	if(brightness > 45) brightness = 45;

	for(int i=0; i<MAX_LED; i++){
		LED_Mod[i][0] = LED_Data[i][0];
		for(int j=0; j<4; j++){
			float angle = 90-brightness;
			angle = angle*3.14/180;
			LED_Mod[i][j] = (LED_Data[i][j])/(tan(angle));
		}
	}
#endif

}



void WS2811_Send(void){

	uint32_t indx = 0;
	uint32_t color;

	// read from the color matrix each RGB color for each LED
	for(int i=0; i<MAX_LED; i++){
		color = ((LED_Mod[i][1]<<16) | (LED_Mod[i][2]<<8) | LED_Mod[i][3]);

		// check each bit of the color number and create corresponfing PWM
		for(int i=23; i>=0; i--){
			if(color&(1<<i)){
				pwmData[indx]=60;
			}else{
				pwmData[indx]=30;
			}
			indx++;
		}
	}

	// wait 50 PWM pulses as a RESET after lighting all LED
	for(int i=0; i<50; i++){
		pwmData[indx] = 0;
		indx++;
	}

	// start the transmission of the data just created
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)pwmData, indx);
	while(!datasentflag){};
	datasentflag=0;

}



// code for checking just 1	LED
void send_DMA(int green, int red, int blue){

	uint32_t data = (green <<16) | (red <<8) | blue;

	for(int i=23; i>=0; i--){

		if(data&(1<<i)) pwmData[i] = 60;

		else pwmData[i] = 30;
	}

	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)pwmData, 24);
}



// *******************************************************************
// 					DEFINITION OF LIST OF PROBLEMS
// *******************************************************************


void problem_search(ProblemInfo *problem, uint32_t id){



}

