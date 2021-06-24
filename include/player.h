//Nick Sells, 2021
//player.h

//handles player interaction within a level

#ifndef PLAYER_H
#define PLAYER_H

#include "level.h" //include level instead of actor because that thing

typedef Actor Player;

static inline Player* plr_create(void) {
	return actor_create(ACTOR_PLAYER);
}

#endif //PLAYER_H
