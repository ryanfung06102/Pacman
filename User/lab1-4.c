#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
//1155188577 BU LIU SHIUAN

void Delay(u32 count){

	u32 i;
	for (i=0; i<count; i++);
}

//initialize some global variables
u32 key1;
u32 key2;
u32 key_up;

u32 DS1_on;
u32 check_DS1;
u32 buzzer_on;
u32 check_buzzer;

int main(void){
	
	//The initialization procedure:
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
	
	
	// initial state for DS1 (the second LED)
	DS1_on &= 0x00000000;
	check_DS1  &= 0x00000000; 
	
	// initial state for buzzer
	buzzer_on &= 0x00000000;
	check_buzzer  &= 0x00000000; 
	

	while(1){
		
		//read inputs
		key2 = GPIOE -> IDR & (1 << 2);
		key1 = GPIOE -> IDR & (1 << 3);  
		key_up = GPIOA -> IDR & 1;
		

		/* 
		DS1 (the second LED) 
		aim: KEY1 toggles the LED DS1 on/off
		*/
		if (key1 != 0){              //if: the button is not pressed, we retain the original state(the value of DS1_on) 
																 //(pull-up, so not pressing down is high)			
			
			check_DS1 &= 0x00000000;   // set check_DS1 back to 0, making the toggle availble
			
			if (DS1_on == 0)           //on is still on   
				GPIOE -> BSRR = 1 << 5; 
			else 											 //off is still off
				GPIOE -> BRR = 1 << 5; 
		}
		
		
		else {                       //else if: the button is pressed, we do the toggle          
			
			if (DS1_on == 1 && check_DS1 == 0)           //the LED is currently on, 
																								   //and has not been toggled (compared to the previous state)  
				DS1_on &= 0x00000000;											 //set the value of DS1_on 0 (off)
			
			else if (DS1_on == 0 && check_DS1 == 0)			 //the LED is currently off,
																									 //and has not been toggled (compared to the previous state) 
				DS1_on |= 0x00000001;                      //set the value of DS1_on 1 (on)
				
			
			check_DS1 |= 0x00000001;         // set check_DS1 to 1, making the toggle unavailble (preventing multiple toggles)
		}
		
		
		
		/* 
		DS0 (the first LED) 
		aim: LED DS0 lit when KEY2 is pressed
		*/
		if (key2 == 0)           //if: the button is pressed
														 //(pull-up, so not pressing down is high)	
			GPIOB -> BRR = 1 << 5; //light-on, since LED is active-low
		else 
			GPIOB -> BSRR = 1 << 5; //light-off
		
		
		
		/* 
		buzzer 
		aim: KEY_UP toggles the buzzer on/off
		*/
		if (key_up == 0){            //if: the button is not pressed, we retain the original state(the value of buzzer_on) 
																 //(pull-down, so not pressing down is low)	
			
			check_buzzer &= 0x00000000;// set check_buzzer back to 0, making the toggle availble       
			
			if (buzzer_on == 0)      						//off is still off         
				GPIOB -> BRR = 1 << 8; 
			else 														   	//on is still on 
				GPIOB -> BSRR = 1 << 8; 
		}
		
		
		else {                       //else if: the button is pressed, we do the toggle           
			if (buzzer_on == 1 && check_buzzer == 0)     //the buzzer is currently on, 
																								   //and has not been toggled (compared to the previous state) 
				buzzer_on &= 0x00000000;                   //set the value of buzzer_on 0 (off)
			
			
			else if (buzzer_on == 0 && check_buzzer == 0)//the buzzer is currently off, 
																								   //and has not been toggled (compared to the previous state) 
				buzzer_on |= 0x00000001;                   //set the value of buzzer_on 1 (on)
	
			
			check_buzzer |= 0x00000001;    // set check_buzzer to 1, making the toggle unavailble (preventing multiple toggles)
		}
		
		
	}
	
}



