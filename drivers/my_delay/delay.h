/*
 * delay.h
 *
 *  Created on: 2018Äê1ÔÂ26ÈÕ
 *      Author: wk
 */

#ifndef DELAY_FCT_DELAY_H_
#define DELAY_FCT_DELAY_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

extern void delay_us(unsigned int n);
extern void delay_ms(unsigned int n);
extern void delay_s (unsigned int n);

#endif /* DELAY_FCT_DELAY_H_ */
