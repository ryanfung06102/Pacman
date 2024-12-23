//1155188577 BU LIU SHIUAN
#include "stm32f10x.h"

u32 sent;
void Delay(u32 count) {
	u32 i;
	for (i = 0; i < count; i++);
}












void IERG3810_clock_tree_init(void) {

	u8 PLL = 7;
	unsigned char temp = 0;  // We use 8 bits
	
	//(Refer to RM0008)
	
	// MCO:1000
	// PLL2 clock selected
	// clear [15:0] bits
	// retain other configurations
	RCC->CFGR &= 0xF8FF0000;
  // PLLON = 0, CSSON = 0, HSEON = 0	
	// PLLON: PLL enable
  // CSSON: Clock security system enable
	// HSEON: HSE clock enable
	// retain other configurations
	RCC->CR   &= 0xFEF6FFFF;
  // HSEON: 1
	// HSEON: HSE clock enable
	RCC->CR   |= 0x00010000;
	// check HSERDY
	while (!(RCC->CR >> 17));
	
	/*
	//RCC -> CFGR = 0x00000400;
	// ***CHNAGED***
	//PPRE1:100 (HCLK divided by 2)
	//PPRE2:100 (HCLK divided by 2) (due to Exp.2.2)
	RCC->CFGR = 0x00002400; 
	*/
	
	//PPRE1:100 (HCLK divided by 2)
	//the APB1 uses 36MHz
	//so the SYSCLK is 72MHz	
	//no changing required on APB2 
	//PPRE2:0xx (HCLK divided by 1) (due to Exp.2.2) 
	RCC->CFGR = 0x00000400;
	
	//PLLMUL:0111
	//0111: PLL input clock *9
	RCC->CFGR |= PLL << 18;
	//PLLSRC:1 (lock from PREDIV1 selected as PLL input clock)
	//PLL entry clock source
	RCC->CFGR |= 1 << 16;
	//set FLASH with 2 wait states
	FLASH->ACR |= 0x32;
	// PLLON = 1	
	// PLLON: PLL enable
	RCC->CR |= 0x01000000;
	// check PLLRDY
	while (!(RCC->CR >> 25));
	// SW:10 (PLL selected as system clock)
	// System clock Switch
	RCC->CFGR |= 0x00000002;
	// check SWS[1:0]
	while (temp != 0x02) {
		temp = RCC->CFGR >> 2; // check SWS[1:0] status
		temp &= 0x03; // keep last two bits
	}

}












void IERG3810_USART2_init(u32 pclk1, u32 bound) {

	//(Refer to the tutorial slides) 
	/*Below is the calculation of USARTDIV*/
	float temp;
	u16 mantissa;
	u16 fraction;
	temp = (float)(pclk1 * 1000000) / (bound * 16);
	mantissa = temp;
	fraction = (temp - mantissa) * 16;
	mantissa <<= 4;
	mantissa += fraction;
	/*Above is the calculation of USARTDIV*/
	
	//clock for GPIOA
  //set IOPA is 1
	RCC->APB2ENR |= 1 << 2;
	//USART2 clock enable
  //set bit-17 = 1 (enable)
	RCC->APB1ENR |= 1 << 17;
	//retain other configurations
	// CNF3MODE3:1000
	// input mode, input with pull-up/pull-down
	// CNF2MODE2:1011
	// output mode, ALTERNATIVE push-pull
	GPIOA->CRL &= 0xFFFF00FF;
	GPIOA->CRL |= 0x00008B00;
	// reset USART2
	RCC->APB1RSTR |= 1 << 17;
	RCC->APB1RSTR &= ~(1 << 17);
	// Baud rate register 
	// These 12 bits reserved for mantissa of USARTDIV
	// These 4  bits reserved for fraction of USARTDIV
	USART2->BRR = mantissa;
	// UE (bit-13): USART enable
	// set UE = 1 (USART enabled)
	// TE (bit-3):  Transmitter enable
	// set TE = 1 (Transmitter enabled)
	USART2->CR1 |= 0x2008;
}





void IERG3810_USART1_init(u32 pclk1, u32 bound) {

	//(Refer to the tutorial slides) 
	/*Below is the calculation of USARTDIV*/
	float temp;
	u16 mantissa;
	u16 fraction;
	temp = (float)(pclk1 * 1000000) / (bound * 16);
	mantissa = temp;
	fraction = (temp - mantissa) * 16;
	mantissa <<= 4;
	mantissa += fraction;
	/*Above is the calculation of USARTDIV*/
	
	//clock for GPIOA
  //set IOPA is 1
	RCC->APB2ENR |= 1 << 2;
	//USART1 clock enable
  //set bit-14 = 1 (enable)
	RCC->APB2ENR |= 1 << 14;	//***CHANGED***
	// retain other configurations
	// CNF10MODE10:1000
	// input mode, input with pull-up/pull-down
	// CNF9MODE9:1011
	// output mode, ALTERNATIVE push-pull
	GPIOA->CRH &= 0xFFFFF00F;	//***CHANGED***
	GPIOA->CRH |= 0x000008B0;	//***CHANGED***
	// reset USART1
	RCC->APB2RSTR |= 0x00004000;	//***CHANGED***
	RCC->APB2RSTR &= 0xFFFFBFFF;	//***CHANGED***
	// Baud rate register 
	// These 12 bits reserved for mantissa of USARTDIV
	// These 4  bits reserved for fraction of USARTDIV
	USART1->BRR = mantissa;
	// UE (bit-13): USART enable
	// set UE = 1 (USART enabled)
	// TE (bit-3):  Transmitter enable
	// set TE = 1 (Transmitter enabled)
	USART1->CR1 |= 0x2008;
}




void Wait(void) {
	while (!((USART1->SR) >> 7));
}





int main(void) {

	sent = 0x0;
	IERG3810_clock_tree_init();
	//USART1
	//72MHz
	IERG3810_USART1_init(72, 9600); 
	
	Delay(1000000);
	
	while (1) {
	
		if (sent == 0){
			USART1 -> DR = 0x31;
			Delay(1000000);
			USART1 -> DR = 0x31;
			Delay(1000000);
			USART1 -> DR = 0x35;
			Delay(1000000);
			USART1 -> DR = 0x35;
			Delay(1000000);
			USART1 -> DR = 0x31;
			Delay(1000000);
			USART1 -> DR = 0x38;
			Delay(1000000);
			USART1 -> DR = 0x38;
			Delay(1000000);
			USART1 -> DR = 0x35;
			Delay(1000000);
			USART1 -> DR = 0x37;
			Delay(1000000);
			USART1 -> DR = 0x37;
			sent |= 0x1;
		}
		
	}

}



