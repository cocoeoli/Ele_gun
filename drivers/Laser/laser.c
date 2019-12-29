/*
 * laser.c
 *
 *  Created on: 2019��8��9��
 *      Author: xiong
 */
#include "stdio.h"
#include "laser.h"
#include "../uart/uart.h"

extern char uart4_buff[Buff4_Len];
extern uint16_t  Dis;

void Laser_init(void)      //PC7 power_on      PC6  ����ģʽ
{
          SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);      //ʹ��GPIO����
          GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7|GPIO_PIN_6);       //PC7,PC6���Ƽ���
          GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0xFF);                  //�ߵ�ƽ
          GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0xFF);                  //�͵�ƽ
}

void Cal_laser(void)
{
    char num_char[20];
    char dis_c[7], acc_c[5];      //��ŵ�ǰ���뼰׼ȷ���ַ�
    static uint8_t dcount = 0;
    static float  this_dis[2],  this_acc[2];  //������ε�ǰ�����׼ȷ��
    uint8_t i;

    for(i=0; i<15; i++)
        num_char[i] = uart4_buff[i];

    if((num_char[0] == 'D' ||  num_char[0] == ':')  &&  (num_char[1] == ' ')){    //���ݸ�ʽ��ȷ
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
        if(this_acc[0] > 1000 &&  this_acc[1] > 1000)   //��׼�Ȳ��� ���²���
        {
            UART_send_char('D', UART4_BASE);
        }
        else{                     //�Ƚ�������ѡȡ��׼��С��
            if( this_acc[0] > this_acc[1] ){
                if( this_dis[1] > 2 && this_dis[1] < 3 )
                    Dis = (int)(this_dis[1]*1000);            //mm��λ

            }else{
                if( this_dis[0] > 2 && this_dis[0] < 3 )
                     Dis = (int)(this_dis[0]*1000);            //mm��λ
                else          //�������ݷǰб�����
                    UART_send_char('D', UART4_BASE);
            }
        }
    }
    else{         //���²���
        UART_send_char('D', UART4_BASE);
    }

}



