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
u8 digit;

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

void IERG3810_TFTLCD_WrReg(u16 regval){

	LCD -> LCD_REG = regval;

}

void IERG3810_TFTLCD_WrData(u16 data){

	LCD -> LCD_RAM = data;

}

void LCD_Set9341_Parameter(void){

	IERG3810_TFTLCD_WrReg(0x01);
	IERG3810_TFTLCD_WrReg(0x11);

	IERG3810_TFTLCD_WrReg(0x3A);
	IERG3810_TFTLCD_WrData(0x55);

	IERG3810_TFTLCD_WrReg(0x29);

	IERG3810_TFTLCD_WrReg(0x36);
	IERG3810_TFTLCD_WrData(0xC8);

}

void IERG3810_TFTLCD_Init(void) {

	RCC -> AHBENR |= 1 << 8;
	RCC -> APB2ENR |= 1 << 3;
	RCC -> APB2ENR |= 1 << 5;
	RCC -> APB2ENR |= 1 << 6;
	RCC -> APB2ENR |= 1 << 8;
	GPIOB -> CRL &= 0xFFFFFFF0;
	GPIOB -> CRL |= 0x00000003;

	GPIOD -> CRH &= 0x00FFF000;
	GPIOD -> CRH |= 0xBB000BBB;
	GPIOD -> CRL &= 0xFF00FF00;
	GPIOD -> CRL |= 0x00BB00BB;

	GPIOE -> CRH &= 0x00000000;
	GPIOE -> CRH |= 0xBBBBBBBB;
	GPIOE -> CRL &= 0x0FFFFFFF;
	GPIOE -> CRL |= 0xB0000000;

	GPIOG -> CRH &= 0xFFF0FFFF;
	GPIOG -> CRH |= 0x000B0000;
	GPIOG -> CRL &= 0xFFFFFFF0;
	GPIOG -> CRL |= 0x0000000B;

	FSMC_Bank1 -> BTCR[6] = 0x00000000; 
	FSMC_Bank1 -> BTCR[7] = 0x00000000; 
	FSMC_Bank1E -> BWTR[6] = 0x00000000; 
	FSMC_Bank1 -> BTCR[6] |= 1 << 12; 
	FSMC_Bank1 -> BTCR[6] |= 1 << 14; 
	FSMC_Bank1 -> BTCR[6] |= 1 << 4; 
	FSMC_Bank1 -> BTCR[7] |= 0 << 28; 
	FSMC_Bank1 -> BTCR[7] |= 1 << 0; 
	FSMC_Bank1 -> BTCR[7] |= 0xF << 8; 
	FSMC_Bank1E -> BWTR[6] |= 0 << 28; 
	FSMC_Bank1E -> BWTR[6] |= 0 << 0;
	FSMC_Bank1E -> BWTR[6] |= 3 << 8; 
	FSMC_Bank1 -> BTCR[6] |= 1 << 0;
	LCD_Set9341_Parameter();
	//LCD_LIGHT_ON;
	GPIOB -> BSRR |= 0x00000001;

}

void IERG3810_TFTLCD_DrawDot(u16 x, u16 y, u16 color) {

	IERG3810_TFTLCD_WrReg(0x2A); //set x position 
	IERG3810_TFTLCD_WrData(x >> 8);
	IERG3810_TFTLCD_WrData(x & 0xFF);
	IERG3810_TFTLCD_WrData(0x01); 
	IERG3810_TFTLCD_WrData(0x3F);

	IERG3810_TFTLCD_WrReg(0x2B); //set y position 
	IERG3810_TFTLCD_WrData(y >> 8); 
	IERG3810_TFTLCD_WrData(y & 0xFF); 
	IERG3810_TFTLCD_WrData(0x01);
	IERG3810_TFTLCD_WrData(0xDF);

	IERG3810_TFTLCD_WrReg(0x2C); //set point with color 
	IERG3810_TFTLCD_WrData(color);

}

