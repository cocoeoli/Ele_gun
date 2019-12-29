#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "../Laser/laser.h"
#include "stdlib.h"
#include "string.h"

char uart0_buff[Buff0_Len];
char uart2_buff[Buff2_Len];
char uart4_buff[Buff4_Len];
char uart1_buff[Buff1_Len];
char uart5_buff[Buff5_Len];

uint16_t final_count=0;

extern uint8_t Mode;
extern uint8_t uart_flag;
extern uint8_t W_stop;
extern uint8_t Left;
extern char Aim_char;
extern uint16_t count;

extern int    Angle ;          //角度值C
extern int Dis_set ;
extern uint8_t  emi_flag ;       //数据采集成功标记  1--mode2成功       2--mode3

void UART_send_char(const unsigned char Byte, int32_t URATX)
{
    UARTCharPut(URATX, Byte);
}
char UART_recive_char(int32_t URATX)
{
    char Byte;
    Byte = UARTCharGet(URATX);
    return Byte;
}
void UART_send_string(const char *sendbuf, int32_t URATX)
{
    while(*sendbuf != '\0')
    {
        UART_send_char(*sendbuf, URATX);
        sendbuf ++;
    }
}

void UART_send_int64_t(const int64_t temp,char on, int32_t URATX)
{
    int64_t temp_p = temp;

    if(temp_p < 0)
    {
        UART_send_char('-', URATX);
        temp_p = 0-temp_p;
    }
    if(temp_p > 0)
    {
        UART_send_int64_t(temp_p/10,0,URATX);
        UART_send_char(temp_p%10 + '0', URATX);
    }
    else if(temp_p == 0)
    {
        if(on)
            UART_send_char('0',URATX);
    //  UART_send_char('\n');
    }
}

void UART_send_int32_t(const int32_t temp,char on,int32_t URATX)
{
    UART_send_int64_t((int64_t) temp,on, URATX);
}

void UART_send_int16_t(const int16_t temp,char on, int32_t URATX)
{
    UART_send_int64_t((int64_t) temp,on, URATX);
}

void UART_send_float(const float temp,int64_t lenth, int32_t URATX)
{
    int64_t i;
    float num = temp;
    int64_t temp_p;
   //发送整数部分
    if(num<0)
    {
        UART_send_char('-', URATX);
        num = 0-num;
    }
    temp_p = (int64_t)num;
    UART_send_int64_t(temp_p,1, URATX);
    UART_send_char('.', URATX);
    //发送n位小数
    for(i=0; i<lenth; i++)
    {
        UART_send_char(((int)(10*(num - temp_p)))%10 + '0', URATX);
        num = num*10;
        temp_p = (int64_t)num;
    }
}

void UART_send_int64_t_string(const int64_t *temp, uint64_t num, int32_t URATX )
{
    uint64_t i;
    for(i=0; i<num; i++)
        UART_send_int64_t(temp[i],1,URATX);
}
void UART_send_int32_t_string(const int32_t *temp, uint64_t num, int32_t URATX)
{
    uint64_t i;
    for(i=0; i<num; i++)
        UART_send_int32_t(temp[i],1, URATX);
}
void UART_send_int16_t_string(const int16_t *temp, uint64_t num, int32_t URATX)
{
    uint64_t i;
    for(i=0; i<num; i++)
        UART_send_int16_t(temp[i],1, URATX);
}
void UART_send_char_string(const unsigned char *temp, uint64_t num, int32_t URATX)
{
    uint64_t i;
    for(i=0; i<num; i++)
            UART_send_char(temp[i], URATX);
}
void UART_send_float_string(const float *temp, int64_t lenth, uint64_t num, int32_t URATX)
{
    uint64_t i;
    for(i=0; i<num; i++)
        UART_send_float(temp[i],lenth, URATX);
}

void UART_recive_string(char *reciveBuf,int num, int32_t URATX)
{
    int i;
    for(i=0; i<num;i++)
        reciveBuf[i] = UART_recive_char(URATX);
}

