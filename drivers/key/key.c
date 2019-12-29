///*
// * Key.c
// *
// *  Created on: 2019年8月3日
// *      Author: liziyang
// */
//#include "Key.h"
//#include "../timer/timer_config.h"
//
//extern uint8_t Key_flag;
//extern float Speed;
//
//void Key_Init(void)
//{
//       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//使能GPIO外设
//       /* Unlock PF0 (SW2)*/
//       HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
//       HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
//       HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0x00;
//
//       //配置SW1和SW2
//       GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
//       GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0 ,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
//
//       //按键低电平触发
//       GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_LOW_LEVEL);
//       GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_LOW_LEVEL);
//
//       //中断注册
//       GPIOIntRegister(GPIO_PORTF_BASE,Key_int);
//       IntPrioritySet(INT_GPIOF,0x02);
//       GPIOIntEnable(GPIO_PORTF_BASE,GPIO_INT_PIN_4|GPIO_INT_PIN_0);
//       IntEnable(INT_GPIOF);
//}
//
//void Cap(void)
//{
//          SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//使能GPIO外设
//
//          GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_3);
//          GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_3 ,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPD);
//
//          //高电平触发
//          GPIOIntTypeSet(GPIO_PORTE_BASE,GPIO_PIN_3,GPIO_HIGH_LEVEL);
//
//          //中断注册
//          GPIOIntRegister(GPIO_PORTE_BASE,Cap_int);
//          IntPrioritySet(INT_GPIOE, 0x03);
//          GPIOIntEnable(GPIO_PORTE_BASE,GPIO_INT_PIN_3);
//          IntEnable(INT_GPIOE);
//}
//
//
//void Cap_int(void)
//{
//        static uint32_t value[2] = {0, 0};
//        static uint8_t count = 0;
//        uint32_t s = GPIOIntStatus(GPIO_PORTE_BASE, true);
//        GPIOIntClear(GPIO_PORTE_BASE, s);
//
//        if((s&GPIO_PIN_3) == GPIO_PIN_3)
//        {
//               value[count++] = TimerValueGet(TIMER0_BASE, TIMER_BOTH);
//               if(count == 2)
//               {
//                   count = 0;
//                   float fperiod = value[1] > value[0] ? (value[1]-value[0]):(value[1]-value[0]+65535);
//                   float temp =   610/fperiod   ;   //    0.01/(0.1/61*fperiod);         //速度cm/s
//                   Speed = temp;
//                   UARTprintf("Speed: %d \n", (int)Speed*10);
//                   GPIOIntTypeSet(GPIO_PORTE_BASE,GPIO_PIN_3,GPIO_HIGH_LEVEL);
//           //        if(temp > 400)//超过4m认为数据错误
//           //            TimerControlEvent(WTIMER0_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
//           //        else
//           //            dis2 = temp*10;
//               }else
//                      GPIOIntTypeSet(GPIO_PORTE_BASE,GPIO_PIN_3,GPIO_LOW_LEVEL);
//        }
//}
//
//void Key_int(void)
//{
//    uint32_t s = GPIOIntStatus(GPIO_PORTF_BASE, true);
//    GPIOIntClear(GPIO_PORTF_BASE, s);
//
//    if((s&GPIO_PIN_4) == GPIO_PIN_4)
//    {
//        Key_flag = 1;
//    }
//    if((s&GPIO_PIN_0) == GPIO_PIN_0)
//    {
//        Key_flag = 2;
//    }
//}
//
