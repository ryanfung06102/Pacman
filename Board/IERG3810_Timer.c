#include "stm32f10x.h"


void IERG3810_TIM3_Init(u16 arr, u16 psc){
	
	RCC->APB1ENR |= 1<<1;	//TIM3 enable					
	
	TIM3->ARR=arr; //counter
	TIM3->PSC=psc; //pre-scalar
	
	TIM3->DIER |= 1<<0; //enable TIM3 interrupt
	TIM3->CR1 |=  0x01; //counter of TIM3 enable
	
	NVIC->IP[29] = 0x45;   //need to search the corresponding IRQ from vector table!!!
	NVIC->ISER[0] |= (1<<29);   //interrupt enable(ISER)   
}


void IERG3810_TIM4_Init(u16 arr, u16 psc){
	
	RCC->APB1ENR |= 1<<2;	//TIM4 enable					
	
	TIM4->ARR=arr; //counter
	TIM4->PSC=psc; //pre-scalar
	
	TIM4->DIER |= 1<<0; //enable TIM4 interrupt
	TIM4->CR1 |=  0x01; //counter of TIM4 enable
	
	NVIC->IP[30] = 0x45; //need to search the corresponding IRQ from vector table!!!
	NVIC->ISER[0] |= (1<<30);     //interrupt enable(ISER)   
}

void IERG3810_SYSTICK_Init10ms(void)
{
    // SYSTICK
    SysTick->CTRL = 0;       // clear
    SysTick->LOAD = 719999;     //change
    SysTick->VAL=0;                         // Refer to 0337E page 8-10.
    // CLKSOURCE=0: STCLK (FCLK/8)
    // CLKSOURCE=1: FCLK/8
    // set CLKSOURCE=0 is synchronized and better than CLKSOURCE=1
    // Refer to Clock tree on RM0008 page-93.
    SysTick->CTRL |= 0x07;    // What should be filled?
                             // set internal clock, use interrupt, start count
}




/*
void TIM4_IRQHandler(void)
{
    if (TIM4->SR & 1 << 0)     // check UIF, RM0008 page-410
        GPIOE->ODR ^= 1 << 5;  // toggle DS1 with read-modify-write
    TIM4->SR &= ~(1 << 0);     // clear UIF, RM0008 page-410
}

void TIM3_IRQHandler(void){
	
	if(TIM3->SR & 1<<0)
		GPIOB->ODR ^= 1<<5;
	TIM3->SR &= ~(1<<0);
}
*/


