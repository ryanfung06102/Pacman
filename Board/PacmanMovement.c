#include "stm32f10x.h"
#include "PacmanMovement.h"

// put your procedure and code here

// update pacman x and y coordianate
// UpdatePacmanPosition -> UpdatePacmanNextPosition
// *********************************************************************************************
// update pacman.x and pacman.y
void UpdatePacmanPosition(Pacman *pacman){

    pacman -> x = (pacman -> x) + (pacman -> xDirection);
    pacman -> y = (pacman -> y) + (pacman -> yDirection);

}

// update pacman.x and pacman.y if on a portal
// return 1 for left portal, 2 for right portal
u8 UpdatePacmanPortalPosition(Pacman* pacman){

    if ((pacman -> x) == 0 && (pacman -> y) == 16){
        pacman -> x = 27;
        pacman -> y = 16;
        return 1;
    }
    else if ((pacman -> x) == 27 && (pacman -> y) == 16){
        pacman -> x = 0;
        pacman -> y = 16;
        return 2;
    }
    return 0;

}

// get pacman info and return as a list
// s16* info = GetPacmanInfo();
s16* GetPacmanInfo(Pacman *pacman){

    static s16 Info[7];

    Info[0] = pacman -> x;
    Info[1] = pacman -> y;
    Info[2] = pacman -> xDirection;
    Info[3] = pacman -> yDirection;
    Info[4] = pacman -> xBuffer;
    Info[5] = pacman -> yBuffer;
    Info[6] = pacman -> steps;

    return Info;

}
// *********************************************************************************************

// update pacman direction
// *********************************************************************************************
// update pacman.xDirection and pacman.yDirection FROM pacman.xBuffer and pacman.yBuffer
void UpdatePacmanDirection(Pacman *pacman){

    pacman -> xDirection = pacman -> xBuffer;
    pacman -> yDirection = pacman -> yBuffer;

}
// *********************************************************************************************

// update pacman buffer input
// *********************************************************************************************
// update pacman.xBuffer and pacman.yBuffer
// buffer should be user input
void UpdatePacmanBuffer(Pacman *pacman, s16 xInput, s16 yInput){

    pacman -> xBuffer = xInput;
    pacman -> yBuffer = yInput;

}
// *********************************************************************************************

// update pacman x and y coordianate
// *********************************************************************************************
// update pacman.x and pacman.y
void UpdatePacmanNextDirection(Pacman *pacman, u8 GameMap[31][28]){

    s16 X = (pacman -> x) + (pacman -> xBuffer);
    s16 Y = (pacman -> y) + (pacman -> yBuffer);

    if (GameMap[Y][X] != 1 && GameMap[Y][X] != 5){
        UpdatePacmanDirection(pacman);
    }

}
// *********************************************************************************************

// update pacman steps
// *********************************************************************************************
// update pacman.steps by increasing steps by input
void UpdatePacmanSteps(Pacman *pacman, s16 step){

    pacman -> steps += step;

}

// check if pacman -> steps is 8 or larger 
// reset steps to 0 once reached 8
u8 PacmanStepsIs8(Pacman *pacman){
    
    if ((pacman -> steps) < 8){
        return 0;
    }
    else{
        UpdatePacmanSteps(pacman, -8);
        return 1;
    }

}
// *********************************************************************************************

// checking function
// *********************************************************************************************
// check next tile's walkability
u8 PacmanNextIsWalkable(Pacman *pacman, u8 GameMap[31][28]){

    u8 pos = GameMap[(pacman -> y) + (pacman -> yDirection)][(pacman -> x) + (pacman -> xDirection)];
    if (pos != 1 && pos != 5){
        return 1;
    }
    else {
        return 0;
    }

}

// check on a portal
u8 PacmanIsOnPortal(Pacman *pacman, u8 GameMap[31][28]){

    if (GameMap[pacman -> y][pacman -> x] == 7){
        return 1;
    }
    else {
        return 0;
    }

}
// *********************************************************************************************
