#ifndef __IERG3810_TFTLCD_H
#define __IERG3810_TFTLCD_H
#include "stm32f10x.h"


void IERG3810_TFTLCD_WrReg(u16);
void IERG3810_TFTLCD_WrData(u16);

void LCD_Set9341_Parameter(void);
void IERG3810_TFTLCD_Init(void);

void IERG3810_TFTLCD_DrawDot(u16, u16, u16);
void IERG3810_TFTLCD_FillRectangle(u16, u16, u16, u16, u16);
void IERG3810_TFTLCD_ShowChar    (u16, u16, u8,  u16, u16);
void IERG3810_TFTLCD_ShowChinChar(u16, u16, u8,  u16, u16);
void IERG3810_TFTLCD_ShowString  (u16, u16, u8*, u16, u16);

#endif
