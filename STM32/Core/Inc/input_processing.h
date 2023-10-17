/*
 * input_processing.h
 *
 *  Created on: Oct 13, 2023
 *      Author: hh.aof
 */

#ifndef INC_INPUT_PROCESSING_H_
#define INC_INPUT_PROCESSING_H_

//Button FSMs
void fsm_for_mode_button();
void fsm_for_modify_button();
void fsm_for_set_button();

//Individual mode handlers
void mode_normal();
void mode_mod_red();
void mode_mod_yellow();
void mode_mod_green();

//Driver code for modes
void mainMode();

//7Seg & 7Seg buffer
#ifndef MAX7SEG
#define MAX7SEG 4
#endif /* MAX7SEG */

void update7Seg(int index);
void update7SegBuffer();
void update7SegMain();

//Registering output to pins
void displayAll();

#endif /* INC_INPUT_PROCESSING_H_ */
