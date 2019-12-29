/*
 * Circuit.h
 *
 *  Created on: 2019Äê8ÔÂ8ÈÕ
 *      Author: xiong
 */

#ifndef CIRCUIT_H_
#define CIRCUIT_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom_map.h"


void Circuit_init(void);
void Charging(uint8_t on_off);
void Emission(void);


#endif /* CIRCUIT_H_ */
