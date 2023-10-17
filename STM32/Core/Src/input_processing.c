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
const int MAX7SEG=4;
uint8_t traff1Out=0,
		traff2Out=0,
		enOut=0,
		segOut=0,
		led_buffer[]={0,0,0,0},
		led7SegCounter=MAX7SEG-1;

enum {RG,RY,GR,YR} traffCond;
uint8_t light1Time,light2Time,counter;

enum ButtonState{BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND, BUTTON_PRESSED_MORE_THAN_3_SECONDS}
	 modeButtonState = BUTTON_RELEASED,
	 modifyButtonState = BUTTON_RELEASED,
	 setButtonState = BUTTON_RELEASED;

enum {NORMAL, MOD_RED, MOD_YELLOW, MOD_GREEN} mode=NORMAL;

uint8_t durationRed = 5,
		durationRedWaiting,
		durationYellow = 2,
		durationYellowWaiting,
		durationGreen = 3,
		durationGreenWaiting,
		normalInit=0;

uint8_t intervalCounter=0;
const uint8_t maxInterval = 2;

void display7SEG(uint8_t number){
	if(number>=0 && number<10)segOut=digitMask[number];
}

void update7SEG(int index){
	display7SEG(led_buffer[index]);
	switch(index){
		case 0:
			enOut=0xE;
			break;
		case 1:
			enOut=0xD;
			break;
		case 2:
			enOut=0xB;
			break;
		case 3:
			enOut=0x7;
			break;
		default:
			break;
	}
}

void update7SEGMain(){
	led7SegCounter=led7SegCounter>=MAX7SEG-1?0:led7SegCounter+1;
	update7SEG(led7SegCounter);
}

void fsm_for_mode_button(){
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
						if(durationRed>durationYellow+durationGreen){
							durationGreen=durationRed-durationYellow;
						}else if(durationRed<durationYellow+durationGreen){
							if(durationGreen+durationYellow>99){
								durationRed=99;
								durationGreen=96;
								durationYellow=3;
							}else durationRed=durationGreen+durationYellow;
						}
						break;
				}
				counter=0;
				durationRedWaiting = durationRed;
				durationYellowWaiting = durationYellow;
				durationGreenWaiting = durationGreen;
			}
			break;
	}
}

uint8_t done_interval(){
	if(intervalCounter>0){
		intervalCounter--;
		return 0;
	}
	intervalCounter=maxInterval;
	return 1;
}

void fsm_for_modify_button(){
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
					intervalCounter=maxInterval;
				}
			}
			break;
		case BUTTON_PRESSED_MORE_THAN_3_SECONDS:
			if(!is_button_pressed(1)){
				modifyButtonState = BUTTON_RELEASED;
			}else if(done_interval()==1){
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
			}
			break;
	}
	switch(mode){
		case MOD_RED:
			led_buffer[0]=durationRedWaiting/10;
			led_buffer[1]=durationRedWaiting%10;
			led_buffer[2]=durationRedWaiting/10;
			led_buffer[3]=durationRedWaiting%10;
			break;
		case MOD_YELLOW:
			led_buffer[0]=durationYellowWaiting/10;
			led_buffer[1]=durationYellowWaiting%10;
			led_buffer[2]=durationYellowWaiting/10;
			led_buffer[3]=durationYellowWaiting%10;
			break;
		case MOD_GREEN:
			led_buffer[0]=durationGreenWaiting/10;
			led_buffer[1]=durationGreenWaiting%10;
			led_buffer[2]=durationGreenWaiting/10;
			led_buffer[3]=durationGreenWaiting%10;
			break;
	}
}

void fsm_for_set_button(){
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
				durationRed = durationRedWaiting;
				durationYellow = durationYellowWaiting;
				durationGreen = durationGreenWaiting;
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
			mode_mod_red();
			break;
		case MOD_YELLOW:
			mode_mod_yellow();
			break;
		case MOD_GREEN:
			mode_mod_green();
			break;
	}
}

void mode_normal(){
	if(normalInit==0){
		traff1Out=6;
		traff2Out=3;
		traffCond=RG;
		light1Time=durationRed;
		light2Time=durationGreen;
		normalInit=1;
		counter=4*MAIN_MODE_PERIOD/TIMER_INTERRUPT_PERIOD;
	}else{
		if(counter>0)counter--;
		else{
			counter=4*MAIN_MODE_PERIOD/TIMER_INTERRUPT_PERIOD;
			switch(traffCond){
				case RG:
					if(light1Time>0&&light2Time>0){
						light1Time--;
						light2Time--;
					}else{
						traff1Out=6;
						traff2Out=5;
						light2Time=durationYellow;
						traffCond=RY;
					}
					break;
				case RY:
					if(light1Time>0&&light2Time>0){
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
					if(light1Time>0&&light2Time>0){
						light1Time--;
						light2Time--;
					}else{
						traff1Out=5;
						traff2Out=6;
						light1Time=durationYellow;
						traffCond=YR;
					}
					break;
				case YR:
					if(light1Time>0&&light2Time>0){
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
	led_buffer[0]=light1Time/10;
	led_buffer[1]=light1Time%10;
	led_buffer[2]=light2Time/10;
	led_buffer[3]=light2Time%10;
}

void mode_mod_red(){
	if(counter>0)counter--;
	else{
		counter=MAIN_MODE_PERIOD/TIMER_INTERRUPT_PERIOD;
		if(normalInit==0){
			traff1Out=6;
			traff2Out=6;
			normalInit=1;
		}else{
			normalInit=0;
			traff1Out=7;
			traff2Out=7;
		}
	}
}

void mode_mod_yellow(){
	if(counter>0)counter--;
	else{
		counter=MAIN_MODE_PERIOD/TIMER_INTERRUPT_PERIOD;
		if(normalInit==0){
			traff1Out=5;
			traff2Out=5;
			normalInit=1;
		}else{
			normalInit=0;
			traff1Out=7;
			traff2Out=7;
		}
	}
}

void mode_mod_green(){
	if(counter>0)counter--;
	else{
		counter=MAIN_MODE_PERIOD/TIMER_INTERRUPT_PERIOD;
		if(normalInit==0){
			traff1Out=3;
			traff2Out=3;
			normalInit=1;
		}else{
			normalInit=0;
			traff1Out=7;
			traff2Out=7;
		}
	}
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
