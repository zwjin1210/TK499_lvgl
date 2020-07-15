/****************************************Copyright (c)****************************************************
** 
**                                      
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			main.c
** modified Date:  		2019-8-12
** Last Version:		V0.1
** Descriptions:		  main ��������
** Author : xiao chen
** Historical Version :
** ������Ƽ���оƬ��ҵ��----��������Ӧ�÷ֲ�
*********************************************************************************************************/
#include "main.h"
#include "LCD.h"
#include "SPI.h"
#include "UART.h"
#include "test.h"
#include "esp8266.h"
#include "MM_T035.h"
/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� ����
**������� ����
**������� ����
********************************************************************************************************/

int main(void)
{	
	int i;
  RemapVtorTable();
	SystemClk_HSEInit(RCC_PLLMul_20);//����PLLʱ�ӣ�12MHz*20=240MHz
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//2��2��ȫ���Ժ�������������һ��
	UartInit(UART1,460800);      //���ô���1��������Ϊ460800
	printf(" Welcome to use HJR TK499! \r\n");
	LED_Init();
	TIM8_Config(1000,192);          //���ö�ʱ��8
	TIM3_Config(1000,192);
	
	LCD_Initial();               //LCD��ʼ������
	
	Lcd_ColorBox(0,0,XSIZE_PHYS,YSIZE_PHYS,Blue);//����ɫ����
	Lcd_ColorBox(400,10,50,50,Red);//д��ɫ����
	printf(" Welcome to use HJR TK499! \r\n");
	
	LCD_PutString_GB_n(60,3,"��ӭ���٣�",White,Black,0,48);//48������
	LCD_PutString(10,60,"Welcome to use HJR TK499 and RGB LCD!",Red,Yellow,1);
	LCD_PutString(10,80,"�����к�����Ƽ����޹�˾",Red,Yellow,1);
	LCD_PutString(10,100,"�绰��0755-33561760",Red,Yellow,1);
	LCD_PutString(10,120," 3.2�� TK032F8004 Һ���� ",Red,Yellow,1);
	LCD_PutString(100,170," Smart IoT ESP8266 WIFI Test ",Red,Yellow,0);
	LCD_Fill_Pic(400,0,320,480,(u32*)gImage_MM_T035);

	ESP8266_Init();
	printf("��ӭ����������WIFI����������!\n");	
	ESP8266_StaTcpClient_UnvarnishTest();
	while(1)
	{
	}
}



