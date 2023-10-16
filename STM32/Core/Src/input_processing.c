/*
 * input_processing.c
 *
 *  Created on: Oct 13, 2023
 *      Author: hh.aof
 */

#include "main.h"
#include "input_reading.h"
#include "input_processing.h"

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
const int MAX_LED=4;
int led_buffer[4];
uint8_t traff1Out=0,traff2Out=0,enOut=0,segOut=0;

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

enum ButtonState{BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND, BUTTON_PRESSED_MORE_THAN_3_SECONDS}
	 modeButtonState = BUTTON_RELEASED,
	 modifyButtonState = BUTTON_RELEASED,
	 setButtonState = BUTTON_RELEASED;

enum {NORMAL, MOD_RED, MOD_YELLOW, MOD_GREEN} mode=NORMAL;

int durationRed = 5,
	durationRedWaiting,
	durationYellow = 2,
	durationYellowWaiting,
	durationGreen = 4,
	durationGreenWaiting;

int normalInit=0;

void fsm_for_mode_button(void){
	switch(modeButtonState){
		case BUTTON_RELEASED:
			if(is_button_pressed(0)){
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
						normalInit=0;
						mode=MOD_RED;
						break;
					case MOD_RED:
						normalInit=0;
						mode=MOD_YELLOW;
						break;
					case MOD_YELLOW:
						normalInit=0;
						mode=MOD_GREEN;
						break;
					case MOD_GREEN:
						normalInit=0;
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
			if(!is_button_pressed(1)){
				modifyButtonState = BUTTON_RELEASED;
			} else {
				if(is_button_pressed_1s(1)){
					modifyButtonState = BUTTON_PRESSED_MORE_THAN_1_SECOND;
				}
			}
			break;
		case BUTTON_PRESSED_MORE_THAN_1_SECOND:
			if(!is_button_pressed(1)){
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
				if(is_button_pressed_3s(1)){
					modifyButtonState = BUTTON_PRESSED_MORE_THAN_3_SECONDS;
				}
			}
			break;
		case BUTTON_PRESSED_MORE_THAN_3_SECONDS:
			if(!is_button_pressed(1)){
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
			if(is_button_pressed(2)){
				setButtonState = BUTTON_PRESSED;
			}
			break;
		case BUTTON_PRESSED:
			if(!is_button_pressed(2)){
				setButtonState = BUTTON_RELEASED;
			} else {
				if(is_button_pressed_1s(2)){
					setButtonState = BUTTON_PRESSED_MORE_THAN_1_SECOND;
				}
			}
			break;
		case BUTTON_PRESSED_MORE_THAN_1_SECOND:
			if(!is_button_pressed(2)){
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

void mainMode(){
	switch(mode){
		case NORMAL:
			mode_normal();
			break;
		case MOD_RED:
			normalInit=0;
			mode_mod_red();
			break;
		case MOD_YELLOW:
			normalInit=0;
			mode_mod_yellow();
			break;
		case MOD_GREEN:
			normalInit=0;
			mode_mod_green();
			break;
	}
}

enum {RG,RY,GR,YR} traffCond;
uint8_t light1Time,light2Time,counter;

void mode_normal(){
	if(normalInit==0){
		traff1Out=6;
		traff2Out=3;
		traffCond=RG;
		light1Time=durationRed;
		light2Time=durationGreen;
		normalInit=1;
		counter=MAIN_MODE_PERIOD/TIMER_INTERRUPT_PERIOD;
	}else{
		if(counter>0)counter--;
		else{
			counter=MAIN_MODE_PERIOD/TIMER_INTERRUPT_PERIOD;
			switch(traffCond){
				case RG:
					if(light2Time>0){
						light2Time--;
					}else{
						traff1Out=6;
						traff2Out=5;
						light2Time=durationYellow;
						traffCond=RY;
					}
					light1Time--;
					break;
				case RY:
					if(light1Time>0){
						light1Time--;
						light2Time--;
					}else{
						traff1Out=3;
						traff2Out=6;
						light1Time=durationGreen;
						light2Time=durationRed;
						traffCond=GR;
					}
					break;
				case GR:
					if(light1Time>0){
						light1Time--;
					}else{
						traff1Out=5;
						traff2Out=6;
						light1Time=durationYellow;
						traffCond=YR;
					}
					light2Time--;
					break;
				case YR:
					if(light1Time>0){
						light1Time--;
						light2Time--;
					}else{
						traff1Out=6;
						traff2Out=3;
						light2Time=durationGreen;
						light1Time=durationRed;
						traffCond=RG;
					}
					break;
			}
		}
	}
	led_buffer[0]=light1Time%10;
	led_buffer[1]=light1Time/10;
	led_buffer[2]=light2Time%10;
	led_buffer[3]=light2Time/10;
}

void mode_mod_red(){
	if(normalInit==0){
		traff1Out=6;
		traff2Out=6;
	}else{
		traff1Out=7;
		traff2Out=7;
	}
}

void mode_mod_yellow(){
	traff1Out=5;
	traff2Out=5;
}

void mode_mod_green(){
	traff1Out=3;
	traff2Out=3;
}

void updateSEGBuffer(){
	switch(mode){
		case NORMAL:
			break;
		case MOD_RED:
			break;
		case MOD_YELLOW:
			break;
		case MOD_GREEN:
			break;
	}
}

void displayAll(){
	GPIOA->ODR=(traff1Out<<12)|(traff2Out<<4);
	GPIOB->ODR=(enOut<<8)|(segOut);
}
