/*
 * laser.c
 *
 *  Created on: 2019年8月9日
 *      Author: xiong
 */
#include "stdio.h"
#include "laser.h"
#include "../uart/uart.h"

extern char uart4_buff[Buff4_Len];
extern uint16_t  Dis;

void Laser_init(void)      //PC7 power_on      PC6  连续模式
{
          SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);      //使能GPIO外设
          GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7|GPIO_PIN_6);       //PC7,PC6控制激光
          GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0xFF);                  //高电平
          GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0xFF);                  //低电平
}

void Cal_laser(void)
{
    char num_char[20];
    char dis_c[7], acc_c[5];      //存放当前距离及准确度字符
    static uint8_t dcount = 0;
    static float  this_dis[2],  this_acc[2];  //存放两次当前距离和准确度
    uint8_t i;

    for(i=0; i<15; i++)
        num_char[i] = uart4_buff[i];

    if((num_char[0] == 'D' ||  num_char[0] == ':')  &&  (num_char[1] == ' ')){    //数据格式正确
        for(i=0; i<6; i++)
            dis_c[i] = num_char[i+2];
        dis_c[6] = '\0';
        for(i=0; i<4; i++)
            acc_c[i] = num_char[i+9];
        acc_c[4] = '\0';
         if(dcount==2)
             dcount = 0;
        sscanf(dis_c, "%f", &this_dis[dcount] );
        sscanf(acc_c, "%f", &this_acc[dcount] );
        dcount ++;
        if(this_acc[0] > 1000 &&  this_acc[1] > 1000)   //精准度不够 重新测量
        {
            UART_send_char('D', UART4_BASE);
        }
        else{                     //比较两个，选取精准度小的
            if( this_acc[0] > this_acc[1] ){
                if( this_dis[1] > 2 && this_dis[1] < 3 )
                    Dis = (int)(this_dis[1]*1000);            //mm单位

            }else{
                if( this_dis[0] > 2 && this_dis[0] < 3 )
                     Dis = (int)(this_dis[0]*1000);            //mm单位
                else          //测量数据非靶标数据
                    UART_send_char('D', UART4_BASE);
            }
        }
    }
    else{         //重新测量
        UART_send_char('D', UART4_BASE);
    }

}



