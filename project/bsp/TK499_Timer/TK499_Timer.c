#include "TK499_Timer.h"
#include "stdio.h"

void TIM8_Config(u16 arr,u16 psc)
{
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
   NVIC_InitTypeDef NVIC_InitStruct;        //中断参数结构体
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM8,ENABLE); //TIM8时钟使能
   TIM_TimeBaseStructure.TIM_Period = arr; //重装载值        
   TIM_TimeBaseStructure.TIM_Prescaler =psc; //预分频值
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //不懂，不用管
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
   TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //初始化
        //中断优先级NVIC设置 
   TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE ); //使能TIM8中断，允许更新中断
   NVIC_InitStruct.NVIC_IRQChannel = TIM8_IRQn;  //TIM8中断
   NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;  //占先优先级0
   NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;  //次优先级0
   NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
   NVIC_Init(&NVIC_InitStruct);  //初始化NVIC寄存器
        
   TIM_Cmd(TIM8, ENABLE);  //使能TIM8 
}
vu32 runtime;

void TIM8_IRQHandler(void)
{
   if((TIM8->SR & (1<<0)) == 1)
	{	
		runtime ++;
	}
	TIM8->SR &=~(1<<0);
}

void Delay_nms(u32 time)
{
  u32 P_time = runtime;
	while((P_time + time) > runtime);
}

void TIM3_Config(u16 arr,u16 psc)
{
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
   NVIC_InitTypeDef NVIC_InitStruct;        //中断参数结构体
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //TIM8时钟使能
   TIM_TimeBaseStructure.TIM_Period = arr; //重装载值        
   TIM_TimeBaseStructure.TIM_Prescaler =psc; //预分频值
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //不懂，不用管
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
   TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //初始化
        //中断优先级NVIC设置 
   TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能TIM8中断，允许更新中断
   NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;  //TIM6中断
   NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;  //占先优先级0
   NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;  //次优先级0
   NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
   NVIC_Init(&NVIC_InitStruct);  //初始化NVIC寄存器
        
   TIM_Cmd(TIM3, ENABLE);  //使能TIM8 
}

vu32 uart_time = 0;
void TIM3_IRQHandler(void) 
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		{
			uart_time ++;
			if(uart_time > 10)
			{
				uart_time = 0;
				Ux_rxdata.Flag = 1;
				TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE ); //失能TIM3中断，关闭更新中断
			}
		}
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

void TIM10_Config(u16 arr,u16 psc)
{
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
   NVIC_InitTypeDef NVIC_InitStruct;        //中断参数结构体
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM10,ENABLE); //TIM8时钟使能
   TIM_TimeBaseStructure.TIM_Period = arr; //重装载值        
   TIM_TimeBaseStructure.TIM_Prescaler =psc; //预分频值
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //不懂，不用管
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
   TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure); //初始化
        //中断优先级NVIC设置 
   TIM_ITConfig(TIM10,TIM_IT_Update,ENABLE ); //使能TIM8中断，允许更新中断
   NVIC_InitStruct.NVIC_IRQChannel = TIM10_IRQn;  //中断通道为 TIM10_IRQn
   NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;  //占先优先级0
   NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;  //次优先级0
   NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
   NVIC_Init(&NVIC_InitStruct);  //初始化NVIC寄存器
        
   TIM_Cmd(TIM10, ENABLE);  //使能TIM8 
}
void TIM10_IRQHandler(void)
{
   if (TIM_GetITStatus(TIM10, TIM_IT_Update) != RESET)//检查TIM10更新中断发生与否
	{	
		printf("\n TIM10_IRQn \n");
	}
	TIM_ClearITPendingBit(TIM10, TIM_IT_Update); //清除TIM10更新中断标志 
}










