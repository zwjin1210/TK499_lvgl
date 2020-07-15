#ifndef _UART_H_
#define _UART_H_



#include "HAL_conf.h"
#include "stdio.h"
#include "TK499_Timer.h"
#include "string.h"

void UartInit(UART_TypeDef* UARTx,int BaudRate);
void send_data(u8 data);
void send_group(u8*data,u16 len);
void send_str(char *p);
void Uart2Init(int BaudRate);
u8 Uart2_Receive(void);
void send2_data(u8 data);
void send2_group(u8*data,u16 len);
void send2_str(char *p);


#define Rx_buf_len     1024  
extern struct Receive_Ux 
{
	char Data_rx_buf[Rx_buf_len];
    int Flag;	
	int Data_count;
}Ux_rxdata;





#endif










