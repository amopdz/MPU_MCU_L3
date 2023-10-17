/*
 * input_processing.h
 *
 *  Created on: Oct 13, 2023
 *      Author: hh.aof
 */

#ifndef INC_INPUT_PROCESSING_H_
#define INC_INPUT_PROCESSING_H_

//BUTTON FSM
void fsm_for_mode_button();
void fsm_for_modify_button();
void fsm_for_set_button();

//MODE
void mode_normal();
void mode_mod_red();
void mode_mod_yellow();
void mode_mod_green();

//MAIN
void mainMode();

//7SEG & 7SEG BUFFER

void update7SEG(int index);
void update7SEGBuffer();
void update7SEGMain();

//TRAFFIC LIGHTS & TRAFFIC LIGHTS BUFFER
void updateLED();

//REGISTER ALL TO PIN
void displayAll();

#endif /* INC_INPUT_PROCESSING_H_ */
