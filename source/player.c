//Nick Sells. 2021
//player.c

#include <tonc_types.h>
#include <tonc_input.h>
#include <tonc_mgba.h>

#include "player.h"

//=============================================================================
//FUNCTIONS
//=============================================================================

//ticks down the move timer and inches the player forward
static inline void plr_move_tick(Player* plr) {
	//decrement the move timer to zero
	if(plr->move_timer > 0) {
		plr->move_timer--;
		//and inc or dec the x or y based on which way we're facing
		switch(plr->facing) {
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
	if(plr->move_cooldown > 0)
		plr->move_cooldown--;
	plr->move_cooldown -= (plr->move_cooldown > 0);
}


//converts d-pad input into movement
void plr_move(Player* plr, const Level* lvl, cu32 bg) {
	//attempt to move a bit
	plr_move_tick(plr);

	//if the player is not moving and is allowed to move again
	if((plr->move_timer == 0) & (plr->move_cooldown == 0)) {
		
		if(key_is_down(KEY_UP)) {
			plr->facing = FACING_UP;
			if(LVL_IS_WALKABLE(lvl, PLR_GET_X(plr), PLR_GET_Y(plr) - 1))
				plr->move_timer = MOVE_FRAMES; 
		}
		else if(key_is_down(KEY_DOWN)) {
			plr->facing = FACING_DOWN;
			if(LVL_IS_WALKABLE(lvl, PLR_GET_X(plr), PLR_GET_Y(plr) + 1))
				plr->move_timer = MOVE_FRAMES;
		}
		else if(key_is_down(KEY_LEFT)) {
			plr->facing = FACING_LEFT;
			if(LVL_IS_WALKABLE(lvl, PLR_GET_X(plr) - 1, PLR_GET_Y(plr)))
				plr->move_timer = MOVE_FRAMES;
		}
		else if(key_is_down(KEY_RIGHT)) {
			plr->facing = FACING_RIGHT;
			if(LVL_IS_WALKABLE(lvl, PLR_GET_X(plr) + 1, PLR_GET_Y(plr)))
				plr->move_timer = MOVE_FRAMES;
		}

		plr->move_cooldown = MOVE_COOLDOWN;
	}

	REG_BG_OFS[bg] = plr->pos;
}
