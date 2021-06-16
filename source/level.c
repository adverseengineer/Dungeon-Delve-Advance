//Nick Sells, 2021
//level.c

#include <stdlib.h>

#include "level.h"

//=============================================================================
//CONSTRUCTOR/DESTRUCTOR
//=============================================================================

//actor contains a pointer to the level it inhabits
//actors are stored in a list
//actor needs to include level

//level does not need to know about actors

//allocate all the memory needed for a Level
Level* lvl_create(void) {
	Level* self = calloc(1, sizeof(*self));
	self->tiles = calloc(LVL_HEIGHT * LVL_WIDTH, sizeof(*self->tiles));
	self->terrain = calloc(LVL_HEIGHT * LVL_WIDTH, sizeof(*self->terrain));
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

//claims the next available actor slot, sets it up, and returns a pointer to it
//TODO: test
Actor* lvl_createActor(Level* self, ActorType type, u32 x, u32 y) {
	for(size_t i = 0; i < MAX_ACTORS; i++) {
		if(self->actors[i] == NULL) {
			//claim the slot and set up the actor
			self->actors[i] = actor_create(type);
			actor_setPos(self->actors[i], self, x, y);
			return self->actors[i];
		}
	}
	//if no slot was available,
	mgba_printf(LOG_ERR, "failed to create actor: no actor slots available");
	return NULL;
}

//relinquishes an actor slot
void lvl_destroyActor(Level* self, Actor* actor) {
	actor_destroy(actor);
	actor = NULL;
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

	for(size_t x = 0; x < LVL_WIDTH; x++) {
		if(in_range(qran_range(0, 64), 0, 56))
			lvl_setTile(self, x, 0, TILE_WALL);
		else
			lvl_setTile(self, x, 0, TILE_WALL_ALT);

		if(in_range(qran_range(0, 64), 0, 56))
			lvl_setTile(self, x, LVL_HEIGHT - 1, TILE_WALL);
		else
			lvl_setTile(self, x, LVL_HEIGHT - 1, TILE_WALL_ALT);
	}
	
	for(size_t y = 1; y < LVL_HEIGHT; y++) {
		if(in_range(qran_range(0, 64), 0, 56))
			lvl_setTile(self, 0, y, TILE_WALL);
		else
			lvl_setTile(self, 0, y, TILE_WALL_ALT);

		if(in_range(qran_range(0, 64), 0, 56))
			lvl_setTile(self, LVL_WIDTH - 1, y, TILE_WALL);
		else
			lvl_setTile(self, LVL_WIDTH - 1, y, TILE_WALL_ALT);
	}

	for(size_t y = 1; y < LVL_HEIGHT - 1; y++) {
	for(size_t x = 1; x < LVL_WIDTH - 1; x++) {
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

//scrolls a level around. also moves sprites and stops at edges
void lvl_scroll(Level* self) {

	u32 dx = 2 * key_tri_horz();
	u32 dy = 2 * key_tri_vert();

	//scroll the level
	self->offset.x += dx;
	self->offset.y += dy;
	REG_BG_OFS[BG_LVL] = self->offset;
	//move the actors along with it
	for(size_t i = 0; i < MAX_ACTORS; i++)
		if(self->actors[i] != NULL)
			spr_move(self->actors[i]->sprite, -dx, -dy);
}

//plots a level to the level sbb defined in config.h
void lvl_draw(const Level* self) {
	for(size_t y = 0; y < LVL_HEIGHT; y++) {
	for(size_t x = 0; x < LVL_WIDTH; x++) {
		bg_plot_m(SBB_LVL, x, y, lvl_getTile(self, x, y));
	}}
}
