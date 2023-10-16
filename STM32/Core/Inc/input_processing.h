/*
 * input_processing.h
 *
 *  Created on: Oct 13, 2023
 *      Author: hh.aof
 */

#ifndef INC_INPUT_PROCESSING_H_
#define INC_INPUT_PROCESSING_H_

//BUTTON FSM
void fsm_for_mode_button(void);
void fsm_for_modify_button(void);
void fsm_for_set_button(void);

//MODE
void mode_normal();
void mode_mod_red();
void mode_mod_yellow();
void mode_mod_green();

//MAIN
void mainMode();

//7SEG & 7SEG BUFFER
const uint8_t digitMask[11];
const int MAX_7SEG;

void update7SEG(int index);
void update7SEGBuffer();

//TRAFFIC LIGHTS & TRAFFIC LIGHTS BUFFER
const uint8_t ledMask[4];
uint8_t traff1Out, traff2Out, segOut;

void updateLED();

//REGISTER ALL TO PIN
void displayAll();

#endif /* INC_INPUT_PROCESSING_H_ */
