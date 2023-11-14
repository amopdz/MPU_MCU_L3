/*
 * scheduler.h
 *
 *  Created on: Nov 13, 2023
 *      Author: hhaof
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h"

/*
 * Watchdog timer
 */
/*void MX_IWDG_Init();
void Watchdog_Refresh();
unsigned char Is_Watchdog_Reset();
void Watchdog_Counting();
void Reset_Watchdog_Counting();*/

/*
 * Scheduler
 */
void SCH_Init(void);
void SCH_Update(void);
unsigned char SCH_Add_Task(void (*pFunction)(), unsigned int, unsigned int);
void SCH_Dispatch_Tasks();
unsigned char SCH_Delete_Task(const unsigned char);
void SCH_Go_To_Sleep();
void SCH_Report_Status();

#endif /* INC_SCHEDULER_H_ */
