//Nick Sells, 2021
//level.h

#ifndef LEVEL_H
#define LEVEL_H

#include "actor.h"

//=============================================================================
// TYPE DEFINITIONS
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
	ROOM_ENTRY, //contains the stairs to the previous depth
	ROOM_EXIT, //contains the stairs to the next depth
	ROOM_VAULT, //contains small piles of gold
	ROOM_LIBRARY, //contains bookshelves on one wall
	ROOM_FOUNTAIN //contains a water fountain 
} ALIGN4 RoomType;

//a dictionary of tiles to use during level generation
typedef enum TileType {
	TILE_NONE           = SE_NONE,
	TILE_WALL           = SE_WALL,
	TILE_WALL_ALT       = SE_WALL_ALT,
	TILE_DOOR_OPEN      = SE_DOOR_OPEN,
	TILE_DOOR_CLOSED    = SE_DOOR_CLOSED,
	TILE_ENTRY          = SE_LADDER_UP,
	TILE_EXIT           = SE_LADDER_DOWN,
	TILE_FLOOR_ROOM     = SE_FLOOR, //NOTE: two different types of floor tiles are necessary for item placement logic
	TILE_FLOOR_HALL     = SE_FLOOR,
	TILE_FOLIAGE        = SE_FOLIAGE,
	TILE_EMBERS         = SE_EMBERS,
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

	TileType* tiles;
	TerrainType* terrain;
	
	Actor* player;
	Actor* actors[MAX_ACTORS];

	// TILE map[(LVL_HEIGHT / 2) * (LVL_WIDTH / 2)]; //NOTE: use this to dynamically create map tiles for the level
	//NOTE: you might as well also use this to implement a fog-of-war
	//NOTE: tiles are rendered horizontally mirrorred
} ALIGN4 Level;

//used to store a room and it's type until it can be built
typedef struct Room {
	RoomType type;
	RECT bounds;
} ALIGN4 Room;

//used to store a hall until it can be built
typedef struct Hall {
	POINT16 start;
	POINT16 end;
} ALIGN4 Hall;

//=============================================================================
// EXTERNS
//=============================================================================

extern Level* lvl_create(void);
extern void lvl_destroy(Level* self);

extern Actor* lvl_createActor(Level* self, ActorType type, u32 x, u32 y);
extern void lvl_destroyActor(Level* self, Actor* actor);

extern void lvl_scroll(Level* self);
extern void lvl_draw(const Level* self);

extern void lvl_build(Level* self);
extern void lvl_erase(void);

//=============================================================================
// INLINES
//=============================================================================

//returns a visual tile from the level
static inline TileType lvl_getTile(const Level* self, u32 x, u32 y) {
	return self->tiles[x + y * LVL_WIDTH];
}

//sets a visual tile in the level
static inline void lvl_setTile(Level* self, u32 x, u32 y, TileType tile) {
	self->tiles[x + y * LVL_WIDTH] = tile;
}

//gets a collision tile from the level
static inline TerrainType lvl_getTerrain(const Level* self, u32 x, u32 y) {
	return self->terrain[x + y * LVL_WIDTH];
}

//sets a collision tile in the level
static inline void lvl_setTerrain(Level* self, u32 x, u32 y, TerrainType terrain) {
	self->terrain[x + y * LVL_WIDTH] = terrain;
}

#endif //ACTOR_H