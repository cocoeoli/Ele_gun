/*
 * pwm_config.h
 *
 *  Created on: 2019.4.21
 *      Author: DuYihao
 */

#ifndef _PWM_CONFIG_H_
#define _PWM_CONFIG_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"


void PWM_clock_div_set(void);
void PWM0_01_init(void);
//void PWM0_4567_init(void);

#endif /* _PWM_CONFIG_H_ */
