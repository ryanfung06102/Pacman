#include "stm32f10x.h"
#include "IERG3810_KEY.h"
#include "IERG3810_LED.h"
#include "IERG3810_Buzzer.h"
#include "IERG3810_USART.h"
#include "IERG3810_Clock.h"
//155188577 Bu Liu shiuan

void delay(u32 count){
	
	u32 i;
	for (i=0; i<count; i++);
}

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
	// input mode setting
	// "pull high"
	GPIOE -> ODR |= 1 << 2;
	
	// APB2 peripheral clock enable register
	// bit[0]: Alternate function I/O clock enable (RM0008, Page-146)
	RCC->APB2ENR |= 0x01; 
	//  bit[11:8] is EXTI2, and write 0100 is to select PEx pin
	// (RM0008, AFIO_EXTICR1, Page-191, correspond to tutorial-4 Page.19)
	AFIO -> EXTICR[0] &= 0xFFFFF0FF;
	AFIO -> EXTICR[0] |= 0x00000400;
	// Interrupt mask register
	// set bit[2] as 1: Interrupt request from Line 2 is not masked
	// (RM0008, AFIO_EXTICR1, Page-211&212)
	EXTI->IMR         |= 1<<2; //edge trigger
	EXTI->FTSR        |= 1<<2; //falling edge
	// set priority of this interrupt  
	// (DDI0337E Page.8-3)
	NVIC->IP[8] = 0x65;
	// set NVIC 'SET ENABLE REGISTER'
	// IRQ8
	// (DDI0337E Page.8-3)
	NVIC->ISER[0] &= ~(1<<8); 
	NVIC->ISER[0] |=  (1<<8); 
}	

//physical pin EXTI2
//EXTI0~4, all have a same corresponding IRQ software
//The below function is viewed as an IRQ for EXTI2  
void EXTI2_IRQHandler(void){
	
	u8 i;
	for (i=0; i<10; i++){
		
		GPIOB -> BRR = 1 << 5;
		delay(1000000);
		GPIOB -> BSRR = 1 << 5;
		delay(1000000);
	}
	
	// Exception pending register
	// This bit is cleared by writing a '1'into the bit. 
	EXTI->PR = 1<<2;
}

u32 sheep = 0;
int main(void) {
	
	IERG3810_clock_tree_init();
	IERG3810_LED_Init();
	
	// set PRIGROUP as 5
	// We have 16 priority levels in this lab
	// set PRIGROUP == 5: 4 groups,
	//										each group 4 subpriority levels
	IERG3810_NVIC_SetPriorityGroup(5);  
	IERG3810_key2_ExtiInit();
	
	GPIOB -> BSRR = 1 << 5;
	GPIOE -> BSRR = 1 << 5;
	while (1){
		
		sheep++;
		GPIOE -> BRR = 1 << 5;
		delay(1000000);
		GPIOE -> BSRR = 1 << 5;
		delay(1000000);
	}
	
}
