#include "LED.h"
#include "gpio.h"
#include "tim.h"

#define LED_NUM 120


void LED_light(uint8_t *array){

	for(int i=0; i<sizeof(array); i++){
		if(array[i]==0){
			TIM2->CCR1 = 3; 							// update the pulse value of timer 2
			HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
		}else{
			TIM2->CCR1 = 67; 							// update the pulse value of timer 2
			HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
		}
	}


}


void LED_setColor(uint16_t LEDnumber, uint8_t REDcolor, uint8_t GREENcolor, uint8_t BLUEcolor, uint8_t *LEDbuffer){

	uint8_t tmpBuf[24];

	for(int i=0; i<7; i++){
		tmpBuf[i] = (REDcolor>>i) & 0x01;
	}

	for(int i=0; i<7; i++){
		tmpBuf[8+i] = (GREENcolor>>i) & 0x01;
	}

	for(int i=0; i<7; i++){
		tmpBuf[16+i] = (BLUEcolor>>i) & 0x01;
	}

	for(int i=0; i<24; i++){
		LEDbuffer[i] = tmpBuf[i];
	}

}





void LED_setAllBlack(uint8_t *LEDbuffer){

	for(int i=0; i<LED_NUM*24; i++){
		LEDbuffer[i]=0;
	}

}


void LED_setAllWhite(uint8_t *LEDbuffer){

	for(int i=0; i<LED_NUM*24; i++){
		LEDbuffer[i]=1;
	}
}
