//Nick Sells, 2021
//player.h

#ifndef PLAYER
#define PLAYER

#include <tonc_video.h>
#include "level.h"

//TODO: any part of the player struct that doesn;t need to be saved to sram should be a global

//=============================================================================
//STRUCTS
//=============================================================================

//NOTE: this disgusting forward declaration is to resolve a cyclic dependency
typedef struct Level Level;
typedef struct Player {
	POINT16 pos;            //the player's position
} ALIGN4 Player;

//=============================================================================
//DIRECTIONS
//=============================================================================

#define FACING_UP       1
#define FACING_DOWN     0
#define FACING_LEFT     2
#define FACING_RIGHT    3

//=============================================================================
//POSITION ON SCREEN
//=============================================================================
//the player sprite always stays at this position on-screen
//you must add these when doing any sort of player location checking

#define plr_scrPosX     (SCREEN_WIDTH / 2 - 8)
#define plr_scrPosY     (SCREEN_HEIGHT / 2 - 16)

//=============================================================================
//POSITION IN LEVEL
//=============================================================================
//these return the player's meta-tile position in the level

#define plr_getX(plr)  (((plr)->pos.x + plr_scrPosX) / 16)
#define plr_getY(plr)  (((plr)->pos.y + plr_scrPosY) / 16)

//=============================================================================
//MOVEMENT
//=============================================================================

#define MOVE_COOLDOWN   12  //how long the player must wait to move again
#define MOVE_FRAMES     8   //how many frames it takes to move one tile
#define MOVE_INTERVAL   2   //the number of pixels to move each frame
//NOTE: OPT_MOVE_INTERVAL x OPT_MOVE_FRAMES must be equal to 16 or the player desyncs with the grid

//=============================================================================
//FUNCTIONS
//=============================================================================

extern void plr_move(Player* plr, const Level* lvl, cu32 bg);

#endif //PLAYER
