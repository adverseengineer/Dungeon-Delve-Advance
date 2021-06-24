//Nick Sells, 2021
//actor.c

#include <stdlib.h>

#include "info.h"

#include "level.h"
//NOTE: (Jun 15, 2021) HOLY SHIT! I finally resolved the cyclic dependency
//between actor and level, but the compromise is that now I can never include
//actor.h in any file except level.h. when you need actors, you must include
//level.h instead.

#include "actor_base.h"
#include "actor_player.h"
#include "actor_skeleton.h"

//=============================================================================
// CONSTRUCTOR/DESTRUCTOR
//=============================================================================

//allocates an actor on the heap
//NOTE: takes no x or y param because they are meaningless until the actor is
//bound to a level
Actor* actor_create(ActorType type) {
	Actor* this = calloc(1, sizeof(*this));
	this->type = type;
	switch(type) {
		case ACTOR_PLAYER:
			this->info = actor_player_info;
			this->move = actor_player_move;
			this->attack = actor_player_attack;
			this->chase = actor_player_chase;
			this->flee = actor_player_flee;
			this->idle = actor_player_idle;
			this->die = actor_player_die;
			this->sprite = spr_create(0, 0, TID_PLAYER_WARRIOR, OBJ_PRIO_ACTORS);
			break;
		case ACTOR_SKELETON:
			this->info = actor_skeleton_info;
			this->move = actor_skeleton_move;
			this->attack = actor_skeleton_attack;
			this->chase = actor_skeleton_chase;
			this->flee = actor_skeleton_flee;
			this->idle = actor_skeleton_idle;
			this->die = actor_skeleton_die;
			this->sprite = spr_create(0, 0, TID_SKELETON, OBJ_PRIO_ACTORS);
			break;
		default:
			this->info = actor_base_info;
			this->move = actor_base_move;
			this->attack = actor_base_attack;
			this->chase = actor_base_chase;
			this->flee = actor_base_flee;
			this->idle = actor_base_idle;
			this->die = actor_base_die;
			this->sprite = spr_create(0, 0, TID_ERR, OBJ_PRIO_ACTORS);
	}

	return this;
}

void actor_destroy(Actor* this) {
	spr_destroy(this->sprite);
	free(this);
}

//=============================================================================
// FUNCTIONS
//=============================================================================

//move the actor to an absolute position within a level
void actor_setPos(Actor* this, Level* lvl, s32 x, s32 y) {
	//the actor moves in level coords
	this->pos.x = x;
	this->pos.y = y;
	//while it's sprite moves in screen coords
	spr_setPos(
		this->sprite,
		this->pos.x * 16 - lvl->offset.x,
		this->pos.y * 16 - lvl->offset.y
	);
}
