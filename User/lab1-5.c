#include "stm32f10x.h"
#include "IERG3810_KEY.h"
#include "IERG3810_LED.h"
#include "IERG3810_Buzzer.h"
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
	
	IERG3810_Buzzer_Init();
	IERG3810_Key_Init();
	IERG3810_LED_Init();
	
	
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

