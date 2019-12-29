/*
 * laser.h
 *
 *  Created on: 2019Äê8ÔÂ9ÈÕ
 *      Author: xiong
 */

#ifndef DRIVERS_LASER_LASER_H_
#define DRIVERS_LASER_LASER_H_

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

void Laser_init(void);
void Cal_laser(void);



#endif /* DRIVERS_LASER_LASER_H_ */
