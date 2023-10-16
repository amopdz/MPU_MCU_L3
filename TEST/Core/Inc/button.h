/*
 * button.h
 *
 *  Created on: Oct 5, 2023
 *      Author: KAI
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

void button_reading ();
unsigned char is_button_pressed ( unsigned char index );
unsigned char is_button_pressed_1s ( unsigned char index );
unsigned char is_button_pressed_3s ( unsigned char index );

#endif /* INC_BUTTON_H_ */

