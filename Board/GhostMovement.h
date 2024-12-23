#ifndef __GHOSTMOVEMENT_H
#define __GHOSTMOVEMENT_H
#include "stm32f10x.h"

// put procedure header here

// Game Data
// *********************************************************************************************
typedef struct ghost{

    // the ghost position
    s16 x;
    s16 y;

		// the direction where the ghost is heading
    s16 xDirection;
    s16 yDirection;

    // the chasing target position
    // each ghost will have their own chasing position, the position will be calculate in seprate functions 
    s16 xTarget;
    s16 yTarget;
		
    // THIS SHOULD BE A CONSTANT, DON NOT CHANGE
    s16 xScatterTarget;
    s16 yScatterTarget;

    // number of pixels ghost has walk between each tiles
    // should not be larger than 8 
    s16 steps;

    // the current ghost mode
    // 0 = chase, 1 = scatter, 2 = eatten, 3 = frightened, 4 = exit ghost house
    s16 mode;

}Ghost;

typedef struct mode{
		
    // current GAME mode, this is the GAME-WISE GHOST MODE, NOT AN INDIVIDUAL GHOST MODE
    // eatten and exit ghost house mode will never be a global game mode
    // 0 = chase, 1 = scatter, 3 = frightened
    s16 mode;

}Mode;
// *********************************************************************************************

void UpdateGameModeTo(Mode *mode, s16 modeToSwitch);
s16 GetGameMode(Mode *mode);
void UpdateGhostModeToGameMode(Ghost *ghost, Mode* mode);
void UpdateGhostModeToInput(Ghost *ghost, s16 mode);
void UpdateGhostPosition(Ghost *ghost);
u8 UpdateGhostPortalPosition(Ghost* ghost);
s16* GetGhostInfo(Ghost *ghost);
s32 CalculateDistance(s16 x1,s16 y1,s16 x2,s16 y2);
void UpdateGhostDirection(Ghost *ghost, u8 GameMap[31][28]);
void TurnBlinky(Ghost *blinky);
void TurnPinky(Ghost *pinky);
void TurnInky(Ghost *inky);
void TurnClyde(Ghost *clyde);
void TargetToScatter(Ghost *ghost);
void TargetToEatten(Ghost *ghost);
void TargetToHouse(Ghost *ghost);
void TargetToFrightened(Ghost *ghost, u8 GameMap[31][28], u8 tick);
void UpdateBlinkyTarget(Ghost *ghost, s16 pacmanX, s16 pacmanY, u8 GameMap[31][28], u8 tick);
void UpdatePinkyTarget(Ghost *ghost, s16 pacmanX, s16 pacmanY, s16 pacmanXDir, s16 pacmanYDir, u8 GameMap[31][28], u8 tick);
void UpdateInkyTarget(Ghost *ghost, Ghost *blinky, s16 pacmanX, s16 pacmanY, s16 pacmanXDir, s16 pacmanYDir, u8 GameMap[31][28], u8 tick);
void UpdateClydeTarget(Ghost *ghost, s16 pacmanX, s16 pacmanY, u8 GameMap[31][28], u8 tick);
void UpdateGhostSteps(Ghost *ghost, s16 step);
u8 GhostStepsIs8(Ghost *ghost);
u8 GhostIsOnPortal(Ghost *ghost, u8 GameMap[31][28]);
u8 GhostIsOnPacman(Ghost *ghost, s16 pacmanX, s16 pacmanY);
u8 RandNum(u8 tick);

#endif
