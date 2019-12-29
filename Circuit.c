/*
 * Circuit.c
 *
 *  Created on: 2019��8��8��
 *      Author: xiong
 */
#include "Circuit.h"
#include "drivers/my_delay/delay.h"

void Circuit_init(void)       //�̵�����ϵͳ�ƹ����
{
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
       GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_6);       //PA5,PA6���Ƽ̵���
       GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_6, 0xFF);                  //�ߵ�ƽ
}

void Charging(uint8_t on_off)      //ģʽ��ʼ�ͳ�磬����ǰ�ŵ�
{
    if(!on_off){                       //�����״̬
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0xFF);
    }else{
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0x00);
         //�����
    }
}

void Emission(void)           //�����·�򿪣���ʱ2S��ر�
{
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0x0);
    delay_s(2);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0xFF);
}



