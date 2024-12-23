//1155188577 BU LIU SHIUAN
#include "stm32f10x.h"
#include "IERG3810_USART.h" //include the USART_print()
#include "IERG3810_Clock.h"

u32 printed;

int main(void) {

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


