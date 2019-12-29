#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pwm.h"
#include "driverlib/rom_map.h"
#include "uartstdio.h"
#include "Circuit.h"
#include "drivers/my_delay/delay.h"
#include "drivers/uart/uart.h"
#include "drivers/pwm/pwm_config.h"
#include "drivers/timer/timer_config.h"
#include "drivers/timer/wtimer.h"
#include "drivers/key/Key.h"
#include "drivers/Laser/laser.h"

////////////////中断优先级
//    UART0        0x08
//    UART1        0x05
//    UART4        0x02
//    UART5        0x01
//    up_int         0x03
//    wtimer4      0x10  计时器
//////////////////////

void mode_choice(void);

uint8_t Key_flag;
uint8_t  emi_flag = 0;       //数据采集成功标记  1--mode2成功       2--mode3
uint8_t Time_count = 0;      //保持时间，防止电没充满就释放
uint8_t uart_flag;                 //8位代表8个uart中断  00000001--uart0
uint8_t Mode = 1;               //模式控制  1-5       // 0--停止
uint8_t W_stop=0;               //发挥部分停止位
uint8_t Left=1;                     //向左移标志
char Aim_char = 0;
uint16_t count= 1130;        //舵机转角标志   count*6防抖130
uint16_t  Dis = 0;               //mm为单位
int    Angle = 0;          //角度值C
int Dis_set = 0;

extern uint16_t final_count;

extern char uart0_buff[Buff0_Len];

void main(void)
{
//    uint8_t d=0;
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |SYSCTL_XTAL_16MHZ);      //200/2.5=40M
    FPULazyStackingEnable();
    FPUEnable();
    Circuit_init();                          //电路控制初始化
