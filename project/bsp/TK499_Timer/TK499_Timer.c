#include "TK499_Timer.h"
#include "stdio.h"

void TIM8_Config(u16 arr,u16 psc)
{
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
   NVIC_InitTypeDef NVIC_InitStruct;        //�жϲ����ṹ��
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM8,ENABLE); //TIM8ʱ��ʹ��
   TIM_TimeBaseStructure.TIM_Period = arr; //��װ��ֵ        
   TIM_TimeBaseStructure.TIM_Prescaler =psc; //Ԥ��Ƶֵ
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //���������ù�
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
   TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //��ʼ��
        //�ж����ȼ�NVIC���� 
   TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE ); //ʹ��TIM8�жϣ���������ж�
   NVIC_InitStruct.NVIC_IRQChannel = TIM8_IRQn;  //TIM8�ж�
   NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;  //ռ�����ȼ�0
   NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0
   NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
   NVIC_Init(&NVIC_InitStruct);  //��ʼ��NVIC�Ĵ���
        
   TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM8 
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
   NVIC_InitTypeDef NVIC_InitStruct;        //�жϲ����ṹ��
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //TIM8ʱ��ʹ��
   TIM_TimeBaseStructure.TIM_Period = arr; //��װ��ֵ        
   TIM_TimeBaseStructure.TIM_Prescaler =psc; //Ԥ��Ƶֵ
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //���������ù�
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
   TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //��ʼ��
        //�ж����ȼ�NVIC���� 
   TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��TIM8�жϣ���������ж�
   NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;  //TIM6�ж�
   NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;  //ռ�����ȼ�0
   NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�0
   NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
   NVIC_Init(&NVIC_InitStruct);  //��ʼ��NVIC�Ĵ���
        
   TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM8 
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
				TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE ); //ʧ��TIM3�жϣ��رո����ж�
			}
		}
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

void TIM10_Config(u16 arr,u16 psc)
{
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
   NVIC_InitTypeDef NVIC_InitStruct;        //�жϲ����ṹ��
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM10,ENABLE); //TIM8ʱ��ʹ��
   TIM_TimeBaseStructure.TIM_Period = arr; //��װ��ֵ        
   TIM_TimeBaseStructure.TIM_Prescaler =psc; //Ԥ��Ƶֵ
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //���������ù�
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
   TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure); //��ʼ��
        //�ж����ȼ�NVIC���� 
   TIM_ITConfig(TIM10,TIM_IT_Update,ENABLE ); //ʹ��TIM8�жϣ���������ж�
   NVIC_InitStruct.NVIC_IRQChannel = TIM10_IRQn;  //�ж�ͨ��Ϊ TIM10_IRQn
   NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;  //ռ�����ȼ�0
   NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�0
   NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
   NVIC_Init(&NVIC_InitStruct);  //��ʼ��NVIC�Ĵ���
        
   TIM_Cmd(TIM10, ENABLE);  //ʹ��TIM8 
}
void TIM10_IRQHandler(void)
{
   if (TIM_GetITStatus(TIM10, TIM_IT_Update) != RESET)//���TIM10�����жϷ������
	{	
		printf("\n TIM10_IRQn \n");
	}
	TIM_ClearITPendingBit(TIM10, TIM_IT_Update); //���TIM10�����жϱ�־ 
}










