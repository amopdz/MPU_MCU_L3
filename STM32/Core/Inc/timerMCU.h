/*
 * timerMCU.h
 *
 *  Created on: Oct 13, 2023
 *      Author: hh.aof
 */

#ifndef INC_TIMERMCU_H_
#define INC_TIMERMCU_H_

#ifndef N0_OF_TIMER
#define N0_OF_TIMER 4
#endif /* N0_OF_TIMER */

extern uint8_t timerFlag[];

void initTimer();
void setTimer(uint8_t, uint8_t);
void timerRun();

#endif /* INC_TIMERMCU_H_ */
