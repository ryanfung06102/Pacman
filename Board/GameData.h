#ifndef __GAMEDATA_H
#define __GAMEDATA_H 
#include "stm32f10x.h"

u8 TileIsPoint(s16 pacmanX, s16 pacmanY, u8 GameMap[31][28]);
u8* IntToString(u32 score);
void CopyMap(u8 CONSTANT_GAMEMAP[31][28], u8 GAMEMAP[31][28]);

#endif
