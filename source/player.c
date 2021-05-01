//Nick Sells. 2021
//player.c

#include <tonc_types.h>
#include <tonc_input.h>

#include "player.h"

//=============================================================================
//GLOBALS
//=============================================================================
//NOTE: these are split out of the player struct because they don't need to be saved to sram

static u32 facing        = 0;   //the direction the player is facing
static u32 move_timer    = 0;   //the number of frames left to finish moving
static u32 move_cooldown = 0;   //the number of frames until you can move again

//=============================================================================
//FUNCTIONS
//=============================================================================

//ticks down the move timer and inches the player forward
static inline void plr_moveTick(Player* plr) {
	//decrement the move timer to zero
	if(move_timer > 0) {
		move_timer--;
		//and inc or dec the x or y based on which way we're facing
		switch(facing) {
			case FACING_DOWN:
				plr->pos.y += MOVE_INTERVAL;
				break;
			case FACING_UP:
				plr->pos.y -= MOVE_INTERVAL;
				break;
			case FACING_LEFT:
				plr->pos.x -= MOVE_INTERVAL;
				break;
			case FACING_RIGHT:
				plr->pos.x +=  MOVE_INTERVAL;
				break;
		}
	}
	//decrement the move cooldown to zero
	if(move_cooldown > 0)
		move_cooldown--;
}


//converts d-pad input into movement
void plr_move(Player* plr, const Level* lvl, cu32 bg) {

	//attempt to move a bit
	plr_moveTick(plr);

	//if the player is not moving and is allowed to move again
	if((move_timer == 0) & (move_cooldown == 0)) {
		
		if(key_is_down(KEY_UP)) {
			facing = FACING_UP;
			if(LVL_IS_WALKABLE(lvl, plr_getX(plr), plr_getY(plr) - 1)) {
				move_timer = MOVE_FRAMES;
				move_cooldown = MOVE_COOLDOWN;
			}
		}
		else if(key_is_down(KEY_DOWN)) {
			facing = FACING_DOWN;
			if(LVL_IS_WALKABLE(lvl, plr_getX(plr), plr_getY(plr) + 1)) {
				move_timer = MOVE_FRAMES;
				move_cooldown = MOVE_COOLDOWN;
			}
		}
		else if(key_is_down(KEY_LEFT)) {
			facing = FACING_LEFT;
			if(LVL_IS_WALKABLE(lvl, plr_getX(plr) - 1, plr_getY(plr))) {
				move_timer = MOVE_FRAMES;
				move_cooldown = MOVE_COOLDOWN;
			}
		}
		else if(key_is_down(KEY_RIGHT)) {
			facing = FACING_RIGHT;
			if(LVL_IS_WALKABLE(lvl, plr_getX(plr) + 1, plr_getY(plr))) {
				move_timer = MOVE_FRAMES;
				move_cooldown = MOVE_COOLDOWN;
			}
		}
	}

	REG_BG_OFS[bg] = plr->pos;
}