//char DMAControlTable[1024];
//char udma_data[10];
//void Uart_dmainit(void)
//{
//            SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);     //时钟初始化
//            uDMAEnable();                                                            //模块使能
//            uDMAControlBaseSet(DMAControlTable);              //模块表
//            uDMAChannelAssign(UDMA_CH8_UART0RX);        //通道分配
//            uDMAChannelAttributeDisable(UDMA_CH8_UART0RX, UDMA_ATTR_USEBURST| UDMA_ATTR_HIGH_PRIORITY|        //失能相应功能
//                                                                    UDMA_ATTR_ALTSELECT| UDMA_ATTR_REQMASK);
//            uDMAChannelAttributeEnable(UDMA_CH8_UART0RX, UDMA_ATTR_REQMASK);
//            uDMAChannelControlSet(UDMA_CH8_UART0RX| UDMA_PRI_SELECT,                               //配置相应通道
//                                  UDMA_SIZE_8|  UDMA_SRC_INC_NONE| UDMA_DST_INC_NONE|  UDMA_ARB_8);
//            uDMAChannelTransferSet(UDMA_CH8_UART0RX|UDMA_PRI_SELECT,
//                                   UDMA_MODE_BASIC, (void*)(HWREG(UART0_BASE + UART_O_DR)), udma_data,10);
////            uDMAChannelTransferSet(UDMA_CH8_UART0RX|UDMA_PRI_SELECT,
////                                  UDMA_MODE_PINGPONG, (void*)(UART4_BASE+UART_O_DR),  remote_dataB,12);
//            uDMAChannelEnable(UDMA_CH8_UART0RX);
//}


void UART0_init(uint32_t Baud)

{
//    Uart_dmainit();                     //uart  dma
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);  //uart function
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 );     //equal to GPIOPadConfigSet()
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1 );

//    UARTDMAEnable(UART0_BASE,UART_DMA_RX);
    UARTStdioConfig(0, Baud, SysCtlClockGet());

//    UARTFIFOEnable(UART0_BASE);
//    UARTFIFOLevelSet(UART0_BASE,UART_FIFO_TX4_8, UART_FIFO_RX7_8);

    IntPrioritySet(INT_UART0,0x08);
    UARTIntRegister(UART0_BASE,UART0IntHandler);
    UARTIntEnable(UART0_BASE, UART_INT_RT);        //Recive error flag enable
}

void UART1_Init(uint32_t ui32Baud)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC); //Clock 16MHz
    UARTStdioConfig(1, ui32Baud, 16000000);
    IntPrioritySet(INT_UART1,0x05);
    UARTIntRegister(UART1_BASE,UART1IntHandler);

    UARTIntEnable(UART1_BASE, UART_INT_RT);    //Recive RT
}

//void UART2_Init(uint32_t Baud)
//{
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
//    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART2)) ;
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
//    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))  ;
//
//    GPIOPinConfigure(GPIO_PD6_U2RX);
//    GPIOPinConfigure(GPIO_PD7_U2TX);
//    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
//
//    UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), Baud, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
//    IntPrioritySet(INT_UART2, 0x05);
//   UARTIntRegister(UART2_BASE, UART2IntHandler);
//    UARTIntEnable(UART2_BASE, UART_INT_RT);
////    IntEnable(INT_UART2);
//}

void UART4_Init(uint32_t Baud)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART4)) ;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC)) ;

    GPIOPinConfigure(GPIO_PC4_U4RX);
    GPIOPinConfigure(GPIO_PC5_U4TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(), Baud, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    IntPrioritySet(INT_UART4,0x03);
    UARTIntRegister(UART4_BASE, UART4IntHandler);
    UARTIntEnable(UART4_BASE, UART_INT_RT);
}

void UART5_Init(uint32_t Baud)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART5)) ;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) ;

    GPIOPinConfigure(GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), Baud, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    IntPrioritySet(INT_UART5,0x01);
    UARTIntRegister(UART5_BASE, UART5IntHandler);
    UARTIntEnable(UART5_BASE, UART_INT_RT);
}

void UART0IntHandler(void)
{
    uint8_t i=0;
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, ui32Status);
    while(UARTCharsAvail(UART0_BASE))
    {
        uart0_buff[i++] = (char)UARTCharGetNonBlocking(UART0_BASE);
    }
    uart_flag = 0x01;

}

void UART1IntHandler(void)
{
    uint32_t ui32Status;
    uint16_t i=0;
    ui32Status = UARTIntStatus(UART1_BASE, true);
    UARTIntClear(UART1_BASE, ui32Status);
    while(UARTCharsAvail(UART1_BASE))
    {
        uart1_buff[i++] = (char)UARTCharGetNonBlocking(UART1_BASE);
    }
//    UART_send_string(uart1_buff, UART0_BASE);
    if( uart1_buff[1] == 'F' ){       //模式五
        final_count = count;
    }
    uart_flag = 0x02;
}

//void UART2IntHandler(void)
//{
//    uint32_t ui32Status;
//    uint16_t i=0;
//    ui32Status = UARTIntStatus(UART2_BASE, true);
//    UARTIntClear(UART2_BASE, ui32Status);
//    while(UARTCharsAvail(UART2_BASE))
//    {
//        uart2_buff[i++] = (char)UARTCharGetNonBlocking(UART2_BASE);
//    }
//    UART_send_string(uart2_buff, UART0_BASE);
//    uart_flag = 0x04;
//}

