/*
 * pwm_config.c
 *
 *  Created on: 2019.4.21
 *      Author: DuYihao
 */
#include "pwm_config.h"

int pwm_clock_div;

void PWM_clock_div_set(void)
{
    SysCtlPWMClockSet(SYSCTL_PWMDIV_16);
    pwm_clock_div = 16;
}

void PWM0_01_init(void)//妞瑰崬濮╅悽鍨簚
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0)) ;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)) ;
    
    PWM_clock_div_set();
    
//    GPIOPinConfigure(GPIO_PB4_M0PWM2);
//    GPIOPinConfigure(GPIO_PB5_M0PWM3);
    GPIOPinConfigure(GPIO_PB6_M0PWM0);
    GPIOPinConfigure(GPIO_PB7_M0PWM1);
//    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);
//    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);

    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
//    PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, SysCtlClockGet()/pwm_clock_div/50);   //50Hz, 20ms
//    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, SysCtlClockGet()/pwm_clock_div/1000); //1KHz
    
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*1490/20000);  //Duty:2.5%锛� //1490
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*2336/20000);   //Duty:2.5%    //2360多调节2度
//    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1)*75/100);//Duty:60%
//    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1)*60/100);//Duty:60%

    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
//    PWMGenEnable(PWM0_BASE, PWM_GEN_1);
    
//    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, false);
//    PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, false);
//    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, false);
//    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, false);

    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
//    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);
//    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, true);
}

//void PWM0_4567_init(void)//娴肩儤婀囬悽鍨簚
//{
//    //瀵拷閸氼垰顦荤拋锟�
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
//    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0))
//        ;
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
//    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
//        ;
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
//    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
//    //閺冨爼鎸撻崚鍡涱暥椤ｏ拷
//    PWM_clock_div_set();
//
//    //瀵洝鍓兼径宥囨暏鐠佸墽鐤�
//    GPIOPinConfigure(GPIO_PE4_M0PWM4);
//    GPIOPinConfigure(GPIO_PE5_M0PWM5);
//    GPIOPinConfigure(GPIO_PC4_M0PWM6);
//    GPIOPinConfigure(GPIO_PC5_M0PWM7);
//    GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4);
//    GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_5);
//    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);
//    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_5);
//
//    //閸欐垹鏁撻崳銊ヤ紣娴ｆ粍膩瀵繗顔曠純锟�
//    PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
//    PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
//
//    //妫版垹宸肩拋鍓х枂
//    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, SysCtlClockGet()/pwm_clock_div/50);//50Hz
//    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, SysCtlClockGet()/pwm_clock_div/50);//50Hz
//
//    //閼村顔旂拋鍓х枂
//    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2)*95/1000);//pulse:500us
//    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2)*75/1000);//pulse:2500us
//    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_3)*75/1000);//pulse:500us
//    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_3)*115/1000);//pulse:500us
//
//    //閹垫挸绱戦崣鎴犳晸閸ｏ拷
//    PWMGenEnable(PWM0_BASE, PWM_GEN_2);
//    PWMGenEnable(PWM0_BASE, PWM_GEN_3);
//
//    //閸忔娊妫存潏鎾冲毉
//    PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, false);
//    PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, false);
//    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, false);
//    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, false);
//}
