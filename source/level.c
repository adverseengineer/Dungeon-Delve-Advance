//Nick Sells, 2021
//level.c

#include "actor_level_common.h"

//=============================================================================
//CONSTRUCTOR/DESTRUCTOR
//=============================================================================

//allocate all the memory needed for a Level
Level* lvl_create(void) {
	Level* self = (Level*) calloc(1, sizeof(*self));
	return self;
}

//deallocate the level
void lvl_destroy(Level* self) {
	for(u32 i = MAX_ACTORS; i > 0; i--)
		actor_destroy(self->actors[i]);

	free(self);
}

//=============================================================================
//FUNCTION DEFINITIIONS
//=============================================================================

//creates an actor bound to this level and returns a pointer to it
Actor* lvl_spawnActor(Level* self, ActorType type, u32 x, u32 y) {
	for(u32 i = 0; i < MAX_ACTORS; i++) {
		if(self->actors[i] == NULL) {
			self->actors[i] = actor_create(type, x, y);
			actor_setPos(self->actors[i], self, x, y);
			return self->actors[i];
		}
	}
	return NULL;
}

//returns a visual tile from the level
TileType lvl_getTile(const Level* self, u32 x, u32 y) {
	return self->tiles[x + y * LVL_WIDTH];
}

//sets a visual tile in the level
void lvl_setTile(Level* self, u32 x, u32 y, TileType tile) {
	self->tiles[x + y * LVL_WIDTH] = tile;
}

//gets a collision tile from the level
TerrainType lvl_getTerrain(const Level* self, u32 x, u32 y) {
	return self->terrain[x + y * LVL_WIDTH];
}

//sets a collision tile in the level
void lvl_setTerrain(Level* self, u32 x, u32 y, TerrainType terrain) {
	self->terrain[x + y * LVL_WIDTH] = terrain;
}

//places rooms, hallways, items, and enemies
void lvl_build(Level* self) {

	for(u32 x = 0; x < LVL_WIDTH; x++) {
		if(in_range(qran_range(0, 64), 0, 56))
			lvl_setTile(self, x, 0, TILE_WALL);
		else
			lvl_setTile(self, x, 0, TILE_WALL_ALT);
	}
	
	for(u32 y = 1; y < LVL_HEIGHT; y++) {
		if(in_range(qran_range(0, 64), 0, 56))
			lvl_setTile(self, 0, y, TILE_WALL);
		else
			lvl_setTile(self, 0, y, TILE_WALL_ALT);
	}

	for(u32 y = 1; y < LVL_HEIGHT; y++) {
	for(u32 x = 1; x < LVL_WIDTH; x++) {
		u32 roll = qran_range(0, 64);
		if(in_range(roll, 0, 32))
			lvl_setTile(self, x, y, TILE_FLOOR_ROOM);
		else if(in_range(roll, 32, 48))
			lvl_setTile(self, x, y, TILE_FOLIAGE);
		else if(in_range(roll, 48, 64))
			lvl_setTile(self, x, y, TILE_WATER);
	}}

	lvl_setTile(self, 2, 2, TILE_EXIT);
	lvl_setTile(self, 1, 4, TILE_WALL);
	lvl_setTile(self, 2, 4, TILE_DOOR_CLOSED);
	lvl_setTile(self, 3, 4, TILE_WALL);
	lvl_setTile(self, 4, 4, TILE_WALL);
	lvl_setTile(self, 4, 3, TILE_WALL);
	lvl_setTile(self, 4, 2, TILE_DOOR_OPEN);
	lvl_setTile(self, 4, 1, TILE_WALL);
}

static u32 scroll_timer = 0;
void lvl_scroll(Level* self) {

	if(scroll_timer == 0) {
		u32 dx = key_tri_horz();
		u32 dy = key_tri_vert();
		self->offset.x += (dx * 16);
		self->offset.y += (dy * 16);
		REG_BG_OFS[BG_LVL] = self->offset;

		for(u32 i = 0; i < MAX_ACTORS; i++) {
			if(self->actors[i] != NULL) {
				//NOTE: does this desync actor's pos with their sprite pos? does it matter?
				spr_move(self->actors[i]->sprite, -dx * 16, -dy * 16);
			}
		}

		scroll_timer = 6;
	}
	else
		scroll_timer--;
}

//plots a level to the level sbb defined in config.h
void lvl_draw(const Level* self) {
	for(u32 y = 0; y < LVL_HEIGHT; y++) {
	for(u32 x = 0; x < LVL_WIDTH; x++) {
		bg_plot_m(SBB_LVL, x, y, lvl_getTile(self, x, y));
	}}
}
