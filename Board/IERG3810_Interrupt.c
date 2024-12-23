#include "stm32f10x.h"

void IERG3810_NVIC_SetPriorityGroup (u8 prigroup){

	/*Set PRIGROUP AIRCR[10:8] (DDI0337E Page.8-22)*/
	
	// put the in put PRIGROUP into temp1
	u32 temp, temp1;
	temp1 = prigroup & 0x00000007; 
  // shift to bit [10:8]	
	temp1 <<= 8; 
	
	// get current AIRCR value into temp
	temp = SCB->AIRCR; 	
	// clear bit [8:10] 	
	temp &= 0x0000F8FF; 
	// [31:16] VECTKEY
	// Writing to this register requires 0x5FA in the VECTKEY field. 
	// Otherwise, the write value is ignored.
	temp |= 0x05FA0000; 
	
	//put temp1 value into temp
  //finally, put temp value into AIRCR	
	temp |= temp1;
	SCB->AIRCR=temp;	
}

void IERG3810_PS2key_ExtiInit(u8 priority){
	
	/*PC11, EXTI11, IRQ40*/
	
	//Clock
	RCC->APB2ENR |= 1 << 4;  
	// port configuration (CNF&MODE)
	// retain other configuration
	// set CNF11MODE11 1000
	// CNF:10, input with pull-up/pull-down; MODE:00, input
	GPIOC -> CRH &= 0xFFFF0FFF;
	GPIOC -> CRH |= 0x00008000;
	// input mode setting
	// "pull high"
	GPIOC -> ODR |= 1 << 11;

	// APB2 peripheral clock enable register
	// bit[0]: Alternate function I/O clock enable (RM0008, Page-146)
	RCC->APB2ENR |= 0x01; 
	//  EXTICR[2], bit[15:12] is EXTI11, and write 0010 is to select PCx pin
	// (RM0008, AFIO_EXTICR1, Page-191, correspond to tutorial-4 Page.19)
	AFIO -> EXTICR[2] &= 0xFFFF0FFF;
	AFIO -> EXTICR[2] |= 0x00002000;
	// Interrupt mask register
	// set bit[11] as 1: Interrupt request from Line 11 is not masked
	// (RM0008, AFIO_EXTICR1, Page-211&212)
	EXTI->IMR         |= 1<<11; //edge trigger
	EXTI->FTSR        |= 1<<11; //falling edge
	// set priority of this interrupt  
	// (DDI0337E Page.8-3)
	NVIC->IP[40] = priority;
	// set NVIC 'SET ENABLE REGISTER'
	// IRQ40
	// (DDI0337E Page.8-3)
	NVIC->ISER[1] &= ~(1<<8); 
	NVIC->ISER[1] |=  (1<<8); 
}

void IERG3810_keyUP_ExtiInit(u8 priority){
	
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
	
	NVIC -> IP[6] = priority;
	NVIC -> ISER[0] &= ~(1 << 6);
	NVIC -> ISER[0] |= 1 << 6;

}

void IERG3810_key2_ExtiInit(u8 priority){

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
	NVIC->IP[8] = priority;				//set priority of this interrupt  
	NVIC->ISER[0] &= ~(1<<8); //set NVIC 'SET ENABLE REGISTER'
	NVIC->ISER[0] |=  (1<<8); //IRQ8
}	

/*
//physical pin EXTI2
//EXTI0~4, all have a corresponding IRQ software
//The below function is viewed as an IRQ for EXTI2  
void EXTI2_IRQHandler(void){
	//...
	//EXTI->PR = 1<<2;
}
*/


/*
void EXTI0_IRQHandler(void){
	//...
	//EXTI -> PR = 1;
}
*/


/*
void EXTI15_10_IRQHandler(void){
	//...
	EXTI -> PR = 1 << 11;
}
*/
