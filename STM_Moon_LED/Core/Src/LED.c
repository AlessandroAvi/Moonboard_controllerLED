#include "LED.h"
#include "gpio.h"
#include "tim.h"
#include "problems.h"
#include <stdio.h>
#include <ctype.h>

#define LED_NUM 198
#define ROW_NUM 18
#define COL_NUM 11

uint8_t __attribute__((section(".upper.rodata"))) problem_arr[ROW_NUM*COL_NUM*3] = {0};




// *******************************************************************
// 					MANIPULATION OF THE LED ARRAY
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

/*
void problem_genArray(uint32_t id){



	uint16_t movesNum = 0;
	uint16_t LedPos = 0;

	problem_search(&problem, id);

	// find the number of holds in the problem
	while(problem.move_int[movesNum]  ){   // trovare metodo per definire lunghezza
		movesNum++;
	}

	// put to 0 all the colors for each LED
	LED_setAllWhite();

	for(int i=0; i< movesNum; i++){

		// transform letter and number in the led position
		LedPos = LED_findPos(problem.move_char[i], problem.move_int[i]);

		// light the led depending if it's start, top or else
		if(i<=1 && problem.move_st[i]==true){
			problem_arr[LedPos*3]   = 0;
			problem_arr[LedPos*3+1] = 0;
			problem_arr[LedPos*3+2] = 255;
		}else if(i>1 && problem.move_st[i]==true){
			problem_arr[LedPos*3]   = 255;
			problem_arr[LedPos*3+1] = 0;
			problem_arr[LedPos*3+2] = 0;
		}else{
			problem_arr[LedPos*3]   = 255;
			problem_arr[LedPos*3+1] = 255;
			problem_arr[LedPos*3+2] = 255;
		}

	}

}
*/


void LED_setAllBlack(uint8_t *LEDbuffer){

	for(int i=0; i<ROW_NUM*COL_NUM*3; i++){
		//prob_arr[i]=0;
	}

}


void LED_setAllWhite(uint8_t *LEDbuffer){

	for(int i=0; i<ROW_NUM*COL_NUM*3; i++){
		//prob_arr[i]=255;
	}


}





// *******************************************************************
// 					GENERATION OF PWM
// *******************************************************************



void LED_light(){

	for(int k=0; k<ROW_NUM*COL_NUM*3; k++){




	}

}
