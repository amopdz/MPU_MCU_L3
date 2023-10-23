/*
 * input_processing_new.c
 *
 *  Created on: Oct 23, 2023
 *      Author: hhaof
 */

#include "main.h"
#include "input_processing.h"
#include "input_reading.h"

//Traffic light states
#define RG 0	//RED & GREEN
#define RY 1	//RED & YELLOW
#define GR 2	//GREEN & RED
#define YR 3	//YELLOW & RED

uint8_t light1Time,	//Time for the two 7 segment LEDs
        light2Time,
		durationRed = 5,	//Duration for each of the lights & their duration waiting to be set
		durationRedWaiting,
		durationYellow = 2,
		durationYellowWaiting,
		durationGreen = 3,
		durationGreenWaiting,
		led_buffer[MAX7SEG],	//7 segments LED buffer array
		buttonFlag[N0_OF_BUTTONS],	//Flags for buttons
		traffCond,	//FSM for the traffic lights
		traff1Out,	//Buffer for 2 traffic lights
		traff2Out;

//FSM for buttons
void fsm_automatic(){
	switch(mode){
		case INIT:
			durationRedWaiting=durationRed;
			durationYellowWaiting=durationYellow;
			durationGreenWaiting=durationGreen;
			if(durationRed>durationYellow+durationGreen){
				durationGreen=durationRed-durationYellow;
			}else if(durationRed<durationYellow+durationGreen){
				if(durationGreen+durationYellow>99){
					durationRed=99;
					durationGreen=96;
					durationYellow=3;
				}else durationRed=durationGreen+durationYellow;
			}
			light1Time=durationRed;
			light2Time=durationGreen;
			traffCond=RG;
			led_buffer[0]=light1Time/10;
			led_buffer[1]=light1Time%10;
			led_buffer[2]=light2Time/10;
			led_buffer[3]=light2Time%10;
			buttonFlag[0]=0;
			buttonFlag[1]=0;
			buttonFlag[2]=0;
			mode=NORMAL;
			break;
		case NORMAL:
			if(is_button_pressed(0))buttonFlag[0]=1;
			if((!is_button_pressed(0))&&(buttonFlag[0])){
				mode=MOD_RED;
				traff1Out=6;
				traff2Out=6;
				buttonFlag[0]=0;
				buttonFlag[1]=0;
				buttonFlag[2]=0;
			}
			break;
		case MOD_RED:
			if(is_button_pressed(0))buttonFlag[0]=1;
			if((!is_button_pressed(0))&&(buttonFlag[0])){
				mode=MOD_YELLOW;
				traff1Out=3;
				traff2Out=3;
				buttonFlag[0]=0;
				buttonFlag[1]=0;
				buttonFlag[2]=0;
			}
			if(is_button_pressed(1))buttonFlag[1]=1;
			if((!is_button_pressed(1)) && (buttonFlag[1])){
				durationRedWaiting = durationRedWaiting >= 99 ? 1 : durationRedWaiting + 1;
				buttonFlag[1]=0;
			}
			if(is_button_pressed(2))buttonFlag[2]=1;
			if((!is_button_pressed(2)) && (buttonFlag[2])){
				durationRed = durationRedWaiting;
				buttonFlag[2]=0;
			}
			break;
		case MOD_YELLOW:
			if(is_button_pressed(0))buttonFlag[0]=1;
			if((!is_button_pressed(0))&&(buttonFlag[0])){
				mode=MOD_GREEN;
				traff1Out=3;
				traff2Out=3;
				buttonFlag[0]=0;
				buttonFlag[1]=0;
				buttonFlag[2]=0;
			}
			if(is_button_pressed(1))buttonFlag[1]=1;
			if((!is_button_pressed(1)) && (buttonFlag[1])){
				durationYellowWaiting = durationYellowWaiting >= 99 ? 1 : durationYellowWaiting + 1;
				buttonFlag[1]=0;
			}
			if(is_button_pressed(2))buttonFlag[2]=1;
			if((!is_button_pressed(2)) && (buttonFlag[2])){
				durationYellow = durationYellowWaiting;
				buttonFlag[2]=2;
			}
			break;
		case MOD_GREEN:
			if(is_button_pressed(0))buttonFlag[0]=1;
			if((!is_button_pressed(0))&&(buttonFlag[0])){
				mode=INIT;
			}
			if(is_button_pressed(1))buttonFlag[1]=1;
			if((!is_button_pressed(1)) && (buttonFlag[1])){
				durationGreenWaiting = durationGreenWaiting >= 99 ? 1 : durationGreenWaiting + 1;
				buttonFlag[1]=0;
			}
			if(is_button_pressed(2))buttonFlag[2]=1;
			if((!is_button_pressed(2)) && (buttonFlag[2])){
				durationGreen = durationGreenWaiting;
				buttonFlag[2]=0;
			}
			break;
	}
}

//Function for handling LEDs
void led_processing(){
	switch(mode){
		case MOD_RED:
			traff1Out=traff1Out==6?7:6;
			traff2Out=traff2Out==6?7:6;
			break;
		case MOD_YELLOW:
			traff1Out=traff1Out==5?7:5;
			traff2Out=traff2Out==5?7:5;
			break;
		case MOD_GREEN:
			traff1Out=traff1Out==3?7:3;
			traff2Out=traff2Out==3?7:3;
			break;
	}
	if(mode!=NORMAL)return;
	switch(traffCond){
		case RG:
			traff1Out=6;
			traff2Out=3;
			break;
		case RY:
			traff1Out=6;
			traff2Out=5;
			break;
		case GR:
			traff1Out=3;
			traff2Out=6;
			break;
		case YR:
			traff1Out=5;
			traff2Out=6;
			break;
	}
}

//Function for handling 7 segments LED buffer & decreasing the time
void buffer_processing(){
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
	if(mode!=NORMAL)return;
	light1Time--;
	light2Time--;
	switch(traffCond){
		case RG:
			if(light2Time<=0){
				traffCond=RY;
				light2Time=durationYellow;
			}
			break;
		case RY:
			if(light1Time<=0){
				traffCond=GR;
				light1Time=durationGreen;
				light2Time=durationRed;
			}
			break;
		case GR:
			if(light1Time<=0){
				traffCond=YR;
				light1Time=durationYellow;
			}
			break;
		case YR:
			if(light2Time<=0){
				traffCond=RG;
				light1Time=durationRed;
				light2Time=durationGreen;
			}
			break;
	}
	led_buffer[0]=light1Time/10;
	led_buffer[1]=light1Time%10;
	led_buffer[2]=light2Time/10;
	led_buffer[3]=light2Time%10;
}
