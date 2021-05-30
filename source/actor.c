//Nick Sells, 2021
//actor.c

#include <stdlib.h>
#include "actor.h"

//=============================================================================
//BASE ACTOR THINGS
//=============================================================================

//base actor
static const u32 actor_maxHP_base = 20;
static const u32 actor_atk_base = 5;
static const Info actor_info_base = {
	.name = "Base Actor",
	.desc = "A generic actor with no special properties"
};

void actor_idle_base(Actor* self) {
	mgba_printf(LOG_INFO, "%s wanders!", self->info.name);
}

void actor_attack_base(Actor* self, Actor* other) {
	if(other->isAlive) {
		if(other->currentHP <= self->atk) {
			other->die(other);
			mgba_printf(LOG_INFO, "%s was killed by %s", other->info.name, self->info.name);
		}
		else {
			other->currentHP -= self->atk;
			mgba_printf(LOG_INFO, "%s attacked %s for %d points of damage!", self->info.name, other->info.name, self->atk);
		}
	}
}

void actor_die_base(Actor* self) {
	self->isAlive = FALSE;
	mgba_printf(LOG_INFO, "%s died!", self->info.name);
}

//=============================================================================
//SKELETON THINGS
//=============================================================================

//skeleton
static const u32 actor_maxHP_skeleton = 15;
static const u32 actor_atk_skeleton = 3;
static const Info actor_info_skeleton = {
	.name = "Skeleton",
	.desc = "The reanimated remains of a long-dead adventurer. Maybe it still has some of it's loot"
};

void actor_idle_skeleton(Actor* self) {
	actor_idle_base(self);
}

void actor_attack_skeleton(Actor* self, Actor* other) {
	actor_attack_base(self, other);
}

void actor_die_skeleton(Actor* self) {
	actor_die_base(self);
}

//=============================================================================
//CONSTRUCTOR/DESTRUCTOR
//=============================================================================

Actor* actor_create(ActorType type, s32 x, s32 y) {
	Actor* self = malloc(sizeof(*self));
	self->type = type;
	actor_setPos(self, x, y);
	self->isAlive = true;
	switch(type) {
		case ACTOR_SKELETON:
			self->info = actor_info_skeleton;
			self->currentHP = actor_maxHP_skeleton;
			self->atk = actor_atk_base;
			self->idle = actor_idle_skeleton;
			self->attack = actor_attack_skeleton;
			self->die = actor_die_skeleton;
			self->sprite = spr_create(x, y, 0, 0);
			break;
		default:
			self->info = actor_info_base;
			self->currentHP = actor_maxHP_base;
			self->atk = actor_atk_skeleton;
			self->idle = actor_idle_base;
			self->attack = actor_attack_base;
			self->die = actor_die_base;
			self->sprite = spr_create(x, y, 0, 0);
	}

	spr_link(self->sprite);

	return self;
}

void actor_destroy(Actor* self) {
	spr_destroy(self->sprite);
	free(self);
}
