/*
 * input_reading_new.h
 *
 *  Created on: Oct 23, 2023
 *      Author: hhaof
 */

#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

#include "main.h"

#ifndef N0_OF_BUTTONS
#define N0_OF_BUTTONS 3
#endif

void button_reading();
uint8_t is_button_pressed(uint8_t index);

#endif /* INC_INPUT_READING_H_ */
