#include "stm32f10x.h"
#include "IERG3810_USART.h"

//include the USART_print()
//include the USART_print()
//include the USART_print()


// put your procedure and code here

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

void USART_print(u8 USARTport, char* st){

	u8 i = 0;
	if (USARTport == 1){
		while (st[i] != 0x00){
			USART1 -> DR = st[i];
			while (!((USART1 -> SR) >> 7));
			if (i == 255) break;
			i++;
		}
	}
	else if (USARTport == 2){
		while (st[i] != 0x00){
			USART2-> DR = st[i];
			while (!((USART2 -> SR) >> 7));
			if (i == 255) break;
			i++;
		}
	}

}
