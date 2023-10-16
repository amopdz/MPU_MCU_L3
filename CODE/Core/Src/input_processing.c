/*
 * input_processing.c
 *
 *  Created on: Oct 13, 2023
 *      Author: hh.aof
 */

#include "main.h"
#include "input_reading.h"

enum ButtonState{BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND, BUTTON_PRESSED_MORE_THAN_3_SECONDS} ;
enum ButtonState modeButtonState = BUTTON_RELEASED,
				 modifyButtonState = BUTTON_RELEASED,
				 setButtonState = BUTTON_RELEASED;

enum{NORMAL, MOD_RED, MOD_YELLOW, MOD_GREEN} mode=NORMAL;
int durationRed = 5,
	durationRedWaiting = durationRed,
	durationYellow = 2,
	durationYellowWaiting = durationYellow,
	durationGreen = 4,
	durationGreenWaiting = durationGreen;

void fsm_for_mode_button(void){
	switch(modeButtonState){
		case BUTTON_RELEASED:
			if(is_button_pressed(1)){
				modeButtonState = BUTTON_PRESSED;
			}
			break;
		case BUTTON_PRESSED:
			if(!is_button_pressed(0)){
				modeButtonState = BUTTON_RELEASED;
			} else {
				if(is_button_pressed_1s(0)){
					modeButtonState = BUTTON_PRESSED_MORE_THAN_1_SECOND;
				}
			}
			break;
		case BUTTON_PRESSED_MORE_THAN_1_SECOND:
			if(!is_button_pressed(0)){
				modeButtonState = BUTTON_RELEASED;
				switch(mode){
					case NORMAL:
						mode=MOD_RED;
						break;
					case MOD_RED:
						mode=MOD_YELLOW;
						break;
					case MOD_YELLOW:
						mode=MOD_GREEN;
						break;
					case MOD_GREEN:
						mode=NORMAL;
						break;
				}
				durationRedWaiting = durationRed;
				durationYellowWaiting = durationYellow;
				durationGreenWaiting = durationGreen;
			}
			break;
	}
}

void fsm_for_modify_button(void){
	switch(modifyButtonState){
		case BUTTON_RELEASED:
			if(is_button_pressed(1)){
				modifyButtonState = BUTTON_PRESSED;
			}
			break;
		case BUTTON_PRESSED:
			if(!is_button_pressed(0)){
				modifyButtonState = BUTTON_RELEASED;
			} else {
				if(is_button_pressed_1s(0)){
					modifyButtonState = BUTTON_PRESSED_MORE_THAN_1_SECOND;
				}
			}
			break;
		case BUTTON_PRESSED_MORE_THAN_1_SECOND:
			if(!is_button_pressed(0)){
				modifyButtonState = BUTTON_RELEASED;
				switch(mode){
					case MOD_RED:
						durationRedWaiting = durationRedWaiting >= 99 ? 1 : durationRedWaiting + 1;
						break;
					case MOD_YELLOW:
						durationYellowWaiting = durationYellowWaiting >= 99 ? 1 : durationYellowWaiting + 1;
						break;
					case MOD_GREEN:
						durationGreenWaiting = durationGreenWaiting >= 99 ? 1 : durationGreenWaiting + 1;
						break;
				}
			} else {
				if(is_button_pressed_3s(0)){
					modifyButtonState = BUTTON_PRESSED_MORE_THAN_3_SECONDS;
				}
			break;
		case BUTTON_PRESSED_MORE_THAN_3_SECONDS:
			if(!is_button_pressed(0)){
				modifyButtonState = BUTTON_RELEASED;
			}
			switch(mode){
				case MOD_RED:
					durationRedWaiting = durationRedWaiting >= 99 ? 1 : durationRedWaiting + 1;
					break;
				case MOD_YELLOW:
					durationYellowWaiting = durationYellowWaiting >= 99 ? 1 : durationYellowWaiting + 1;
					break;
				case MOD_GREEN:
					durationGreenWaiting = durationGreenWaiting >= 99 ? 1 : durationGreenWaiting + 1;
					break;
			}
			break;
	}
}

void fsm_for_set_button(void){
	switch(setButtonState){
		case BUTTON_RELEASED:
			if(is_button_pressed(1)){
				setButtonState = BUTTON_PRESSED;
			}
			break;
		case BUTTON_PRESSED:
			if(!is_button_pressed(0)){
				setButtonState = BUTTON_RELEASED;
			} else {
				if(is_button_pressed_1s(0)){
					setButtonState = BUTTON_PRESSED_MORE_THAN_1_SECOND;
				}
			}
			break;
		case BUTTON_PRESSED_MORE_THAN_1_SECOND:
			if(!is_button_pressed(0)){
				setButtonState = BUTTON_RELEASED;
				switch(mode){
					case MOD_RED:
						durationRed = durationRedWaiting;
						break;
					case MOD_YELLOW:
						durationYellow = durationYellowWaiting;
						break;
					case MOD_GREEN:
						durationGreen = durationGreenWaiting;
						break;
				}
			}
			break;
	}
}

void mode_normal(){

}

void mode_mod_red(){

}

void mode_mod_yellow(){

}

void mode_mod_green(){

}

const uint8_t digitMask[] = {	//fetch bit by bit, small endian, first position -> final pin e.g Pin6
		0x40,//0
		0x79,//1
		0x24,//2
		0x30,//3
		0x19,//4
		0x12,//5
		0x02,//6
		0x78,//7
		0x00,//8
		0x10,//9
		0x7F//10 e.g blank
};
int led_buffer[]={1,2,3,4};
uint8_t traff1Out,traff2Out,swOut,enOut,segOut;

void display7SEG(uint8_t number){
	if(number>=0 && number<10)segOut=digitMask[number];
}

void update7SEG(int index){
	display7SEG(led_buffer[index]);
	switch(index){
		case 0:
			HAL_GPIO_TogglePin(EN3_GPIO_Port, EN3_Pin);
			HAL_GPIO_TogglePin(EN0_GPIO_Port, EN0_Pin);
			break;
		case 1:
			HAL_GPIO_TogglePin(EN0_GPIO_Port, EN0_Pin);
			HAL_GPIO_TogglePin(EN1_GPIO_Port, EN1_Pin);
			break;
		case 2:
			HAL_GPIO_TogglePin(EN1_GPIO_Port, EN1_Pin);
			HAL_GPIO_TogglePin(EN2_GPIO_Port, EN2_Pin);
			break;
		case 3:
			HAL_GPIO_TogglePin(EN2_GPIO_Port, EN2_Pin);
			HAL_GPIO_TogglePin(EN3_GPIO_Port, EN3_Pin);
			break;
		default:
			break;
	}
}

void displayAll(){
	GPIOA->ODR=(enOut<<12)||(swOut<<8)||(traff2Out<<4)||(traff1Out);
	GPIOB->ODR=segOut;
}
