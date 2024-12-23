#include "stm32f10x.h"
#include "GameData.h"

// put your procedure and code here

//check tile
// *********************************************************************************************
u8 TileIsPoint(s16 pacmanX, s16 pacmanY, u8 GameMap[31][28]){

    if (GameMap[pacmanY][pacmanX] == 0){
        return 1;
    } 
    if (GameMap[pacmanY][pacmanX] == 2){
        return 2;
    }
    return 0;

}
// *********************************************************************************************

//fransfer int to char
// *********************************************************************************************
u8* IntToString(u32 score){

    static u8 string[4];

    string[0] = (u8) ((score / 1000) + 48);
    string[1] = (u8) ((score / 100 % 10) + 48);
    string[2] = (u8) ((score / 10 % 100 % 10) + 48);
    string[3] = (u8) ((score % 1000 % 100 % 10) + 48);

    return string;

}
// *********************************************************************************************

//copy constant map to map use in game 
// *********************************************************************************************
void CopyMap(u8 CONSTANT_GAMEMAP[31][28], u8 GAMEMAP[31][28]){

    u8 i, j;

    for (j = 0; j < 31; j++){
        for (i = 0; i < 28; i++){
            GAMEMAP[j][i] = CONSTANT_GAMEMAP[j][i];
        }
    }

}
// *********************************************************************************************
