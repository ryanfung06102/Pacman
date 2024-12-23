#ifndef __SCORE_H
#define __SCORE_H
#include "stm32f10x.h"

// put procedure header here

u32 PointScoreIncrease(s16 pacmanX, s16 pacmanY, u8 GameMap[31][28]);
u32 GhostEattenScoreIncrease(u8 numberOfGhosts);

#endif
