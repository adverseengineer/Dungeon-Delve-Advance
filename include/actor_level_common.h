//Nick Sells, 2021
//actor_level_common.h

//This file SUCKS. It's only purpose is to resolve a cyclic dependency between
//actor.h and level.h by definining all their structs and enums and declaring
//each of their functions. This is a sloppy solution but this is a jam game and
//I've already fucked around with this for three weeks trying to couple actors
//to levels.

#ifndef ACTOR_LEVEL_COMMON_H
#define ACTOR_LEVEL_COMMON_H

#include "config.h"
#include "info.h"
#include "sprite.h"

//=============================================================================
//TYPE DECLARATIONS
//=============================================================================

typedef struct Level ALIGN4 Level;
typedef struct Actor ALIGN4 Actor;

//=============================================================================
//TYPE DEFINITIONS FROM ACTOR.H
//=============================================================================

//used by actors to mimic polymorphism
typedef enum ActorType {
	ACTOR_BASE, //a basic actor type. in C++, this would be an abstract base class
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
struct Actor {
	ActorType type;
	ActorState state;
	Info info;

	POINT16 pos;
	POINT16 target;
	Sprite* sprite;

	//"methods"
	void (*die) (Actor* self);
	void (*idle) (Actor* self);
	void (*flee) (Actor* self, Actor* other, Level* lvl);
	void (*chase) (Actor* self, Actor* other, Level* lvl);
	void (*attack) (Actor* self, Actor* other);
};

//=============================================================================
//FUNCTION DECLARATIONS FROM ACTOR.H
//=============================================================================

Actor* actor_create(ActorType type, s32 x, s32 y);
void actor_destroy(Actor* self);

void actor_setPos(Actor* self, Level* lvl, s32 x, s32 y);
void actor_move(Actor* self, Level* lvl, s32 dx, s32 dy);
void actor_updateState(Actor* self, Level* lvl);

//============-================================================================
//TYPE DEFINITIONS FROM LEVEL.H
//=============================================================================

//used during level generation to influence room generation
typedef enum LevelTheme {
	THEME_NORMAL,
	THEME_FLOODED,
	THEME_OVERGROWN,
	THEME_CHASM,
	THEME_BOSS
} ALIGN4 LevelTheme;

//used during level generation to make unique rooms
typedef enum RoomType {
	ROOM_NORMAL,
	ROOM_ENTRY,
	ROOM_EXIT
} ALIGN4 RoomType;

//a dictionary of tiles to use during level generation
typedef enum TileType {
	TILE_NONE           = SE_NONE,
	TILE_WALL           = SE_WALL,
	TILE_WALL_ALT       = SE_WALL_ALT,
	TILE_DOOR_OPEN      = SE_DOOR_CLOSED,
	TILE_DOOR_CLOSED    = SE_DOOR_OPEN,
	TILE_ENTRY          = SE_LADDER_UP,
	TILE_EXIT           = SE_LADDER_DOWN,
	TILE_FLOOR_ROOM     = SE_FLOOR, //NOTE: two different types of floor tiles are necessary for item placement logic
	TILE_FLOOR_HALL     = SE_FLOOR,
	TILE_FOLIAGE        = SE_FOLIAGE,
	TILE_WATER          = SE_WATER,
	TILE_CHASM          = SE_CHASM
} ALIGN4 TileType;

//collision tiles to use for pathfinding
typedef enum TerrainType {
	TERRAIN_BLOCKED,
	TERRAIN_WALKABLE
} ALIGN4 TerrainType;

//models a level of the game
typedef struct Level {
	POINT16 offset;
	LevelTheme theme;
	TileType tiles[LVL_WIDTH * LVL_HEIGHT];
	TerrainType terrain[LVL_WIDTH * LVL_HEIGHT];
	Actor* actors[MAX_ACTORS];
} ALIGN4 Level;

//=============================================================================
//FUNCTION DECLARATIONS FROM LEVEL.H
//=============================================================================

Level* lvl_create(void);
void lvl_destroy(Level* self);

Actor* lvl_spawnActor(Level* self, ActorType type, u32 x, u32 y);
TileType lvl_getTile(const Level* self, u32 x, u32 y);
void lvl_setTile(Level* self, u32 x, u32 y, TileType tile);
TerrainType lvl_getTerrain(const Level* self, u32 x, u32 y);
void lvl_setTerrain(Level* self, u32 x, u32 y, TerrainType terrain);

void lvl_build(Level* self);

void lvl_scroll(Level* self);
void lvl_draw(const Level* self);


#endif //ACTOR_LEVEL_COMMON_H