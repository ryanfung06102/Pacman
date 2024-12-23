#ifndef __IERG3810_Timer_H
#define __IERG3810_Timer_H
#include "stm32f10x.h"

void IERG3810_TIM3_Init(u16, u16);
void IERG3810_TIM4_Init(u16, u16);
void IERG3810_SYSTICK_Init10ms(void);

/*
void TIM4_IRQHandler(void);
void TIM3_IRQHandler(void);
*/

#endif