void IERG3810_TFTLCD_FillRectangle (u16 color, u16 start_x, u16 length_x, u16 start_y, u16 length_y) {

	u32 index=0;
	IERG3810_TFTLCD_WrReg(0x2A); 
	IERG3810_TFTLCD_WrData(start_x >> 8);
	IERG3810_TFTLCD_WrData(start_x & 0xFF);
	IERG3810_TFTLCD_WrData((length_x + start_x - 1) >> 8); 
	IERG3810_TFTLCD_WrData((length_x + start_x - 1) & 0xFF);

	IERG3810_TFTLCD_WrReg(0x2B);
	IERG3810_TFTLCD_WrData(start_y >> 8);
	IERG3810_TFTLCD_WrData(start_y & 0xFF);
	IERG3810_TFTLCD_WrData((length_y + start_y - 1) >> 8); 
	IERG3810_TFTLCD_WrData((length_y + start_y - 1) & 0xFF); 

	IERG3810_TFTLCD_WrReg(0x2C);
	for (index = 0; index < length_x * length_y; index++) {
		IERG3810_TFTLCD_WrData(color);
	}

}

void IERG3810_TFTLCD_ShowChar (u16 x, u16 y, u8 ascii, u16 color, u16 bgcolor) {

	u8 i, j;
	u8 index;
	u8 height = 16, length = 8;
	if (ascii < 32 || ascii > 127) {
		return;
	}
	ascii -= 32;
	IERG3810_TFTLCD_WrReg(0x2A);
	IERG3810_TFTLCD_WrData(x >> 8);
	IERG3810_TFTLCD_WrData(x & 0xFF);
	IERG3810_TFTLCD_WrData((length + x - 1) >> 8); 
	IERG3810_TFTLCD_WrData((length + x - 1) & 0xFF); 

	IERG3810_TFTLCD_WrReg (0x2B);
	IERG3810_TFTLCD_WrData(y >> 8);
	IERG3810_TFTLCD_WrData(y & 0xFF);
	IERG3810_TFTLCD_WrData((height + y - 1) >> 8); 
	IERG3810_TFTLCD_WrData((height + y - 1) & 0xFF); 

	IERG3810_TFTLCD_WrReg(0x2C);
	for (j = 0; j < height / 8; j++) {
		for (i = 0; i < height / 2; i++) {
			for (index = 0; index < length; index++) {
				if ((asc2_1608[ascii][index * 2 + 1 - j] >> i) & 0x01) {
					IERG3810_TFTLCD_WrData(color);
				} 
				else {
					IERG3810_TFTLCD_WrData(bgcolor);
				}
			}
		}
	}

}

//1155188577 BU LIU SHIUAN
void IERG3810_TFTLCD_SevenSegment(u16 color, u16 start_x, u16 start_y, u8 digit){
	
	if (digit == 2 || digit == 3 || digit == 5 || digit == 6 || digit == 7 || digit == 8 || digit == 9 || digit == 0) //retangle a
		IERG3810_TFTLCD_FillRectangle (color, start_x + 10, 55, start_y + 130, 10);
	if (digit == 1 || digit == 2 || digit == 3 || digit == 4 || digit == 7 || digit == 8 || digit == 9 || digit == 0) //retangle b
		IERG3810_TFTLCD_FillRectangle (color, start_x + 65, 10, start_y + 75, 55);
	if (digit == 1 || digit == 3 || digit == 4 || digit == 5 || digit == 6 || digit == 7 || digit == 8 || digit == 9 || digit == 0) //retangle c
		IERG3810_TFTLCD_FillRectangle (color, start_x + 65, 10, start_y + 10, 55);
	if (digit == 2 || digit == 3 || digit == 5 || digit == 6 || digit == 8 || digit == 9 || digit == 0) //retangle d
		IERG3810_TFTLCD_FillRectangle (color, start_x + 10, 55, start_y + 0, 10);
	if (digit == 2 || digit == 6 || digit == 8 || digit == 0) //retangle e
		IERG3810_TFTLCD_FillRectangle (color, start_x + 0, 10, start_y + 10, 55);
	if (digit == 4 || digit == 5 || digit == 6 || digit == 8 || digit == 9 || digit == 0) //retangle f
		IERG3810_TFTLCD_FillRectangle (color, start_x + 0, 10, start_y + 75, 55);
	if (digit == 2 || digit == 3 || digit == 4 || digit == 5 || digit == 6 || digit == 8 || digit == 9) //retangle g
		IERG3810_TFTLCD_FillRectangle (color, start_x + 10, 55, start_y + 65, 10);
}
int main(void) {

	IERG3810_TFTLCD_Init();
	digit = 0;
	
	while (1){

		IERG3810_TFTLCD_SevenSegment(red, 85, 85, digit);
		Delay(10000000);
		
		if (digit != 0)
			digit--;
		else 
			digit = 9;
		
		IERG3810_TFTLCD_FillRectangle (white, 0, 240, 0, 320); //refresh the screen
	}
	
}
