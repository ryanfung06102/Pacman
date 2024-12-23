#include "stm32f10x.h"
#include "GhostMovement.h"

// put your procedure and code here

// put procedure header here

// update game mode
// *********************************************************************************************
// update game mode to the mode pass in, should be work with a timer
// eatten mode will never be a global game mode
// 0 = chase, 1 = scatter, 3 = frightened
void UpdateGameModeTo(Mode *mode, s16 modeToSwitch){

    if (modeToSwitch == 0 || modeToSwitch == 1 || modeToSwitch == 3){
        mode -> mode = modeToSwitch;
    }

}

// get the current GLOBAL GAME MODE
s16 GetGameMode(Mode *mode){

    return (mode -> mode);

}
// *********************************************************************************************

// update ghost mode
// *********************************************************************************************
// update ghost mode to the game mode
// 0 = chase, 1 = scatter, 2 = eaten, 3 = frightened, 4 = exit ghost house
void UpdateGhostModeToGameMode(Ghost *ghost, Mode* mode){

    if ((mode -> mode) >= 0 && (mode -> mode) <= 4){
        ghost -> mode = mode -> mode;
    }

}

// update ghost mode to the input mode
// 0 = chase, 1 = scatter, 2 = eaten, 3 = frightened, 4 = exit ghost house
void UpdateGhostModeToInput(Ghost *ghost, s16 mode){

    if (mode >= 0 && mode <= 4){
        ghost -> mode = mode;
    }

}
// *********************************************************************************************

// update ghost x and y coordianate
// *********************************************************************************************
// update ghost.x and ghost.y
void UpdateGhostPosition(Ghost *ghost){

    ghost -> x = (ghost -> x) + (ghost -> xDirection);
    ghost -> y = (ghost -> y) + (ghost -> yDirection);

}

// update ghost.x and ghost.y if on a portal
// return 1 for left portal, 2 for right portal
u8 UpdateGhostPortalPosition(Ghost* ghost){

    if ((ghost -> x) == 0 && (ghost -> y) == 16){
        ghost -> x = 27;
        ghost -> y = 16;
        return 1;
    }
    else if ((ghost -> x) == 27 && (ghost -> y) == 16){
        ghost -> x = 0;
        ghost -> y = 16;
        return 2;
    }
    return 0;

}

// get ghost info and return as a list
// s16* info = GetPacmanInfo();
s16* GetGhostInfo(Ghost *ghost){

    static s16 Info[7];

    Info[0] = ghost -> x;
    Info[1] = ghost -> y;
    Info[2] = ghost -> xDirection;
    Info[3] = ghost -> yDirection;
    Info[4] = ghost -> steps;
    Info[5] = ghost -> mode;

    return Info;

}
// *********************************************************************************************

// update chost xDirection and yDirection
// *********************************************************************************************
// calculate the short distance from the current position to the target position
s32 CalculateDistance(s16 x1, s16 y1, s16 x2, s16 y2){

    // PROBLEM HERE, s16 IS UNSIGNED, I NEED SIGNED DATA TYPE
	// IF POSSIBLE, CHANGE ALL s16 TO SIGNED DATA TYPE
    s16 X = x1 - x2;
    s16 Y = y1 - y2; 
    return (s32) X * (s32) X + (s32) Y * (s32) Y;

}

