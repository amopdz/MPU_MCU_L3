/*
 * input_reading_new.c
 *
 *  Created on: Oct 23, 2023
 *      Author: hhaof
 */

#include "main.h"
#include "input_reading.h"

#define DURATION_FOR_AUTO_INCREASING	   1000/TIMER_INTERRUPT_PERIOD
#define BUTTON_IS_PRESSED                  GPIO_PIN_RESET
#define BUTTON_IS_RELEASED                 GPIO_PIN_SET

GPIO_PinState debounceButtonBuffer1[N0_OF_BUTTONS]={BUTTON_IS_RELEASED,BUTTON_IS_RELEASED,BUTTON_IS_RELEASED};
GPIO_PinState debounceButtonBuffer2[N0_OF_BUTTONS]={BUTTON_IS_RELEASED,BUTTON_IS_RELEASED,BUTTON_IS_RELEASED};
GPIO_PinState debounceButtonBuffer3[N0_OF_BUTTONS]={BUTTON_IS_RELEASED,BUTTON_IS_RELEASED,BUTTON_IS_RELEASED};
GPIO_PinState debounceButtonBuffer4[N0_OF_BUTTONS]={BUTTON_IS_RELEASED,BUTTON_IS_RELEASED,BUTTON_IS_RELEASED};

void button_reading(){
	for(uint8_t i = 0; i < N0_OF_BUTTONS; i ++){
		debounceButtonBuffer4[i] = debounceButtonBuffer3[i];
		debounceButtonBuffer3[i] = debounceButtonBuffer2[i];
		debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
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
	}
}

uint8_t is_button_pressed(uint8_t index){
	if(index >= N0_OF_BUTTONS) return 0;
	return (debounceButtonBuffer4[index]==BUTTON_IS_PRESSED);
}
