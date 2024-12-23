#include "stm32f10x.h"
#include "IERG3810_KEY.h"

// put your procedure and code here


void IERG3810_Key_Init(){
	
  // The initialization procedure:
  // 1. enable the corresponding clock
  // 2. port configuration (CNF&MODE)
  // 3. IDR || ODR || BSR || BSRR  
	
	
/* PE3 for KEY1 */
	// 1.clock for GPIOE
	// retain other configuration
	// set IOPE is 1
	RCC -> APB2ENR &= 0xFFFFFFBF;
	RCC -> APB2ENR |= 0x00000040;
	
	// 2. port configuration (CNF&MODE)
	// retain other configuration
	// set CNF3MODE3 1000
	// CNF:10, input with pull-up/pull-down; MODE:00, input
  // check 3. !
	GPIOE -> CRL &= 0xFFFF0FFF;
	GPIOE -> CRL |= 0x00008000;
	
	// 3. IDR || ODR || BSR || BSRR
	// Here is important! check out Table.20 (in the lab-1 manual, P.13): The ODR needs to be considered!
	// (although it is input mode)
	// pull-up setting (no press is high)
	GPIOE -> ODR &= 0xFFFFFFF7;
	GPIOE -> ODR |= 0x00000008;
	
	
	/* PE2 for KEY2 */
  // 1.clock for GPIOE
	// retain other configuration
	// set IOPE is 1
	RCC -> APB2ENR &= 0xFFFFFFBF;
	RCC -> APB2ENR |= 0x00000040;
	
	// 2. port configuration (CNF&MODE)
	// retain other configuration
	// set CNF2MODE2 1000
	// CNF:10, input with pull-up/pull-down; MODE:00, input
  // check 3. !
	GPIOE -> CRL &= 0xFFFFF0FF;
	GPIOE -> CRL |= 0x00000800;
	
	// 3. IDR || ODR || BSR || BSRR
	// Here is important! check out Table.20 (in the lab-1 manual, P.13): The ODR needs to be considered!
	// (although it is input mode)
	// pull-up setting (no press is high)
	GPIOE -> ODR &= 0xFFFFFFF0B;
	GPIOE -> ODR |= 0x000000004;
	
	
	/* PA0 for KEY_UP */
	// 1.clock for GPIOA
	// retain other configuration
	// set IOPA is 1
	RCC -> APB2ENR &= 0xFFFFFFFB;
	RCC -> APB2ENR |= 0x00000004;
	
	// 2. port configuration (CNF&MODE)
	// retain other configuration
	// set CNF0MODE0 1000
	// CNF:10, input with pull-up/pull-down; MODE:00, input
  // check 3. !
	GPIOA -> CRL &= 0xFFFFFFF0;
	GPIOA -> CRL |= 0x00000008;
	
	// 3. IDR || ODR || BSR || BSRR
	// Here is important! check out Table.20 (in the lab-1 manual, P.13): The ODR needs to be considered!
	// (although it is input mode)
	// pull-down setting (no press is low)
	// Hence, no setting required
}