// update ghost.xDirection and ghost.yDirection
void UpdateGhostDirection(Ghost *ghost, u8 GameMap[31][28]){

    s16 i = 0;
    // PROBLEM HERE, s16 IS UNSIGNED, I NEED SIGNED DATA TYPE
	// IF POSSIBLE, CHANGE ALL s16 TO SIGNED DATA TYPE
    s16 x[4] = {0, -1, 1, 0};
    s16 y[4] = {1, 0, 0, -1};
    u32 distance = 5000;
    s16 choice = 5;
    s16 backX = ghost -> xDirection;
    s16 backY = ghost -> yDirection;
    u8 no = 10;
    if (backX == 0 && backY == 1){
        no = 3;
    }
    else if (backX == -1 && backY == 0){
        no = 2;
    }
    else if (backX == 1 && backY == 0){
        no = 1;
    }
    else if (backX == 0 && backY == -1){
        no = 0;
    }

    for(i = 0; i < 4; i++){
        s16 X = (ghost -> x) + x[i];
        s16 Y = (ghost -> y) + y[i];
        /*((GameMap[Y][X] != 1 && GameMap[Y][X] != 5) || ((ghost -> mode == 2 || ghost -> mode == 4) && GameMap[Y][X] != 1)) && !(x[i] == 0 && y[i] == 1 && GameMap[Y][X] == 4) && x[i] != -(ghost -> xDirection) && y[i] != -(ghost -> yDirection)*/
        // 0 = chase, 1 = scatter, 2 = eatten, 3 = frightened, 4 = exit ghost house
        if (((GameMap[Y][X] != 1 && GameMap[Y][X] != 5) || ((ghost -> mode == 2 || ghost -> mode == 4) && GameMap[Y][X] != 1)) && !(x[i] == 0 && y[i] == 1 && GameMap[Y][X] == 4) && i != no){
            u32 tempDistance = CalculateDistance(X, Y, ghost -> xTarget, ghost -> yTarget);
            if (distance >= tempDistance){
                distance = tempDistance;
                choice = i;
            }
        }
    }

    if (choice != 5){
        ghost -> xDirection = x[choice];
        ghost -> yDirection = y[choice];
    }
    /*
    else {
        GPIOB -> ODR ^= 1 << 5;
    }
    */

}

// turn blinky 180 degree
void TurnBlinky(Ghost *blinky){

    blinky -> xDirection = -(blinky -> xDirection);
    blinky -> yDirection = -(blinky -> yDirection);

}

// turn Pinky 180 degree
void TurnPinky(Ghost *pinky){

    pinky -> xDirection = -(pinky -> xDirection);
    pinky -> yDirection = -(pinky -> yDirection);

}

// turn inky 180 degree
void TurnInky(Ghost *inky){

    inky -> xDirection = -(inky -> xDirection);
    inky -> yDirection = -(inky -> yDirection);

}

// turn clyde 180 degree
void TurnClyde(Ghost *clyde){

    clyde -> xDirection = -(clyde -> xDirection);
    clyde -> yDirection = -(clyde -> yDirection);

}
// *********************************************************************************************

// update ghost.xTarget and ghost.yTarget
// *********************************************************************************************
// change target to scatter target
void TargetToScatter(Ghost *ghost){

    ghost -> xTarget = ghost -> xScatterTarget;
    ghost -> yTarget = ghost -> yScatterTarget;

}

// change target to eatten target i.e. the middle of the ghost house 
void TargetToEatten(Ghost *ghost){

    // (x, y) = (13, 19) in front of the ghost house
    ghost -> xTarget = 13;
    ghost -> yTarget = 16;

}

// change target to exit house target i.e. the entrence of the ghost house
// go from mid house to house entrence
void TargetToHouse(Ghost *ghost){

    // (x, y) = (13, 19) in front of the ghost house
    ghost -> xTarget = 13;
    ghost -> yTarget = 19;

}

void TargetToFrightened(Ghost *ghost, u8 GameMap[31][28], u8 tick){

    s16 x[4] = {0, -1, 1, 0};
    s16 y[4] = {1, 0, 0, -1};
    u8 randomNum = RandNum(tick);
    s16 X = (ghost -> x) + x[randomNum];
    s16 Y = (ghost -> y) + y[randomNum];

    while (GameMap[Y][X] == 1 || (GameMap[Y][X] == 4 && x[randomNum] == 0 && y[randomNum] == 1)){
        if (randomNum < 3){
            randomNum++;
        }
        else {
            randomNum = 0;
        }
        X = (ghost -> x) + x[randomNum];
        Y = (ghost -> y) + y[randomNum];
        //GPIOB -> ODR ^= 1 << 5;
    }
    //GPIOE -> ODR ^= 1 << 5;

    ghost -> xTarget = X;
    ghost -> yTarget = Y;

}

// red ghost
void UpdateBlinkyTarget(Ghost *ghost, s16 pacmanX, s16 pacmanY, u8 GameMap[31][28], u8 tick){

    if (ghost -> mode == 0){
        ghost -> xTarget = pacmanX;
        ghost -> yTarget = pacmanY;
    }
    else if (ghost -> mode == 1){
        TargetToScatter(ghost);
    }
    else if (ghost -> mode == 2){
        TargetToEatten(ghost);
    }
    else if (ghost -> mode == 3){
        TargetToFrightened(ghost, GameMap, tick);
    }
    else if (ghost -> mode == 4){
        TargetToHouse(ghost);
    }

}

