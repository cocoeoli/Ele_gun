/*
 * uart.h
 *
 *  Created on: 2019Äê1ÔÂ24ÈÕ
 *      Author: xiong
 */

#ifndef UART_H_
#define UART_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_uart.h"
#include "inc/hw_udma.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include"driverlib/udma.h"
#include "utils/uartstdio.h"

#define Buff0_Len   20
#define Buff1_Len   30
#define Buff2_Len   20
#define Buff4_Len   30
#define Buff5_Len   20

void UART_send_string(const char *sendbuf, int32_t URATX);
void UART_send_char(const unsigned char Byte, int32_t URATX);
char UART_recive_char( int32_t URATX);
void UART_recive_string(char *reciveBuf,int num, int32_t URATX);
void UART_send_int16_t_string(const int16_t *temp, uint64_t num, int32_t URATX);
void UART_send_int32_t_string(const int32_t *temp, uint64_t num, int32_t URATX);
void UART_send_int64_t_string(const int64_t *temp, uint64_t num, int32_t URATX);
void UART_send_int16_t(const int16_t temp,char on, int32_t URATX);
void UART_send_int32_t(const int32_t temp,char on, int32_t URATX);
void UART_send_int64_t(const int64_t temp,char on, int32_t URATX);
void UART_send_float(const float temp,int64_t lenth, int32_t URATX);
void UART_send_char_string(const unsigned char *temp, uint64_t num, int32_t URATX);
void UART_send_int64_t_string(const int64_t *temp, uint64_t num, int32_t URATX);

//void Uart_dmainit(void);
void UART0_init(uint32_t Baud);
void UART1_Init(uint32_t ui32Baud);
//void UART2_Init(uint32_t Baud);
void UART4_Init(uint32_t Baud);
void UART5_Init(uint32_t Baud);
void UART0IntHandler(void);
void UART1IntHandler(void);
//void UART2IntHandler(void);
void UART4IntHandler(void);
void UART5IntHandler(void);
void Data1pro(void);
void Data5pro(void);


void ceshi(void);

#endif /* UART_H_ */