//    timer0_init();                           //速度计算
//    Cap();                                     //高电平捕获
    Laser_init();                            //激光测距，发送‘D’启动测量
    PWM0_01_init();                   //舵机控制
    UART0_init(115200);              //测试用显示
    UART1_Init(115200);               //openmv通信
    UART4_Init(19200);                 //激光测距
    UART5_Init(115200);              //显示串口屏
    IntMasterEnable();
    while(1)
    {
           if(uart_flag != 0){
                if(uart_flag&0x01){                             //uart0中断
//                    if(uart0_buff[0] == '+'){
//                        d++;
//                        count = count + 12;
//                    }
//                    if(uart0_buff[0] == '-'){
//                        d--;
//                        count = count - 12;
//                    }
//                    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
//                    UARTprintf("icount : %d   i:%d\n", count, d);
//                    Charging(0);
//                    delay_s(1);
//                    Emission();
                    uart_flag = 0;
                }
                if(uart_flag&0x02){                             //uart1
                    uart_flag = 0;
                    Data1pro();
                }
           }
           mode_choice();
    }
}
void mode_choice(void)
{
    float x = 0;
    float biasx_angle = 0,  biasy_angle=0;   //用作偏角计算
    char Aimcharbuf[2];
    switch(Mode)             //模式
    {
        case 0:{                  //停止状态
//            if( Left ){
//               if( count++  >  1850){
//                PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
//                delay_ms(5);
//                 Left = 0;
//                 }
//               PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
//               delay_ms(6);
//               PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*(2390-174)/20000);
//               delay_ms(6);  //6
//           }else{
//               if( count--  <  1130){
//                     PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
//                     delay_ms(5);
//                      Left = 1;
//                       }
//                       PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
//                       delay_ms(6);
//                         PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*(2390-174)/20000);
//                       delay_ms(6);
//           }

            break;
        }
        case 1:{                 //只需要发射
            delay_s(5);
            Charging(1);
            delay_s(9);      //充电时间
            Charging(0);
            delay_s(1);
            Emission();
            Charging(1);
            Mode = 0;
            break;
        }
        case 2:{
            if(  emi_flag == 1 ){
                x = -0.00025*Dis_set*Dis_set+0.22*Dis_set-27;
                x = 2000*x/180;
                PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*(2390-(int)x-24)/20000);  //舵机转固定角度
                delay_s(2);
                Charging(0);
                delay_s(1);
                 Emission();
                Charging(1);
                emi_flag = 0;
                Mode = 0;
                PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*1490/20000);//Duty:2.5%锛� 1.5ms
                PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*2336/20000); //Duty:2.5%
            }
            break;
        }
        case 3:{
            if(  emi_flag == 2 ){
                   Angle = Angle*2000/180;
                   PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*(1490+Angle)/20000);  //舵机转固定角度左右
                   delay_s(2);
                   x = -0.00025*Dis_set*Dis_set+0.22*Dis_set-27;
                   x = 2000*x/180;
                   PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*(2390-(int)x-48)/20000);  //舵机转固定角度
                   delay_s(2);
                   Charging(0);
                   delay_s(1);
                   Emission();
                   emi_flag = 0;
                   Mode = 0;
                   PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*1490/20000);         //控制回到起点，最左边
                   PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*2336/20000);          //炮筒
                   Charging(1);
               }
            break;
        }
        case 4:{
            if(W_stop){
                do{
                    UART_send_char('D', UART4_BASE );     //激光测量触发
                    delay_ms(10);
                }while(!Dis);
                x = -0.0000025*(Dis-300)*(Dis-300)+0.022*(Dis-300)-27;
                x = 2000*x/180;
                UART_send_string("Dis: \t", UART0_BASE);
                UART_send_int32_t(Dis, 1 , UART0_BASE);
                UART_send_string( "\t\0xd\0xa", UART0_BASE);
                UART_send_string("X: \t", UART0_BASE);
                 UART_send_int32_t((int) x, 1 , UART0_BASE);
                PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*(2390-(int)x-36)/20000);
                delay_s(3);
                Charging(0);
                delay_s(1);
                Emission();
                Mode = 0;
                PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*1130/20000);         //控制回到起点，最右边
                PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*2336/20000);
                Charging(1);
                Left = 1;
                W_stop = 0;
            }else{
                if( Left ){
                    if( count++  >  1850){
                                    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                                    delay_ms(5);
                                    Left = 0;
                                }
                                PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                                delay_ms(6);
                }else{
                    if( count--  <  1130){
                                    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                                    delay_ms(5);
                                    UART_send_string("*X#", UART1_BASE);
                                    Left = 1;
                                }
                                PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                                delay_ms(6);
                }
            }
            break;
        }
        case 5:{
            if(final_count == 0){
                if( Left ){
                   if( count++  >  1850){
                    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                    delay_ms(5);
                     Left = 0;
                     }
                   PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                   delay_ms(3);
                   PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*(2390-174)/20000);
                   delay_ms(6);  //6
               }else{
                   if( count--  <  1130){
                         PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                         delay_ms(5);
                          Left = 1;
                           }
                           PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                           delay_ms(3);
                             PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*(2390-174)/20000);
                           delay_ms(6);
               }
            }
            else{
                if( Left ){
                   if( count++  >  1850){
                           PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                           Left = 0;
                       }
                   if( abs(final_count-count)  <  250 )
                       {
                           Charging(0);
                           if( abs(final_count-count)  <  37  ){
                               GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0x0);
                                delay_ms(20);
                                GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0xFF);
                                delay_ms(20);
//                                 Mode = 0;
                                 Charging(1);
//                                 delay_s(5);
                                 final_count = 0;
//                                PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*1166/20000);         //控制回到起点，最左边
//                                PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*2390/20000);
                           }
                       }
                       PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                       delay_ms(3);
                       PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*(2390-174)/20000);
                       delay_ms(1);
               }else{
                   if( count--  <  1130){
                                   PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                                   delay_ms(5);
                                   Left = 1;
                               }
                               PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                               if(abs(final_count-count)<  250  )
                                 {
                                     Charging(0);
                                     if( abs(final_count-count)  <  37  )
                                         GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0x0);
                                         delay_ms(20);
                                         GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0xFF);
                                         delay_ms(20);
                                           Charging(1);
                                           final_count = 0;
                                 }
                               delay_ms(6);
               }
                final_count = 0;
            }
           break;

       }
        case 6:{                 //自主发挥  Mode = 6
            if(W_stop){
                Aimcharbuf[1] = Aimcharbuf[0];
                 Aimcharbuf[0] = Aim_char;
                    if(  Aimcharbuf[1] != Aimcharbuf[0] ){
                        do{
                                    UART_send_char('D', UART4_BASE );     //激光测量触发
                                    delay_ms(10);
                               }while(!Dis);
                    }

                    if( Aim_char == '1' ||  Aim_char == '4'  || Aim_char == '7')
                        biasx_angle = atan( 200.0/Dis )*180/3.1415 ;
                    else if(  Aim_char == '3' ||  Aim_char == '6'  || Aim_char == '9' )
                        biasx_angle = -atan( 200.0/Dis )*180/3.1415 ;
                    else biasx_angle = 0;
                    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*(1490+(short)(biasx_angle*2000/180))/20000);  //舵机转角度
                    delay_ms(100);

                    if( Aim_char == '1' ||  Aim_char == '2'  || Aim_char == '3' )
                        biasy_angle = atan( (550.0- 350)/Dis )*180/3.1415 ;
                    else if( Aim_char == '4' ||  Aim_char == '5'  || Aim_char == '6' )
                        biasy_angle = 0 ;
                    else biasy_angle = atan( (150.0- 350)/Dis )*180/3.1415 ;
                    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*(2360-(short)(biasy_angle*2000/180))/20000);
                    delay_ms(100);
                }else{
                if( Left ){
                    if( count++  >  1850){
                                   PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                                    delay_ms(5);
                                    Left = 0;
                                }
                                PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                                delay_ms(6);
                }else{
                    if( count--  <  1130){
                                    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                                    delay_ms(5);
                                    Left = 1;
                                }
                                PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                                delay_ms(6);
                }
            }
                  break;
              }
    }
}

