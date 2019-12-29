/*
 * wtimer.h
 *
 *  Created on: 2019Äê8ÔÂ3ÈÕ
 *      Author: xiong
 */

#ifndef DRIVERS_TIMER_WTIMER_H_
#define DRIVERS_TIMER_WTIMER_H_

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

//void wtimer2AB_init(uint16_t Amscount ,uint16_t Bmscount);
void wtimer4_init(uint8_t  scount);

//void WTimer2A_IntHandler(void);
//void WTimer2B_IntHandler(void);
void WTimer4_IntHandler(void);



#endif /* DRIVERS_TIMER_WTIMER_H_ */
