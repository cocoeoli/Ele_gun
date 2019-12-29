/*
 * Circuit.c
 *
 *  Created on: 2019年8月8日
 *      Author: xiong
 */
#include "Circuit.h"
#include "drivers/my_delay/delay.h"

void Circuit_init(void)       //继电器及系统灯光控制
{
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
       GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_6);       //PA5,PA6控制继电器
       GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_6, 0xFF);                  //高电平
}

void Charging(uint8_t on_off)      //模式开始就充电，发射前放电
{
    if(!on_off){                       //不充电状态
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0xFF);
    }else{
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0x00);
         //亮红灯
    }
}

void Emission(void)           //发射电路打开，延时2S后关闭
{
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0x0);
    delay_s(2);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0xFF);
}



