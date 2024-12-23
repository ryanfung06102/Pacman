#include "stm32f10x.h"
#include "IERG3810_KEY.h"
#include "IERG3810_LED.h"
#include "IERG3810_Buzzer.h"
#include "IERG3810_USART.h"
#include "IERG3810_Clock.h"

void delay(u32 count){

	u32 i;
	for (i=0; i<count; i++);
}


void IERG3810_NVIC_SetPriorityGroup (u8 prigroup){

	/*Set PRIGROUP AIRCR[10:8]*/
	u32 temp, temp1;
	temp1 = prigroup & 0x00000007; //only consider 3 bits
																 //so the 0<= prigroup <=7
	temp1 <<= 8; // shift to bit [10:8]
	temp = SCB->AIRCR; //put the data into the AIRCR register
										 //DDI0337E Page.8-22
	temp &= 0x0000F8FF;//?
										 //DDI0337E Page.8-22 
	temp |= 0x05FA0000;//?
										 //DDI0337E Page.8-22
	temp |= temp1;
	SCB->AIRCR=temp;	
}






void IERG3810_keyUP_ExtiInit(void){
	
	// KEY_UP, PA0, EXTI-0, IRQ6, Rising edge
	RCC -> APB2ENR |= 1 << 2;
	GPIOA -> CRL &= 0xFFFFFFF0;
	GPIOA -> CRL |= 0x00000008;
	//GPIOA -> ODR &= 0xFFFFFFF0;
	
	RCC -> APB2ENR |= 0x01;
	// PROBLEM HERE
	AFIO -> EXTICR[0] &= 0xFFFFFFF0;
	AFIO -> EXTICR[0] |= 0x00000000;
	EXTI -> IMR |= 1;
	EXTI -> RTSR |= 1;
	
	NVIC -> IP[6] = 0x95;
	NVIC -> ISER[0] &= ~(1 << 6);
	NVIC -> ISER[0] |= 1 << 6;

}

void IERG3810_key2_ExtiInit(void){

	/*KEY2 at PE2, EXTI-2, IPQ#8*/
	//Clock
	RCC->APB2ENR |= 1 << 6;  
	// port configuration (CNF&MODE)
	// retain other configuration
	// set CNF2MODE2 1000
	// CNF:10, input with pull-up/pull-down; MODE:00, input
	GPIOE -> CRL &= 0xFFFFF0FF; 
	GPIOE -> CRL |= 0x00000800;
	// IDR || ODR || BSR || BSRR
	// Here is important! check out Table.20 (in the lab-1 manual, P.13): 
	// The ODR needs to be considered!
	//
	//
	// (although it is input mode)
	// so here is set as "pull high"
	GPIOE -> ODR |= 1 << 2;
	
	// enable AFIO clock (RM0008, Page-146)
	RCC->APB2ENR |= 0x01; 
	// (RM0008, AFIO_EXTICR1, Page-191)
	// correspond to tutorial-4 Page.19 
	AFIO -> EXTICR[0] &= 0xFFFFF0FF;
	AFIO -> EXTICR[0] |= 0x00000400;
	// (RM0008, AFIO_EXTICR1, Page-211&212)
	EXTI->IMR         |= 1<<2; //edge trigger
	EXTI->FTSR        |= 1<<2; //falling edge
	
	//DDI0337E Page.8-3
	NVIC->IP[8] = 0x65;				//set priority of this interrupt  
	NVIC->ISER[0] &= ~(1<<8); //set NVIC 'SET ENABLE REGISTER'
	NVIC->ISER[0] |=  (1<<8); //IRQ8
}	



void EXTI0_IRQHandler(void){
	
	u8 i;
	for (i = 0; i < 10; i++){
		//DS1_ON;
		GPIOE -> BRR = 1 << 5;
		delay(1000000);
		//DS1_OFF;
		GPIOE -> BSRR = 1 << 5;
		delay(1000000);
	}
	EXTI -> PR = 1;

}









//physical pin EXTI2
//EXTI0~4, all have a corresponding IRQ software
//The below function is viewed as an IRQ for EXTI2  
void EXTI2_IRQHandler(void){
	
	u8 i;
	for (i=0; i<10; i++){
		//DS0_ON;
		GPIOB -> BRR = 1 << 5;
		delay(1000000);
		//DS0_OFF;
		GPIOB -> BSRR = 1 << 5;
		delay(1000000);
	}
	
	EXTI->PR = 1<<2;
}







u32 sheep = 0;
int main(void) {
	
	IERG3810_clock_tree_init();
	IERG3810_LED_Init();
	
	IERG3810_NVIC_SetPriorityGroup(5); // set PRIGROUP
	IERG3810_key2_ExtiInit();
	IERG3810_keyUP_ExtiInit();
	
	
	
	//DS0_OFF;
	GPIOB -> BSRR = 1 << 5;
	
	while (1){
		
		/*
		USART_print(2, " --- ABCDEF ");
		delay(5000000);
		//DS1_ON; ()
		GPIOE -> BRR = 1 << 5;
		delay(5000000);
		//DS1_OFF;
		GPIOE -> BSRR = 1 << 5;
		*/
		sheep++;
		
	}
	
}






