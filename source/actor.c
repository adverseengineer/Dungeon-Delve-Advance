//Nick Sells, 2021
//actor.c

#include <stdlib.h>

#include "info.h"

#include "level.h"
//NOTE: (Jun 15, 2021) HOLY SHIT! I finally resolved the cyclic dependency
//between actor and level, but the compromise is that now I can never include
//actor.h in any file except level.h. when you need actors, you must include
//level.h instead.

#include "actor/base.h"
#include "actor/player.h"
#include "actor/skeleton.h"

//=============================================================================
// CONSTRUCTOR/DESTRUCTOR
//=============================================================================

//allocates an actor on the heap
//NOTE: takes no x or y param because they are meaningless until the actor is bound to a level
Actor* actor_create(ActorType type) {
	Actor* self = malloc(sizeof(*self));
	self->type = type;
	switch(type) {
		case ACTOR_PLAYER:
			self->info = actor_info_player;
			self->move = actor_move_player;
			self->attack = actor_attack_player;
			self->chase = actor_chase_player;
			self->flee = actor_flee_player;
			self->idle = actor_idle_player;
			self->die = actor_die_player;
			self->sprite = spr_create(0, 0, TID_PLAYER_WARRIOR, OBJ_PRIO_ACTORS);
			break;
		case ACTOR_SKELETON:
			self->info = actor_info_skeleton;
			self->move = actor_move_skeleton;
			self->attack = actor_attack_skeleton;
			self->chase = actor_chase_skeleton;
			self->flee = actor_flee_skeleton;
			self->idle = actor_idle_skeleton;
			self->die = actor_die_skeleton;
			self->sprite = spr_create(0, 0, TID_SKELETON, OBJ_PRIO_ACTORS);
			break;
		default:
			self->info = actor_info_base;
			self->move = actor_move_base;
			self->attack = actor_attack_base;
			self->chase = actor_chase_base;
			self->flee = actor_flee_base;
			self->idle = actor_idle_base;
			self->die = actor_die_base;
			self->sprite = spr_create(0, 0, TID_ERR, OBJ_PRIO_ACTORS);
	}

	return self;
}

void actor_destroy(Actor* self) {
	spr_destroy(self->sprite);
	free(self);
}

//=============================================================================
// FUNCTIONS
//=============================================================================

//move the actor to an absolute position within a level
void actor_setPos(Actor* self, Level* lvl, s32 x, s32 y) {
	//the actor moves in level coords
	self->pos.x = x;
	self->pos.y = y;
	//while it's sprite moves in screen coords
	spr_setPos(
		self->sprite,
		self->pos.x * 16 - lvl->offset.x, //map from lvl coords to screen coords
		self->pos.y * 16 - lvl->offset.y
	);
}
