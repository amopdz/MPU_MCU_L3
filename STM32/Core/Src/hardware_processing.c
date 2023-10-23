/*
 * hardware_processing.c
 *
 *  Created on: Oct 23, 2023
 *      Author: hhaof
 */

#include "main.h"
#include "input_processing.h"

//Array storing pin output for the desired numbers
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

uint8_t led7SegCounter=MAX7SEG-1,
		segOut=0,
		enOut=0;

//FUNCTIONS
//7Seg & 7Seg buffer & Driver code for these functions
void display7Seg(uint8_t number){
	if(number>=0 && number<10)segOut=digitMask[number];
}

void update7Seg(int index){
	display7Seg(led_buffer[index]);
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

void update7SegMain(){
	led7SegCounter=led7SegCounter>=MAX7SEG-1?0:led7SegCounter+1;
	update7Seg(led7SegCounter);
}

//Registering output to pins
void displayAll(){
	GPIOA->ODR=(traff1Out<<12)|(traff2Out<<4);
	GPIOB->ODR=(enOut<<8)|(segOut);
}
