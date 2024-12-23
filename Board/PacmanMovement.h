#ifndef __PACMANMOVEMENT_H
#define __PACMANMOVEMENT_H
#include "stm32f10x.h"

// put procedure header here

// Game Data
// *********************************************************************************************
typedef struct pacman{

    // the tile position of the pacman
    // remember to use GAMEMAP[pacman.y][pacman.x]
    s16 x;
    s16 y; 

    // PROBLEM HERE, s16 IS UNSIGNED, I NEED SIGNED DATA TYPE
    // IF POSSIBLE, CHANGE ALL s16 TO SIGNED DATA TYPE
    // the direction where the pacman is heading
    s16 xDirection;
    s16 yDirection;

    // the buffering input direction from the player
    s16 xBuffer;
    s16 yBuffer;

    // number of pixels pacman has walk between each tiles
    // should not be larger than 8 
    s16 steps;

}Pacman;
// *********************************************************************************************

void UpdatePacmanPosition(Pacman *pacman);
u8 UpdatePacmanPortalPosition(Pacman* pacman);
s16* GetPacmanInfo(Pacman *pacman);
void UpdatePacmanDirection(Pacman *pacman);
void UpdatePacmanBuffer(Pacman *pacman, s16 xInput, s16 yInput);
void UpdatePacmanNextDirection(Pacman *pacman, u8 GameMap[31][28]);
void UpdatePacmanSteps(Pacman *pacman, s16 step);
u8 PacmanStepsIs8(Pacman *pacman);
u8 PacmanNextIsWalkable(Pacman *pacman, u8 GameMap[31][28]);
u8 PacmanIsOnPortal(Pacman *pacman, u8 GameMap[31][28]);

#endif
