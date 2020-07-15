#include "UART.h"

void UartInit(UART_TypeDef* UARTx,int BaudRate)
{
  UART_InitTypeDef       UART_InitStructure;  
  GPIO_InitTypeDef  GPIO_InitStructure;   
	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;   //uart1_tx  pa9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���⸴�����
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;  //uart1_rx  pa10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������   

  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_PinAFConfig(GPIOA, GPIO_Pin_9 | GPIO_Pin_10, GPIO_AF_UART_1); //PA9��PA10����Ϊ����1
  GPIO_PinAFConfig(GPIOA, GPIO_Pin_15, GPIO_AF_GPIO); //PA15����Ϊ��ͨGPIO
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);
  UART_InitStructure.UART_BaudRate = BaudRate; //������
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;//����λ
  UART_InitStructure.UART_StopBits = UART_StopBits_1;//ֹͣλ
  UART_InitStructure.UART_Parity = UART_Parity_No ;
  UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;//�������ģʽ
  UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None; 	
  UART_Init(UARTx, &UART_InitStructure);
  UART_Cmd(UARTx, ENABLE);  //UART ģ��ʹ��
	
  UART_ClearITPendingBit(UARTx, 0xff); 
	
  UART_ITConfig(UARTx, UART_IT_RXIEN, ENABLE);//ʹ�ܽ����ж�
  NVIC_SetPriority(UART1_IRQn,3);
  NVIC_EnableIRQ(UART1_IRQn);
	
}
u8 Uart1_Receive(void)
{
	//�ж������Ƿ���
  while((UART1->CSR & (1<<1)) ==0);
	return UART1->RDR;

}
void UART1_IRQHandler(void)//���ڽ����жϴ���������ӡ��ASCIIֵ
{
   if(UART1->ISR & (1<<1))     
	 {
		  UART1->RDR = Uart1_Receive();
	    UART1->ICR |= 1<<1;
		   
		 
		 
		 (UART1->RDR);
	 }
}
void send_data(u8 data)
{
	while((UART1->CSR &0x1) == 0);
	UART1->TDR = data; 
}
void send_group(u8*data,u16 len)
{
	while(len--)
	send_data(*data++);
}
void send_str(char *p)
{
		while(*p != '\0')
		{
			send_data(*p);
			p++;
		}
}

//�������д��룬���ù�ѡUSE microlib,ʹ��printf

#pragma import(__use_no_semihosting)                          
struct __FILE 
{ 
int handle; 
}; 
FILE __stdout;       
void _sys_exit(int x) 
{ 
x = x; 
} 
int fputc(int ch, FILE *f)
{      
while((UART1->CSR &0x1) == 0){}
	UART1->TDR = (u8) ch;      
return ch;
}

//********************* UART2 ���� *********************//

struct Receive_Ux Ux_rxdata= {0};


void Uart2Init(int BaudRate)
{
	UART_InitTypeDef       UART_InitStructure;  
	GPIO_InitTypeDef  GPIO_InitStructure;   
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;   //uart2_tx  PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���⸴�����
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;  //uart2_rx  PA3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������   
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_Pin_2 | GPIO_Pin_3, GPIO_AF_UART_2345); //PA2��PA3����Ϊ����2
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART2, ENABLE);
	UART_InitStructure.UART_BaudRate = BaudRate; //������
	UART_InitStructure.UART_WordLength = UART_WordLength_8b;//����λ
	UART_InitStructure.UART_StopBits = UART_StopBits_1;//ֹͣλ
	UART_InitStructure.UART_Parity = UART_Parity_No ;
	UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;//�������ģʽ
	UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None; 	
	UART_Init(UART2, &UART_InitStructure);
	UART_Cmd(UART2, ENABLE);  //UART ģ��ʹ��
	
	UART_ClearITPendingBit(UART2, 0xff); 
	
	UART_ITConfig(UART2, UART_IT_RXIEN, ENABLE);//ʹ�ܽ����ж�
	
	NVIC_SetPriority(UART2_IRQn,4);
	NVIC_EnableIRQ(UART2_IRQn);
}
u8 Uart2_Receive(void)
{
	//�ж������Ƿ���
  while((UART2->CSR & (1<<1)) ==0);
	return UART2->RDR;
}

void send2_data(u8 data)
{
	while((UART2->CSR &0x1) == 0);
	UART2->TDR = data; 
}
void send2_group(u8 *data,u16 len)
{
	while(len--)
	send2_data(*data++);
}

void send2_str(char *p)
{
		while(*p != '\0')
		{
			send2_data(*p);
			p++;
		}
		send2_data('\r');
		send2_data('\n');
}

void UART2_IRQHandler(void)//���ڽ����жϴ���������ӡ��ASCIIֵ
{
	u8 ucCh;
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��TIM3�жϣ���������ж�
	if(UART2->ISR & (1<<1))
	{
		uart_time = 0;
		ucCh = UART2->RDR;
		if(Ux_rxdata.Data_count < (Rx_buf_len - 1))
			Ux_rxdata.Data_rx_buf[Ux_rxdata.Data_count ++] = ucCh;
		UART2->ICR |= 1<<1;
	}
}
