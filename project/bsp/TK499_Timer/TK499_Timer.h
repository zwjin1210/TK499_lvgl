#ifndef _TIME_H_
#define _TIME_H_

#include "HAL_conf.h"
#include "UART.h"
void TIM8_Config(u16 arr,u16 psc);
void TIM3_Config(u16 arr,u16 psc);
void TIM10_Config(u16 arr,u16 psc);

extern vu32 runtime;
extern vu32 uart_time;
void Delay_nms(u32 time);

#endif
