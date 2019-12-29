/*
 * wtimer.c
 *
 *  Created on: 2019年8月3日
 *      Author: xiong
 */
#include "wtimer.h"

//void wtimer2AB_init(uint16_t Amscount,   uint16_t Bmscount)
//{
//       SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);
//       while(!SysCtlPeripheralReady(SYSCTL_PERIPH_WTIMER2)) ;
//
//       TimerConfigure(WTIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC |TIMER_CFG_B_PERIODIC);   //使用两个32位定时器周期计数模式
//       TimerLoadSet(WTIMER2_BASE, TIMER_A , Amscount*(SysCtlClockGet()/1000)-1);   //计数次数1000*mscount单位
//       TimerLoadSet(WTIMER2_BASE, TIMER_B , Bmscount*(SysCtlClockGet()/1000)-1);   //计数次数1000*mscount单位
////       TimerLoadSet64(WTIMER2_BASE, mscount*(SysCtlClockGet()/1000) );    //计数次数1000*mscount单位
//
//       IntPrioritySet(INT_WTIMER2A, 0x21);
//       IntPrioritySet(INT_WTIMER2B, 0x22);
//
//       TimerIntRegister(WTIMER2_BASE, TIMER_A, WTimer2A_IntHandler);
//       TimerIntRegister(WTIMER2_BASE, TIMER_B, WTimer2B_IntHandler);
//
//       IntEnable(INT_WTIMER2A);
//       IntEnable(INT_WTIMER2B);
//       TimerIntEnable(WTIMER2_BASE, TIMER_TIMA_TIMEOUT);
//       TimerIntEnable(WTIMER2_BASE, TIMER_TIMB_TIMEOUT);
//
////          TimerDisable(WTIMER2_BASE, TIMER_A);
//       TimerDisable(WTIMER2_BASE, TIMER_BOTH);
//}

extern uint8_t Time_count;

void wtimer4_init(uint8_t  scount)
{
         SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER4);
          while(!SysCtlPeripheralReady(SYSCTL_PERIPH_WTIMER4)) ;

          TimerConfigure(WTIMER4_BASE, TIMER_CFG_PERIODIC);   //使用全宽64位定时器周期计数模式
//          TimerPrescaleSet(WTIMER3_BASE, TIMER_BOTH, 8 - 1);     //预分频系数 8

          TimerLoadSet64(WTIMER4_BASE, scount*(SysCtlClockGet()) );   //1000000*scount次

          IntPrioritySet(INT_WTIMER4A, 0x10);
          TimerIntRegister(WTIMER4_BASE, TIMER_BOTH, WTimer4_IntHandler);

          IntEnable(INT_WTIMER4A);
//          TimerIntDisable(WTIMER3_BASE, TIMER_TIMA_TIMEOUT);
          TimerIntEnable(WTIMER4_BASE, TIMER_TIMA_TIMEOUT);

          TimerEnable(WTIMER4_BASE, TIMER_BOTH);
//          TimerDisable(WTIMER4_BASE, TIMER_BOTH);

}

//void WTimer2A_IntHandler(void)         //100ms一次
//{
//            TimerIntClear(WTIMER2_BASE, TIMER_TIMA_TIMEOUT);
// }
//
//void WTimer2B_IntHandler(void)         //90ms一次
//{
//            TimerIntClear(WTIMER2_BASE, TIMER_TIMB_TIMEOUT);
// }

void WTimer4_IntHandler(void)         //s一次
{
        TimerIntClear(WTIMER4_BASE, TIMER_TIMA_TIMEOUT);
        Time_count++;
 }



