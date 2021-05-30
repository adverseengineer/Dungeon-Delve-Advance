//Nick Sells, 2021
//actor.h

#ifndef ACTOR_H
#define ACTOR_H

#include <tonc.h>

#include "info.h"
#include "sprite.h"

//=============================================================================
//STRUCTS AND ENUMS
//=============================================================================

typedef enum ActorType {
	ACTOR_BASE,                 //generic actor type. in c++ this would be an abstract base class
	ACTOR_SKELETON
} ActorType;

typedef struct Actor ALIGN4 Actor;
struct Actor {
	//meta stuff
	ActorType type;
	Info info;
	//visual things
	Sprite* sprite;
	POINT16 pos;
	//rpg things
	BOOL isAlive;
	u32 currentHP;
	u32 atk;
	//"method" pointers
	void (*idle)(Actor* self);
	void (*attack)(Actor* self, Actor* other);
	void (*die)(Actor* self);
};

//=============================================================================
//EXTERNS
//=============================================================================

extern Actor* actor_create(ActorType type, s32 x, s32 y);
extern void actor_destroy(Actor* self);

//=============================================================================
//INLINES
//=============================================================================

static inline void actor_setPos(Actor* self, s32 x, s32 y) {
	self->pos.x = x;
	self->pos.y = y;
	spr_setPos(self->sprite, x, y);
}

static inline void actor_move(Actor* self, s32 x, s32 y) {
	self->pos.x += x;
	self->pos.y += y;
	spr_move(self->sprite, x, y);
}

#endif //ACTOR_H