// pink ghost
void UpdatePinkyTarget(Ghost *ghost, s16 pacmanX, s16 pacmanY, s16 pacmanXDir, s16 pacmanYDir, u8 GameMap[31][28], u8 tick){
    
    if (ghost -> mode == 0){
        ghost -> xTarget = (pacmanX) + (pacmanXDir) * 4;
        ghost -> yTarget = (pacmanY) + (pacmanYDir) * 4;
    }
    else if (ghost -> mode == 1){
        TargetToScatter(ghost);
    }
    else if (ghost -> mode == 2){
        TargetToEatten(ghost);
    }
    else if (ghost -> mode == 3){
        TargetToFrightened(ghost, GameMap, tick);
    }
    else if (ghost -> mode == 4){
        TargetToHouse(ghost);
    }

}

// blue ghost
void UpdateInkyTarget(Ghost *ghost, Ghost *blinky, s16 pacmanX, s16 pacmanY, s16 pacmanXDir, s16 pacmanYDir, u8 GameMap[31][28], u8 tick){
    
    if (ghost -> mode == 0){
        s16 xTemp = (pacmanX) + (pacmanXDir) * 2;
        s16 yTemp = (pacmanY) + (pacmanYDir) * 2;

        // PROBLEM HERE, s16 IS UNSIGNED, I NEED SIGNED DATA TYPE
        // IF POSSIBLE, CHANGE ALL s16 TO SIGNED DATA TYPE
        s16 xLength = (blinky -> x) - xTemp;
        s16 ylength = (blinky -> y) - yTemp;

        ghost -> xTarget = xTemp - xLength;
        ghost -> yTarget = yTemp - ylength;
    }
    else if (ghost -> mode == 1){
        TargetToScatter(ghost);
    }
    else if (ghost -> mode == 2){
        TargetToEatten(ghost);
    }
    else if (ghost -> mode == 3){
        TargetToFrightened(ghost, GameMap, tick);
    }
    else if (ghost -> mode == 4){
        TargetToHouse(ghost);
    }

}

//orange ghost
void UpdateClydeTarget(Ghost *ghost, s16 pacmanX, s16 pacmanY, u8 GameMap[31][28], u8 tick){
    
    if (ghost -> mode == 0){
        if (CalculateDistance(ghost -> x, ghost -> y, pacmanX, pacmanY) <= 64){
            TargetToScatter(ghost);
        }
        else{
            ghost -> xTarget = pacmanX;
            ghost -> yTarget = pacmanY;
        }
    }
    else if (ghost -> mode == 1){
        TargetToScatter(ghost);
    }
    else if (ghost -> mode == 2){
        TargetToEatten(ghost);
    }
    else if (ghost -> mode == 3){
        TargetToFrightened(ghost, GameMap, tick);
    }
    else if (ghost -> mode == 4){
        TargetToHouse(ghost);
    }

}
// *********************************************************************************************

// update ghost steps
// *********************************************************************************************
// update ghost.steps by increasing steps by input
void UpdateGhostSteps(Ghost *ghost, s16 step){

    ghost -> steps += step;

}

// check if ghost -> steps is 8 or larger 
// reset steps to 0 once reached 8
u8 GhostStepsIs8(Ghost *ghost){
    
    if ((ghost -> steps) < 8){
        return 0;
    }
    else{
        UpdateGhostSteps(ghost, -8);
        return 1;
    }

}
// *********************************************************************************************

// checking function
// *********************************************************************************************
// check on a portal
u8 GhostIsOnPortal(Ghost *ghost, u8 GameMap[31][28]){

    if (GameMap[ghost -> y][ghost -> x] == 7){
        return 1;
    }
    else{
        return 0;
    }

}

// ghost and pacman is in contect
u8 GhostIsOnPacman(Ghost *ghost, s16 pacmanX, s16 pacmanY){

    if (ghost -> x == pacmanX && ghost -> y == pacmanY){
        return 1;
    }
    else{
        return 0;
    }

}
// *********************************************************************************************

// random number generator
// *********************************************************************************************
u8 RandNum(u8 tick){

    return tick % 4;

}
// *********************************************************************************************
