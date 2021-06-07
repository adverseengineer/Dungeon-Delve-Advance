//Nick Sells, 2021
//actor.c

//NOTE: the constructor and destructor in this file come after the actor
//behaviours because I'm not going to forward declare something that's
//visible only to this file

#include <stdlib.h>
#include "actor_level_common.h"

//=============================================================================
//FUNCTION DEFINITIIONS
//=============================================================================

//move the actor to an absolute position within a level
void actor_setPos(Actor* self, Level* lvl, s32 x, s32 y) {

	//the actor moves in level coords
	self->pos.x = x;
	self->pos.y = y;
	//the sprite moves in screen coords
	spr_setPos(
		self->sprite,
		self->pos.x * 16 - lvl->offset.x, //map from lvl coords to screen coords
		self->pos.y * 16 - lvl->offset.y
	);

	//NOTE: is this still necessary?
	// BOOL wasOnScreen = spr_onScreen(self->sprite);
	// //(move the sprite around)
	// //if the actor just moved onto the screen
	// if(!wasOnScreen && spr_onScreen(self->sprite))
	// 	spr_link(self->sprite);
	// //if the actor just moved off the screen
	// else if (wasOnScreen && !spr_onScreen(self->sprite))
	// 	spr_unlink(self->sprite);
}

//move the actor relative to it's current position in a level
void actor_move(Actor* self, Level* lvl, s32 dx, s32 dy) {
	actor_setPos(self, lvl, self->pos.x + dx, self->pos.y + dy);
}

//handles the state transitions of an actor
void actor_updateState(Actor* self, Level* lvl) {
	//TODO:
}

//=============================================================================
//BASE ACTOR THINGS
//=============================================================================

//base actor
static const Info actor_info_base = {
	.name = "Base Actor",
	.desc = "A generic actor with no special properties"
};

static void actor_die_base(Actor* self) {
	mgba_printf(LOG_INFO, "%s shuffles off this mortal coil!", self->info.name);
}

static void actor_idle_base(Actor* self) {
	mgba_printf(LOG_INFO, "%s does diddly-squat!", self->info.name);
}

static void actor_flee_base(Actor* self, Actor* other, Level* lvl) {
	mgba_printf(LOG_INFO, "%s runs for it's life from %s!", self->info.name, other->info.name);
}

static void actor_chase_base(Actor* self, Actor* other, Level* lvl) {
	mgba_printf(LOG_INFO, "%s relentlessly pursues %s!", self->info.name, other->info.name);
}

static void actor_attack_base(Actor* self, Actor* other) {
	mgba_printf(LOG_INFO, "%s donkey-punched %s!", self->info.name, other->info.name);
}

//=============================================================================
//SKELETON THINGS
//=============================================================================

//skeleton
static const Info actor_info_skeleton = {
	.name = "Skeleton",
	.desc = "The reanimated remains of a long-dead adventurer. Maybe it still has some of it's loot"
};

static void actor_die_skeleton(Actor* self) {
	actor_die_base(self);
}

static void actor_idle_skeleton(Actor* self) {
	actor_idle_base(self);
}

static void actor_flee_skeleton(Actor* self, Actor* other, Level* lvl) {
	actor_flee_base(self, other, lvl);
}

static void actor_chase_skeleton(Actor* self, Actor* other, Level* lvl) {
	actor_chase_base(self, other, lvl);
}

static void actor_attack_skeleton(Actor* self, Actor* other) {
	actor_attack_base(self, other);
}

//=============================================================================
//CONSTRUCTOR/DESTRUCTOR
//=============================================================================

Actor* actor_create(ActorType type, s32 x, s32 y) {
	Actor* self = malloc(sizeof(*self));
	self->type = type;
	switch(type) {
		case ACTOR_SKELETON:
			self->info = actor_info_skeleton;
			self->die = actor_die_skeleton;
			self->idle = actor_idle_skeleton;
			self->flee = actor_flee_skeleton;
			self->chase = actor_chase_skeleton;
			self->attack = actor_attack_skeleton;
			self->sprite = spr_create(0, 0, TID_SKELETON, 0);
			break;
		default:
			self->info = actor_info_base;
			self->die = actor_die_base;
			self->idle = actor_idle_base;
			self->flee = actor_flee_base;
			self->chase = actor_chase_base;
			self->attack = actor_attack_base;
			self->sprite = spr_create(0, 0, TID_ERR, 0);
	}

	return self;
}

void actor_destroy(Actor* self) {
	spr_destroy(self->sprite);
	free(self);
}
