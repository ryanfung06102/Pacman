#include "stm32f10x.h"


u32 sent;


void Delay(u32 count) {

	u32 i;
	for (i = 0; i < count; i++);
}





void IERG3810_clock_tree_init(void) {

	u8 PLL = 7;
	unsigned char temp = 0;
	RCC->CFGR &= 0xF8FF0000;
	RCC->CR &= 0xFEF6FFFF;
	RCC->CR |= 0x00010000;
	while (!(RCC->CR >> 17));
	//RCC -> CFGR = 0x00000400;
	RCC->CFGR = 0x00002400; //***CHNAGED***
	RCC->CFGR |= PLL << 18;
	RCC->CFGR |= 1 << 16;
	FLASH->ACR |= 0x32;
	RCC->CR |= 0x01000000;
	while (!(RCC->CR >> 25));
	RCC->CFGR |= 0x00000002;
	while (temp != 0x02) {

		temp = RCC->CFGR >> 2;
		temp &= 0x03;

	}

}



void IERG3810_USART2_init(u32 pclk1, u32 bound) {

	float temp;
	u16 mantissa;
	u16 fraction;
	temp = (float)(pclk1 * 1000000) / (bound * 16);
	mantissa = temp;
	fraction = (temp - mantissa) * 16;
	mantissa <<= 4;
	mantissa += fraction;
	RCC->APB2ENR |= 1 << 2;
	RCC->APB1ENR |= 1 << 17;
	GPIOA->CRL &= 0xFFFF00FF;
	GPIOA->CRL |= 0x00008B00;
	RCC->APB1RSTR |= 1 << 17;
	RCC->APB1RSTR &= ~(1 << 17);
	USART2->BRR = mantissa;
	USART2->CR1 |= 0x2008;

}

void IERG3810_USART1_init(u32 pclk1, u32 bound) {

	float temp;
	u16 mantissa;
	u16 fraction;
	temp = (float)(pclk1 * 1000000) / (bound * 16);
	mantissa = temp;
	fraction = (temp - mantissa) * 16;
	mantissa <<= 4;
	mantissa += fraction;
	RCC->APB2ENR |= 1 << 2;
	//RCC -> APB1ENR |= 1 << 17;
	RCC->APB2ENR |= 1 << 14;	//***CHANGED***
	//GPIOA -> CRL &= 0xFFFF00FF;
	//GPIOA -> CRL |= 0x00008B00;
	GPIOA->CRH &= 0xFFFFF00F;	//***CHANGED***
	GPIOA->CRH |= 0x000008B0;	//***CHANGED***
	//RCC -> APB1RSTR |= 1 << 17;
	//RCC -> APB1RSTR &= ~(1 << 17);
	RCC->APB2RSTR |= 0x00004000;	//***CHANGED***
	RCC->APB2RSTR &= 0xFFFFBFFF;	//***CHANGED***
	USART1->BRR = mantissa;
	//USART2 -> CR1 |= 0x2008; 
	USART1->CR1 |= 0x2008;
}






void USART_print(u8 USARTport, char* st){

	u8 i = 0;
	
	if (USARTport == 1){      //if choose USART1
		while (st[i] != 0x00){  //if not the end of a string 
			
			// (Refer to RM0008 Page.820: USART_DR)
			// => Data register
			// DR[8:0]: Data value
			// Contain the Received or Transmitted data character
			//
			// Hence, the below line is just to put each character of the
			// string into this data register [8:0].
			USART1 -> DR = st[i];
			
			// (Refer to RM0008 Page.818: USART_SR)
			// => Status register
			// Bit-7: TXE
			// Transmit data register empty
			// (0: Data is not transferred to the shift register) 
			// (1: Data is transferred to the shift register)
			// 
			// Hence, the below line is just to replace the Delay() function.
			// if ((USART1 -> SR) >> 7) == 0, then keep looping
			// else, jump out of this loop
			while (!((USART1 -> SR) >> 7));
			
			if (i == 255) break;
			i++;
		}
	}
	else if (USARTport == 2){     //if choose USART2
		while (st[i] != 0x00){      //if not the end of a string
			
			// (Refer to RM0008 Page.820: USART_DR)
			// => Data register
			// DR[8:0]: Data value
			// Contain the Received or Transmitted data character
			//
			// Hence, the below line is just to put each character of the
			// string into this data register [8:0].
			USART2-> DR = st[i];
			
			// (Refer to RM0008 Page.818: USART_SR)
			// => Status register
			// Bit-7: TXE
			// Transmit data register empty
			// (0: Data is not transferred to the shift register) 
			// (1: Data is transferred to the shift register)
			// 
			// Hence, the below line is just to replace the Delay() function.
			// if ((USART2 -> SR) >> 7) == 0, then keep looping
			// else, jump out of this loop
			while (!((USART2 -> SR) >> 7));
			
			if (i == 255) break;
			i++;
		}
	}

	
}
u32 printed;
int main(void) {

	sent = 0x0;
	printed = 0x0;
	
	IERG3810_clock_tree_init();
	IERG3810_USART1_init(36, 9600);
	
	while (1) {
	
		if (printed == 0){
			USART_print(1, "1155188577");
			printed |= 0x1;
		}
				
	}

}

