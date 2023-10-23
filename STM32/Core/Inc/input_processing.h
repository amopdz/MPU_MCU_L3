/*
 * input_processing_new.h
 *
 *  Created on: Oct 23, 2023
 *      Author: hhaof
 */

#ifndef INC_INPUT_PROCESSING_H_
#define INC_INPUT_PROCESSING_H_

#include "main.h"

//States for the FSM of buttons
#define INIT 0
#define NORMAL 1
#define MOD_RED 2
#define MOD_YELLOW 3
#define MOD_GREEN 4

//Button FSM
void fsm_automatic();

//Display buffers
extern uint8_t traff1Out,
			   traff2Out,
			   led_buffer[];

//LED & 7 Seg buffer processing
void led_processing();
void buffer_processing();

#endif /* INC_INPUT_PROCESSING_H_ */
