/*
 * hardware_processing.h
 *
 *  Created on: Oct 23, 2023
 *      Author: hhaof
 */

#ifndef INC_HARDWARE_PROCESSING_H_
#define INC_HARDWARE_PROCESSING_H_

//7Seg & 7Seg buffer
#ifndef MAX7SEG
#define MAX7SEG 4
#endif /* MAX7SEG */

void update7Seg(int index);
void update7SegBuffer();
void update7SegMain();

//Registering output to pins
void displayAll();

#endif /* INC_HARDWARE_PROCESSING_H_ */
