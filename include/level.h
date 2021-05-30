//Nick Sells, 2021
//level.h

//provides functions and structs for generating a level

#ifndef LEVEL_H
#define LEVEL_H

#include <stdlib.h>
#include <tonc.h>

#include "common.h"
#include "config.h"
#include "actor.h"

//NOTE: this include isn't necessary now, but it will be eventually. I have no idea how ill resolve the circular dependency when it does
// #include "entity.h"

//=============================================================================
//STRUCTS AND ENUMS
//=============================================================================

typedef enum LevelType {
	LEVEL_NORMAL,
	LEVEL_FLOODED,
	LEVEL_OVERGROWN,
	LEVEL_CHASM,
	LEVEL_BOSS
} ALIGN4 LevelType;

typedef enum RoomType {
	ROOM_NORMAL,
	ROOM_ENTRY,
	ROOM_EXIT
} ALIGN4 RoomType;

typedef enum TileType {
	TILE_NONE           = SE_NONE,
	TILE_WALL           = SE_WALL,
	TILE_FLOOR_ROOM     = SE_FLOOR,   //two different types of floor tiles are necessary for item placement logic
	TILE_FLOOR_HALL     = SE_FLOOR,
	TILE_WATER          = SE_FLOOR_WATER,
	TILE_FOLIAGE        ,//TODO: draw this
	TILE_CHASM          ,//TODO: draw this
	TILE_DOOR_CLOSED    = SE_DOOR_OPEN,
	TILE_DOOR_OPEN      = SE_DOOR_CLOSED,
	TILE_ENTRY          = SE_LADDER_UP,
	TILE_EXIT           = SE_LADDER_DOWN
} ALIGN4 TileType;

typedef enum TerrainType {
	TERRAIN_BLOCKED,
	TERRAIN_WALKABLE
} ALIGN4 TerrainType;

typedef struct Level {
	POINT16 offset;
	LevelType theme;
	TileType tiles[LVL_WIDTH * LVL_HEIGHT];
	TerrainType terrain[LVL_WIDTH * LVL_HEIGHT];
	Actor* actors[MAX_ACTORS];
} ALIGN4 Level;

//=============================================================================
//INLINES
//=============================================================================

//allocate all the memory needed for a Level
static inline Level* lvl_create(void) {
	Level* lvl = (Level*) calloc(1, sizeof(*lvl));
	return lvl;
}

//deallocate the level
static inline void lvl_destroy(Level* lvl) {
	for(u32 i = MAX_ACTORS; i > 0; i--)
		free(lvl->actors[i]);

	free(lvl);
}

//returns a visual tile from the level
static inline u32 lvl_getTile(const Level* lvl, u32 x, u32 y) {
	if(lvl != NULL)
		return lvl->tiles[x + y * LVL_WIDTH];
	else
		return ERR;
}

//sets a visual tile in the level
static inline void lvl_setTile(Level* lvl, u32 x, u32 y, TileType tile) {
	if(lvl != NULL)
		lvl->tiles[x + y * LVL_WIDTH] = tile;
}

//gets a collision tile from the level
static inline u32 lvl_getTerrain(const Level* lvl, u32 x, u32 y) {
	if(lvl != NULL)
		return lvl->terrain[x + y * LVL_WIDTH];
	else
		return ERR;
}

//sets a collision tile in the level
static inline void lvl_setTerrain(Level* lvl, u32 x, u32 y, TerrainType terrain) {
	if(lvl != NULL)
		lvl->terrain[x + y * LVL_WIDTH] = terrain;
}

//places rooms, hallways, items, and enemies
static inline void lvl_build(Level* lvl) {
	for(u32 x = 0; x < LVL_WIDTH; x++) {
		lvl_setTile(lvl, x, 0, TILE_WALL);
		lvl_setTile(lvl, x, LVL_HEIGHT - 1, TILE_WALL);
	}
	for(u32 y = 1; y < LVL_HEIGHT - 1; y++) {
		lvl_setTile(lvl, 0, y, TILE_WALL);
		lvl_setTile(lvl, LVL_WIDTH - 1, y, TILE_WALL);
	}
	for(u32 y = 1; y < LVL_HEIGHT - 1; y++) {
	for(u32 x = 1; x < LVL_WIDTH - 1; x++) {
		lvl_setTile(lvl, x, y, TILE_FLOOR_ROOM);
	}}

	lvl_setTile(lvl, 2, 2, TILE_EXIT);
	lvl_setTile(lvl, 1, 4, TILE_WALL);
	lvl_setTile(lvl, 2, 4, TILE_DOOR_CLOSED);
	lvl_setTile(lvl, 3, 4, TILE_WALL);
	lvl_setTile(lvl, 4, 4, TILE_WALL);
	lvl_setTile(lvl, 4, 3, TILE_WALL);
	lvl_setTile(lvl, 4, 2, TILE_DOOR_OPEN);
	lvl_setTile(lvl, 4, 1, TILE_WALL);
}

//NOTE: REMOVE ME BEFORE COMMITTING
#include <tonc_input.h>
static inline void lvl_scroll(Level* lvl) {
	lvl->offset.x += (key_tri_horz() * 3);
	lvl->offset.y += (key_tri_vert() * 3);
}

//plots a level to the level sbb defined in config.h
static inline void lvl_draw(const Level* lvl) {
	REG_BG_OFS[BG_LVL] = lvl->offset;

	for(u32 y = 0; y < LVL_HEIGHT; y++) {
	for(u32 x = 0; x < LVL_WIDTH; x++) {
		bg_plot_m(SBB_LVL, x, y, lvl_getTile(lvl, x, y));
	}}
}

#endif //LEVEL_H