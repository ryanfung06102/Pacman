#include "stm32f10x.h"
#include "IERG3810_LED.h"

// put your procedure and code here

void IERG3810_LED_Init(){
	
	// The initialization procedure:
  // 1. enable the corresponding clock
  // 2. port configuration (CNF&MODE)
  // 3. IDR || ODR || BSR || BSRR  
	
	
	/* PB5 for DS0 (the first LED) */
	// 1.clock for GPIOB
	// retain other configuration 
	// set IOPB is 1
	RCC -> APB2ENR &= 0xFFFFFFF7;
	RCC -> APB2ENR |= 0x00000008;
	
	// 2. port configuration (CNF&MODE)
	// retain other configuration
  // set CNF5MODE5 0011
  // CNF:00, general push-pull; MODE:11, output 50 MHz	
	GPIOB -> CRL &= 0xFF0FFFFF;
	GPIOB -> CRL |= 0x00300000;

	// 3. IDR || ODR || BSR || BSRR
  // set BS5 is 1    
  // 	
	// ("the bit of ODR" will be equivalent to "the bit of BSRR")
  // (BSRR is write only; ODR is write/read only)
  //
	// Also, since the LED is active-low, so the default is light-off	
	GPIOB -> BSRR = 1 << 5;
	
	
	/* PE5 for DS1 (the second LED) */
	// 1.clock for GPIOE
	// retain other configuration
	// set IOPE is 1
	RCC -> APB2ENR &= 0xFFFFFFBF;
	RCC -> APB2ENR |= 0x00000040;
	
	// 2. port configuration (CNF&MODE)
	// retain other configuration
	// set CNF5MODE5 0011
	// CNF:00, general push-pull; MODE:11, output 50 MHz
	GPIOE -> CRL &= 0xFF0FFFFF;
	GPIOE -> CRL |= 0x00300000;
	
	// 3. IDR || ODR || BSR || BSRR
  // set BS5 is 1    
  // 	
	// ("the bit of ODR" will be equivalent to "the bit of BSRR")
  // (BSRR is write only; ODR is write/read only)
  //
	// Also, since the LED is active-low, so the default is light-off	
	GPIOE -> BSRR = 1 << 5;
}

void DS0_ON(){
	
	GPIOB -> ODR ^= 1 << 5;
	
}
