#include "stm32f10x.h"
#include "IERG3810_Buzzer.h"

// put your procedure and code here

void IERG3810_Buzzer_Init(){
	
	// The initialization procedure:
  // 1. enable the corresponding clock
  // 2. port configuration (CNF&MODE)
  // 3. IDR || ODR || BSR || BSRR  
	
	/* PB8 for Buzzer */	
	// 1.clock for GPIOB
	// retain other configuration
  // set IOPB is 1	
	RCC -> APB2ENR &= 0xFFFFFFF7;
	RCC -> APB2ENR |= 0x00000008;
	
	// 2. port configuration (CNF&MODE)
	// retain other configuration
  // set CNF8MODE8 0011
  // CNF:00, general push-pull; MODE:11, output 50 MHz	
	GPIOB -> CRH &= 0xFFFFFFF0;
	GPIOB -> CRH |= 0x00000003;
	
	// 3. IDR || ODR || BSR || BSRR
	// The buzzer is active-high, so we don't need to do any default setting here
}
