/****************************************Copyright (c)****************************************************
** 
**                                      
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			main.c
** modified Date:  		2019-8-12
** Last Version:		V0.1
** Descriptions:		  main 函数调用
** Author : xiao chen
** Historical Version :
** 好钜润科技，芯片事业部----深圳龙华应用分部
*********************************************************************************************************/
#include "main.h"
#include "LCD.h"
#include "SPI.h"
#include "UART.h"
#include "test.h"
#include "esp8266.h"
#include "MM_T035.h"
/********************************************************************************************************
**函数信息 ：int main (void)                          
**功能描述 ：无
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/

int main(void)
{	
	int i;
  RemapVtorTable();
	SystemClk_HSEInit(RCC_PLLMul_20);//启动PLL时钟，12MHz*20=240MHz
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//2：2，全局性函数，仅需设置一次
	UartInit(UART1,460800);      //配置串口1，波特率为460800
	printf(" Welcome to use HJR TK499! \r\n");
	LED_Init();
	TIM8_Config(1000,192);          //配置定时器8
	TIM3_Config(1000,192);
	
	LCD_Initial();               //LCD初始化函数
	
	Lcd_ColorBox(0,0,XSIZE_PHYS,YSIZE_PHYS,Blue);//用蓝色清屏
	Lcd_ColorBox(400,10,50,50,Red);//写红色方块
	printf(" Welcome to use HJR TK499! \r\n");
	
	LCD_PutString_GB_n(60,3,"欢迎光临！",White,Black,0,48);//48号字体
	LCD_PutString(10,60,"Welcome to use HJR TK499 and RGB LCD!",Red,Yellow,1);
	LCD_PutString(10,80,"深圳市好钜润科技有限公司",Red,Yellow,1);
	LCD_PutString(10,100,"电话：0755-33561760",Red,Yellow,1);
	LCD_PutString(10,120," 3.2寸 TK032F8004 液晶屏 ",Red,Yellow,1);
	LCD_PutString(100,170," Smart IoT ESP8266 WIFI Test ",Red,Yellow,0);
	LCD_Fill_Pic(400,0,320,480,(u32*)gImage_MM_T035);

	ESP8266_Init();
	printf("欢迎来到好钜润WIFI物联网测试!\n");	
	ESP8266_StaTcpClient_UnvarnishTest();
	while(1)
	{
	}
}