void UART4IntHandler(void)
{
    uint16_t i=0;
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART4_BASE, true);
    UARTIntClear(UART4_BASE, ui32Status);
    while(UARTCharsAvail(UART4_BASE))
    {
        uart4_buff[i++] = (char)UARTCharGetNonBlocking(UART4_BASE);
    }
    Cal_laser();             //测量距离
}


void UART5IntHandler(void)
{
    uint16_t i=0;
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART5_BASE, true);
    UARTIntClear(UART5_BASE, ui32Status);
    while(UARTCharsAvail(UART5_BASE))
    {
        uart5_buff[i++] = (char)UARTCharGetNonBlocking(UART5_BASE);
    }
//    UART_send_string(uart5_buff, UART0_BASE);
    Data5pro();
//    uart_flag = 0x10;
}

void Data1pro(void)   //数据处理函数
{
    char Num_char;
    Num_char = uart1_buff[1];
    switch(Num_char)     //模式
    {
        case 'S' :{
            W_stop = 1;
            break;
        }
        case 'L' :{
            Left=1;
            break;
        }
        case 'R' :{
            Left=0;
            break;
        }
        default:  break;
    }
}


void Data5pro(void)   //数据处理函数
{
    char Num_char[2] ;
    uint8_t i=0;
    char Num_value[4] ={ 0 };
    int value = 0;
    int  this_angle;
    Num_char[0] = uart5_buff[1];
    Num_char[1] = uart5_buff[2];

    if( Mode == 6 &&( uart5_buff[0] > '0' && uart5_buff[0] <= '9') ){
        Aim_char = uart5_buff[0];
    }else{
    if( Num_char[0] == 'S')        //串口屏停止
        Mode = 0;
    else
    {
    if( Num_char[0] == 'B'  || Num_char[0] == 'R'  )
    switch(Num_char[0])     //模式
    {
        case 'B' :{
            switch(Num_char[1])     //模式
                {
                    case '1' :{
                        Mode = 2;
                        break;
                    }
                    case '2' :{
                        Mode = 3;
                        break;
                    }
                    default:  break;
                }
            break;
        }
        case 'R' :{
            switch(Num_char[1])     //模式
                {
                    case '1' :{
                        Mode = 4;
                        UART_send_string("*X#", UART1_BASE);
                        break;
                    }
                    case '2' :{
                        Mode = 5;
                        UART_send_string("*Y#", UART1_BASE);
                        break;
                    }
                    case '3' :{
                        Mode =6;
                        UART_send_string("*Z#", UART1_BASE);
                        break;
                    }
                    default:  break;
                }
            break;
        }
        default:  break;
    }
    else{
        Num_char[0] = uart5_buff[0];
        if(Num_char[0] > '1' && Num_char[0] < '4' ){
            for(i = 0; i<3; i++){
                Num_value[i] = uart5_buff[i];
            }
            Num_value[3] = '\0';
            sscanf(Num_value, "%d", &value );
            Dis_set = value;
            if( Mode == 2){
                emi_flag = 1;
            }
        }
        if( Num_char[0] == '+' || Num_char[0] == '-'  ){
                for(i = 0; i<3; i++){
                    Num_value[i] = uart5_buff[i];
                }
                Num_value[3] = '\0';
                sscanf(Num_value, "%d", &this_angle );
                Angle =  this_angle;          //角度值C
                emi_flag =2;
        }
    }
    }
}
}

void ceshi(void)
{
    //            PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*75/1000);
    //            PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*75/1000);
        char Num_char[2];
        Num_char[0] = uart0_buff[1];
        Num_char[1] = uart0_buff[2];
        switch(Num_char[0])     //模式
        {
            case 'A' :{          //上下
                switch(Num_char[1])     //模式
                    {
                        case '+' :{
                            if(count < 1890)
                                count = 1890;
                            count = count+12;
                            if(count > 2390)
                                count = 2384;
                            PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                            uart0_buff[1] = 0;
                            uart0_buff[2] = 0;
                            break;
                        }
                        case '-' :{
                            count = count-12;
                            if(count < 1890)
                                count = 2384;
                            PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                            uart0_buff[1] = 0;
                            uart0_buff[2] = 0;
                            break;
                        }
                        default:  break;
                    }
                break;
            }
            case 'B' :{   //左右
                switch(Num_char[1])       //模式
                    {
                        case '+' :{
                            count = count+12;
                            if(count > 1787)
                                count = 1787;
                            PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                            uart0_buff[1] = 0;
                            uart0_buff[2] = 0;
                            break;
                        }
                        case '-' :{
                            count = count-12;
                            if(count > 1787)
                            count = 1787;
                            if(count < 1121)
                                count = 1121;
                            PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*count/20000);
                            uart0_buff[1] = 0;
                            uart0_buff[2] = 0;
                            break;
                        }
                        default:  break;
                    }
                break;
            }
            default:  break;
        }
}
