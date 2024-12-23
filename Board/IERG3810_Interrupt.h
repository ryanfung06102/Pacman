#ifndef __IERG3810_Interrupt_H
#define __IERG3810_Interrupt_H
#include "stm32f10x.h"

void IERG3810_NVIC_SetPriorityGroup (u8 prigroup);

void IERG3810_PS2key_ExtiInit(u8);
void IERG3810_keyUP_ExtiInit(u8);
void IERG3810_key2_ExtiInit(u8);

//void EXTI2_IRQHandler(void)
//void EXTI0_IRQHandler(void)
//void EXTI15_10_IRQHandler(void)


#endif
