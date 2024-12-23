#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
//1155188577 BU LIU SHIUAN

void Delay(u32 count){

	u32 i;
	for (i=0; i<count; i++);
}

int main(void){

	//initialize two structures: GPIO_InitStructure_PB5(for DS0) & 
	//													 GPIO_InitStructure_PE2(for KEY2) 
	GPIO_InitTypeDef GPIO_InitStructure_PB5;
	GPIO_InitTypeDef GPIO_InitStructure_PE2;
	
	// 1. enable the clock of GPIOB
	// 2. pin is 5
	// 3. mode is push-pull output 
	// 4. speed is 50MHz
	// 5. initialization 
	// 6. BSRR: the default LED is off
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure_PB5.GPIO_Pin   = GPIO_Pin_5;
	GPIO_InitStructure_PB5.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure_PB5.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, & GPIO_InitStructure_PB5);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	
	// 1. enable the clock of GPIOE
	// 2. pin is 2
	// 3. mode is 'internal pull-up' input: GPIO_Mode_IPU
	// 4. speed is 50MHz
	// 5. initialization 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	GPIO_InitStructure_PE2.GPIO_Pin   = GPIO_Pin_2;
	GPIO_InitStructure_PE2.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStructure_PE2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, & GPIO_InitStructure_PE2);
	
	
	while(1){
		
		if (!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2))  //if KEY2 is pressed
				GPIO_ResetBits(GPIOB, GPIO_Pin_5);					//turn on the LED			
		else 
				GPIO_SetBits(GPIOB, GPIO_Pin_5);						//trun off the LED	
	}
	
	
}

