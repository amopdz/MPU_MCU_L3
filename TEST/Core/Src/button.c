/*
 * input_reading.c
 *
 *  Created on: Oct 13, 2023
 *      Author: hh.aof
 */

#include "main.h"

#define N0_OF_BUTTONS 				       3

#define DURATION_FOR_AUTO_INCREASING	   100
#define BUTTON_IS_PRESSED                  GPIO_PIN_RESET
#define BUTTON_IS_RELEASED                 GPIO_PIN_SET

GPIO_PinState debounceButtonBuffer1[N0_OF_BUTTONS]={BUTTON_IS_RELEASED,BUTTON_IS_RELEASED,BUTTON_IS_RELEASED};
GPIO_PinState debounceButtonBuffer2[N0_OF_BUTTONS]={BUTTON_IS_RELEASED,BUTTON_IS_RELEASED,BUTTON_IS_RELEASED};
GPIO_PinState debounceButtonBuffer3[N0_OF_BUTTONS]={BUTTON_IS_RELEASED,BUTTON_IS_RELEASED,BUTTON_IS_RELEASED};
GPIO_PinState debounceButtonBuffer4[N0_OF_BUTTONS]={BUTTON_IS_RELEASED,BUTTON_IS_RELEASED,BUTTON_IS_RELEASED};

uint8_t flagForButtonPress1s[N0_OF_BUTTONS];
uint8_t flagForButtonPress3s[N0_OF_BUTTONS];

uint16_t counterForButtonPress[N0_OF_BUTTONS];

void button_reading(){
	for(uint8_t i = 0; i < N0_OF_BUTTONS; i ++){
		debounceButtonBuffer3[i] =debounceButtonBuffer2[i];
		debounceButtonBuffer2[i] =debounceButtonBuffer1[i];
		switch(i){
			case 0:
				debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(SW0_GPIO_Port, SW0_Pin);
				break;
			case 1:
				debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin);
				break;
			case 2:
				debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin);
				break;
		}
		if((debounceButtonBuffer1[i] == debounceButtonBuffer2[i])&&(debounceButtonBuffer2[i] == debounceButtonBuffer3[i])){
			if(debounceButtonBuffer3[i]!=debounceButtonBuffer4[i]){
				debounceButtonBuffer4[i]=debounceButtonBuffer3[i];
				if((debounceButtonBuffer4[i]==BUTTON_IS_PRESSED)&&(flagForButtonPress1s[i]==0)&&(flagForButtonPress3s[i]==0)){
					counterForButtonPress[i] = DURATION_FOR_AUTO_INCREASING;
					HAL_GPIO_TogglePin(RED_GPIO_Port, RED_Pin);
				}
			}else{
				if((debounceButtonBuffer4[i]==BUTTON_IS_PRESSED)&&((flagForButtonPress1s[i]==0)||(flagForButtonPress3s[i]==0))){
					if(counterForButtonPress[i]>0){
						counterForButtonPress[i]--;
						if(counterForButtonPress[i]==0){
							if(flagForButtonPress1s[i]==0){
								counterForButtonPress[i] = 2*DURATION_FOR_AUTO_INCREASING;
								flagForButtonPress1s[i]=1;
							}else flagForButtonPress3s[i]=1;
							HAL_GPIO_TogglePin(RED_GPIO_Port, RED_Pin);
						}
					}
				}
			}
		}
		if(debounceButtonBuffer4[i]==BUTTON_IS_RELEASED){
			flagForButtonPress1s[i]=0;
			flagForButtonPress3s[i]=0;
		}
	}
}

uint8_t is_button_pressed(uint8_t index){
	if(index >= N0_OF_BUTTONS) return 0;
	return (debounceButtonBuffer4[index]==BUTTON_IS_PRESSED);
}

uint8_t is_button_pressed_1s(uint8_t index){
	if(index >= N0_OF_BUTTONS) return 0;
	return (flagForButtonPress1s[index] == 1);
}

uint8_t is_button_pressed_3s(uint8_t index){
	if(index >= N0_OF_BUTTONS) return 0;
	return (flagForButtonPress3s[index] == 1);
}
