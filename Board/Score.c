#include "stm32f10x.h"
#include "Score.h"

// put your procedure and code here

// update socre
// *********************************************************************************************
// score increase when eatten a point
u32 PointScoreIncrease(s16 pacmanX, s16 pacmanY, u8 GameMap[31][28]){

    if (GameMap[pacmanY][pacmanX] == 0){
        return 10;
    }
    else if (GameMap[pacmanY][pacmanX] == 2){
        return 50;
    }
    return 0;

}

// score increase when eatten a ghost
u32 GhostEattenScoreIncrease(u8 numberOfGhosts){

    return 200 * numberOfGhosts;

}
// *********************************************************************************************
