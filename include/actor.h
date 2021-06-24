//Nick Sells, 2021
//actor.h

#ifndef ACTOR_H
#define ACTOR_H

#include <tonc.h>

#include <stdlib.h>

#include "config.h"
#include "info.h"
#include "level.h"
#include "sprite.h"

typedef struct Level ALIGN4 Level;

//=============================================================================
// TYPE DEFINITIONS
//=============================================================================

//used by actors to mimic polymorphism
typedef enum ActorType {
	ACTOR_BASE, //a basic actor type. in C++, this would be an abstract base class
	ACTOR_PLAYER,
	ACTOR_SKELETON
} ALIGN4 ActorType;

//every turn, the actor's state dictates what action it will take
typedef enum ActorState {
	ACTOR_DEAD,
	/*any actor in this state will be destroyed at the end of the turn*/
	
	ACTOR_IDLE,
	/*any actor in this state will waste it's turn*/
	
	ACTOR_FLEEING,
	/*when in this state, the actor will set it's target to the far corner of
	the quadrant opposite the one containing the previous target*/
	
	ACTOR_CHASING,
	/*when in this state, the actor will pathfind to it's target. if it gets
	within reach of the target, it will switch to attacking*/
	
	ACTOR_ATTACKING
	/*when in this state, if the target is within reach, it will attack
	otherwise, it will switch to chasing*/ 
} ALIGN4 ActorState;

//models an actor in a level
typedef struct Actor ALIGN4 Actor; //necessary so that Actor can contain pointers to itself
struct Actor {
	ActorType type; //the "subclass" of the actor
	ActorState state; //the actor's current action
	Info info; //contains the name and description of this actor

	POINT16 pos; //the actor's position in the level
	Level* lvl; //the level that this actor exists in
	Sprite* sprite; //the on-screen representation of this actor

	//"methods"
	void (*move) (Actor* this);
	void (*attack) (Actor* this, Actor* other);
	void (*chase) (Actor* this, Actor* other);
	void (*flee) (Actor* this, Actor* other);
	void (*idle) (Actor* this);
	void (*die) (Actor* this);	
};

//=============================================================================
// FORWARD-DECLARATIONS
//=============================================================================

Actor* actor_create(ActorType type);
void actor_destroy(Actor* this);

void actor_setPos(Actor* this, Level* lvl, s32 x, s32 y);

#endif //ACTOR_H
