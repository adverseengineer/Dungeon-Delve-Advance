//Nick Sells, 2021
//player.h

//handles player interaction within a level

#ifndef PLAYER_H
#define PLAYER_H

#include "sprite.h"

//=============================================================================
//STRUCTS
//=============================================================================

typedef struct Player {
	Sprite* sprite;
	POINT16 pos;
	u16 hp_max;
	u16 hp_current;
} ALIGN4 Player;

//=============================================================================
//INLINES
//=============================================================================

static inline Player* plr_create(s32 x, s32 y, u32 hp_max, u32 hp_current) {
	//allocate the needed memory
	Player* plr = (Player*) calloc(1, sizeof(Player));
	//set up the player
	plr->pos.x = x;
	plr->pos.y = y;
	plr->hp_max = hp_max;
	plr->hp_current = hp_current;
	//set up the player's sprite
	//TODO: set up a way to determine the correct player tid
	// plr->sprite = spr_create(x * 16, y * 16, 1);
	spr_link(plr->sprite);

	return plr;
}

#endif //PLAYER_H