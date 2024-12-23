//1155188577 BU LIU SHIUAN
#include "stm32f10x.h"
#include "IERG3810_KEY.h"
#include "IERG3810_LED.h"
#include "IERG3810_Buzzer.h"
#include "IERG3810_USART.h"
#include "IERG3810_Clock.h"
#include "FONT.h"

u16 black = 0x0000;
u16 white = 0xFFFF;
u16 green = 0x07E0;
u16 red = 0xF800;
u16 blue = 0x001F;
u16 x, y;
u16 colors[5] = {0x0000, 0xFFFF, 0x07E0, 0xF800, 0x001F};

void Delay(u32 count){
	u32 i;
	for (i=0; i<count; i++);
}

typedef struct{
	
	u16 LCD_REG;
	u16 LCD_RAM;

} LCD_TypeDef;

#define LCD_BASE ((u32) (0x6C000000 | 0x000007FE))
#define LCD ((LCD_TypeDef*) LCD_BASE)

/*only the below two functions will be utilized in the following exp.*/
void IERG3810_TFTLCD_WrReg(u16 regval){ //write a command to LCD
	LCD -> LCD_REG = regval;
}
void IERG3810_TFTLCD_WrData(u16 data){ //write a data to LCD
	LCD -> LCD_RAM = data;
}





/*Mine LCD is 9341*/
void LCD_Set9341_Parameter(void){ //set TFT-LCD 

	IERG3810_TFTLCD_WrReg(0x01); //software reset
	IERG3810_TFTLCD_WrReg(0x11); //Exit_sleep_mode

	IERG3810_TFTLCD_WrReg(0x3A);  //Set_pixel_format 
	IERG3810_TFTLCD_WrData(0x55); //65536 colors

	IERG3810_TFTLCD_WrReg(0x29); //display ON

	IERG3810_TFTLCD_WrReg(0x36);  //memory access control
	IERG3810_TFTLCD_WrData(0xC8); //control display direction
}


void IERG3810_TFTLCD_Init(void) { //set FSMC

	/*configure pin function*/
	RCC -> AHBENR  |= 1 << 8; //FSMC
	RCC -> APB2ENR |= 1 << 3; //PORTB
	RCC -> APB2ENR |= 1 << 5; //PORTD
	RCC -> APB2ENR |= 1 << 6; //PORTE
	RCC -> APB2ENR |= 1 << 8; //PORTG
	GPIOB -> CRL &= 0xFFFFFFF0; //PB0
	GPIOB -> CRL |= 0x00000003;

	//PORTD
	GPIOD -> CRH &= 0x00FFF000;
	GPIOD -> CRH |= 0xBB000BBB;
	GPIOD -> CRL &= 0xFF00FF00;
	GPIOD -> CRL |= 0x00BB00BB;
	//PORTE
	GPIOE -> CRH &= 0x00000000;
	GPIOE -> CRH |= 0xBBBBBBBB;
	GPIOE -> CRL &= 0x0FFFFFFF;
	GPIOE -> CRL |= 0xB0000000;
	//PORTG12
	GPIOG -> CRH &= 0xFFF0FFFF;
	GPIOG -> CRH |= 0x000B0000;
	GPIOG -> CRL &= 0xFFFFFFF0; //PG0->RS
	GPIOG -> CRL |= 0x0000000B;
	/*configure pin function*/
	
	/*Configure FSMC*/
  //LCD uses FSMC Bank 4 memory bank
	//use mode A
	FSMC_Bank1  -> BTCR[6]  = 0x00000000;  //FSMC_BCR4  (reset)
	FSMC_Bank1  -> BTCR[7]  = 0x00000000;  //FSMC_BTR4  (reset) 
	FSMC_Bank1E -> BWTR[6]  = 0x00000000;  //FSMC_BWTR4 (reset) 
	FSMC_Bank1  -> BTCR[6] |= 1 << 12;     //FSMC_BCR4-> WREN
	FSMC_Bank1  -> BTCR[6] |= 1 << 14;     //FSMC_BCR4-> EXTMOD
	FSMC_Bank1  -> BTCR[6] |= 1 << 4;      //FSMC_BCR4-> MWID
	FSMC_Bank1  -> BTCR[7] |= 0 << 28;     //FSMC_BTR4-> ACCMOD
	FSMC_Bank1  -> BTCR[7] |= 1 << 0;      //FSMC_BTR4-> ADDSET
	FSMC_Bank1  -> BTCR[7] |= 0xF << 8;    //FSMC_BTR4-> DATAST
	FSMC_Bank1E -> BWTR[6] |= 0 << 28;     //FSMC_BWTR4-> ACCMOD
	FSMC_Bank1E -> BWTR[6] |= 0 << 0;      //FSMC_BWTR4-> ADDSET
	FSMC_Bank1E -> BWTR[6] |= 3 << 8;      //FSMC_BWTR4-> DATAST
	FSMC_Bank1  -> BTCR[6] |= 1 << 0;      //FSMC_BCR4-> FACCEN
	
	LCD_Set9341_Parameter(); //special setting for LCD module
	GPIOB -> BSRR |= 0x00000001; //LCD_LIGHT_ON;
	/*Configure FSMC*/
}







//(command first, then data)
void IERG3810_TFTLCD_DrawDot(u16 x, u16 y, u16 color) {

	/*
	1.write to command register with a value 0x2A (Column Address set) once
	2.follow by a four-byte data of x-position
	*/
	IERG3810_TFTLCD_WrReg(0x2A); //set x position 
	IERG3810_TFTLCD_WrData(x >> 8);
	IERG3810_TFTLCD_WrData(x & 0xFF);
	IERG3810_TFTLCD_WrData(0x01); 
	IERG3810_TFTLCD_WrData(0x3F);

	/*
	1.write to command register with a value 0x2B (Page Address set) once
	2.follow by a four-byte data of y-position
	*/
	IERG3810_TFTLCD_WrReg(0x2B); //set y position 
	IERG3810_TFTLCD_WrData(y >> 8); 
	IERG3810_TFTLCD_WrData(y & 0xFF); 
	IERG3810_TFTLCD_WrData(0x01);
	IERG3810_TFTLCD_WrData(0xDF);

	IERG3810_TFTLCD_WrReg(0x2C); //set point with color 
	IERG3810_TFTLCD_WrData(color);
}

int main(void) {

	IERG3810_TFTLCD_Init();
	
	while (1){
		
		for (y = 1; y <= 5; y++){
			for (x = 10; x <= 30; x++){
				IERG3810_TFTLCD_DrawDot(x, y * 10, colors[y - 1]);
				Delay(10000);
			}
		}

	}	
}
