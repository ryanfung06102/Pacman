#ifndef __SPRITE_H
#define __SPRITE_H
#include "stm32f10x.h"

// put procedure header here

// Define
// *********************************************************************************************
typedef struct{

	s16 LCD_REG;
	s16 LCD_RAM;

} LCD_TypeDef;

#define LCD_BASE ((u32) (0x6C000000 | 0x000007FE))
#define LCD ((LCD_TypeDef*) LCD_BASE)
// *********************************************************************************************

void IERG3810_TFTLCD_WrReg(s16 regval);
void IERG3810_TFTLCD_WrData(s16 data);
void LCD_Set9341_Parameter(void);
void IERG3810_TFTLCD_Init(void);
void IERG3810_TFTLCD_DrawDot(s16 x, s16 y, s16 color);
void PrintSprite(s16 x, s16 y, s16 Height, s16 Length, u8 Sprite[Height][Length]);
s16 ToPixelX(s16 pacmanX);
s16 ToPixelY(s16 pacmanY);
void PrintMap(void);
void RepairPrintMap(s16 inputX, s16 inputY, u8 logicMap[31][28]);
void IERG3810_TFTLCD_FillRectangle(u16 color, u16 start_x, u16 length_x, u16 start_y, u16 length_y);
void IERG3810_TFTLCD_ShowChar    (u16, u16, u8,  u16, u16);
void IERG3810_TFTLCD_ShowChinChar(u16, u16, u8,  u16, u16);
void IERG3810_TFTLCD_ShowString  (u16, u16, u8*, u16, u16);

#endif
