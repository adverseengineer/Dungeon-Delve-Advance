//Nick Sells, 2021
//level.c

#include <stdlib.h>

#include "level.h"

//=============================================================================
// GLOBALS
//=============================================================================

//NOTE: used to store rooms and hallways when generating a level. gets malloc'd
//when needed and free'd when done. necessary because rooms and hallways need
//to know about one another to be placed in ways that don't suck.
static Room* rooms;
static Hall* halls;

//used to track which index to put a room or hall in
static size_t numRooms = 0;
static size_t numHalls = 0;

//=============================================================================
// CONSTRUCTORS/DESTRUCTORS
//=============================================================================

//allocates all the memory needed for a Level
Level* lvl_create(void) {
	Level* self = calloc(1, sizeof(*self));
	self->tiles = calloc(LVL_HEIGHT * LVL_WIDTH, sizeof(*self->tiles));
	self->terrain = calloc(LVL_HEIGHT * LVL_WIDTH, sizeof(*self->terrain));
	return self;
}

//deallocates the level
void lvl_destroy(Level* self) {
	for(u32 i = MAX_ACTORS; i > 0; i--)
		actor_destroy(self->actors[i]);

	free(self);
}

//claims the next available actor slot, sets it up, and returns a pointer to it
Actor* lvl_createActor(Level* self, ActorType type, u32 x, u32 y) {
	for(size_t i = 0; i < MAX_ACTORS; i++) {
		if(self->actors[i] == NULL) {
			//claim the slot and set up the actor
			self->actors[i] = actor_create(type);
			self->actors[i]->lvl = self;
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

//=============================================================================
//FUNCTION DEFINITIIONS
//=============================================================================

//scrolls a level around. also moves sprites and stops at edges
void lvl_scroll(Level* self) {

	u32 dx = 16 * key_tri_horz();
	u32 dy = 16 * key_tri_vert();

	//if the viewport is not already out of bounds
	if(in_range(self->offset.x + dx, 0, LVL_WIDTH * 16 - SCREEN_WIDTH)
	&& in_range(self->offset.y + dy, 0, LVL_HEIGHT * 16 - SCREEN_HEIGHT)) {
		
		mgba_printf(LOG_INFO, "")
		//scroll the level, but do not go into parallel universes
		self->offset.x = clamp(self->offset.x + dx, 0, LVL_WIDTH * 16 - SCREEN_WIDTH);
		self->offset.y = clamp(self->offset.y + dy, 0, LVL_HEIGHT * 16 - SCREEN_HEIGHT);		
		REG_BG_OFS[BG_LVL] = self->offset;

		//move the actors along with it
		for(size_t i = 0; i < MAX_ACTORS; i++)
			if(self->actors[i] != NULL)
				spr_move(self->actors[i]->sprite, -dx, -dy);
	}
}

//plots a level to the level sbb defined in config.h
void lvl_draw(const Level* self) {
	for(size_t y = 0; y < LVL_HEIGHT; y++) {
	for(size_t x = 0; x < LVL_WIDTH; x++) {
		bg_plot_m(SBB_LVL, x, y, lvl_getTile(self, x, y));
	}}
}

//erases a level so that the next level can be drawn
void lvl_erase(void) {
	for(size_t i = 0; i < 4; i++)
		SBB_CLEAR(SBB_LVL + i);
}

//=============================================================================
// LEVEL-BUILDING FUNCTIONS
//=============================================================================

//FIXME: hallways need to be prevented from intersecting a room at it's edge.
//this causes complications with the placement logic for actors, items, and
//doors and can create an ugly "missing corner" look where the hall ends, OR
//adjust each hallway's start and endpoint to where they intersect a room. i
//think either solution will work

//FIXME: prevent rooms connected by a door from touching each other; sharing a
//wall is okay, and so is being completely separate, but two connected room's
//walls can't be allowed to touch. it makes for ugly door placement

//TODO: special room types
//TODO: entry and exit rooms must be as far from each other as possible

//***TODO:*** limit what tiles hall floors can overwrite so that they don't cut so deeply between rooms
//this will allow me to rely on tile types to know where hallways apprarent stop and start

static void lvl_design(Level* self, RECT* rect, size_t numIterations);

static void lvl_deferRoom(const RECT* rect);
static void lvl_deferHall(const Hall* hall);

static void lvl_tweakRooms(void);
static void lvl_buildRooms(Level* self);
static void lvl_buildHalls(Level* self);
static void lvl_placeDoors(Level* self);
static void lvl_placeItems(Level* self);
static void lvl_placeActors(Level* self);

static void lvl_splitRect(const RECT* original, RECT* p1, RECT* p2, size_t numAttempts);

//places rooms, hallways, items, and enemies
void lvl_build(Level* self) {

	//FIXME: remove this before committing.
	profile_start();

	//FIXME: remove this before committing. it just lets me quickly cycle through dungeon builds
	for(size_t y = 0; y < LVL_HEIGHT; y++) {
	for(size_t x = 0; x < LVL_WIDTH; x++) {
		lvl_setTile(self, x, y, TILE_NONE);
	}}
	//FIXME: remove this too
	for(size_t i = 0; i < MAX_ACTORS; i++) {
		if(self->actors[i] != NULL)
			lvl_destroyActor(self, self->actors[i]);
	}



	//allocate the temp arrays to store things until they can be built
	rooms = calloc(1 << NUM_RECUR, sizeof(*rooms));
	halls = calloc((1 << NUM_RECUR) - 1, sizeof(*halls));

	//design the level, using an initial BSP partition
	RECT temp = {0, 0, LVL_WIDTH, LVL_HEIGHT};
	lvl_design(self, &temp, NUM_RECUR);

	//build the level
	lvl_tweakRooms();
	lvl_buildRooms(self);
	lvl_buildHalls(self);
	lvl_placeDoors(self);
	lvl_placeItems(self);
	lvl_placeActors(self);

	//deallocate the temp arrays now that everything is built
	free(rooms);
	free(halls);

	//reset the indices for those
	numRooms = 0;
	numHalls = 0;

	size_t numCycles = profile_stop();
	mgba_printf(LOG_INFO, "level generated in %u cycles", numCycles);
}

//recursively divides an initial surface using BSP and queues up rooms and hallways to be carved later
static inline void lvl_design(Level* self, RECT* rect, size_t numIterations) {
	
	//recursive case: if we are not on the last iteration
	if(numIterations > 0) {

		//split rect into r1 and r2
		RECT r1, r2;
		lvl_splitRect(rect, &r1, &r2,SPLIT_ATTEMPTS);

		//recurse with the two pieces
		lvl_design(self, &r1, numIterations - 1);
		lvl_design(self, &r2, numIterations - 1);

		//calculate the hallway which will connect the two halves
		Hall hall = {
			.start = {(r1.left + r1.right) / 2, (r1.top + r1.bottom) / 2},
			.end = {(r2.left + r2.right) / 2, (r2.top + r2.bottom) / 2}
		};

		//queue up the hallway to be built later
		lvl_deferHall(&hall);
	}
	//base case: if we are on the last iteration (AKA: a leaf of the tree)
	else
		//queue up the rect to be built as a room later
		lvl_deferRoom(rect);
}

//takes a rectangle and copies it as the bounds of a room in the next slot of the room array
static inline void lvl_deferRoom(const RECT* rect) {
	rooms[numRooms++] = (Room) {.bounds = *rect};
}

//takes a hallway and copies it to the next slot of the hall array
static inline void lvl_deferHall(const Hall* hall) {
	halls[numHalls++] = *hall;
}

//iterates over the room array and adjusts each room to be a bit more visually appealing
static inline void lvl_tweakRooms(void) {

	//for every room in the temp array
	for(size_t i = 0; i < numRooms; i++) {

		u32 vertShrink = qran_range(0, rc_height(&rooms[i].bounds) / 2);
		u32 horzShrink = qran_range(0, rc_width(&rooms[i].bounds) / 2);

		//if shrinking would make the room too small, don't bother
		if(rc_height(&rooms[i].bounds) - vertShrink < MIN_ROOM_HEIGHT) vertShrink = 0;
		if(rc_width(&rooms[i].bounds) - horzShrink < MIN_ROOM_WIDTH) horzShrink = 0;

		//recede the left or right edge
		if(qran_range(0, 2))
			rooms[i].bounds.left += horzShrink;
		else
			rooms[i].bounds.right -= horzShrink;

		//recede the top or bottom edge
		if(qran_range(0, 2))
			rooms[i].bounds.top += vertShrink;
		else
			rooms[i].bounds.bottom -= vertShrink;
	}
}

//iterates over the room array and plots each room's tiles to the level
static inline void lvl_buildRooms(Level* self) {
	
	//for every room in the temp array
	for(size_t i = 0; i < numRooms; i++) {

		//place floor tiles
		for(size_t y = rooms[i].bounds.top + 1; y < rooms[i].bounds.bottom - 1; y++) {
		for(size_t x = rooms[i].bounds.left + 1; x < rooms[i].bounds.right - 1; x++) {
			lvl_setTile(self, x, y, TILE_FLOOR_ROOM);
		}}

		//place north and south wall tiles
		for(size_t x = rooms[i].bounds.left; x < rooms[i].bounds.right; x++) {
			lvl_setTile(self, x, rooms[i].bounds.top, TILE_WALL);
			lvl_setTile(self, x, rooms[i].bounds.bottom - 1, TILE_WALL);
		}

		//place west and east wall tiles
		for(size_t y = rooms[i].bounds.top + 1; y < rooms[i].bounds.bottom - 1; y++) {
			lvl_setTile(self, rooms[i].bounds.left, y, TILE_WALL);
			lvl_setTile(self, rooms[i].bounds.right - 1, y, TILE_WALL);
		}
	}
}

//iterates over the hall array and plots each one's tiles to the level
static inline void lvl_buildHalls(Level* self) {

	//for every hall in the temp array
	for(size_t i = 0; i < numHalls; i++) {

		//vertical hallway
		if(halls[i].start.x == halls[i].end.x) {
			for(size_t y = halls[i].start.y; y < halls[i].end.y; y++) {
				//left wall
				if(lvl_getTile(self, halls[i].start.x - 1, y) == TILE_NONE)
					lvl_setTile(self, halls[i].start.x - 1, y, TILE_WALL);

				//floor (middle)
				if(lvl_getTile(self, halls[i].start.x, y) != TILE_FLOOR_ROOM)
					lvl_setTile(self, halls[i].start.x, y, TILE_FLOOR_HALL);
	
				//right wall
				if(lvl_getTile(self, halls[i].start.x + 1, y) == TILE_NONE)
					lvl_setTile(self, halls[i].start.x + 1, y, TILE_WALL);
			}
		}
		//horizontal hallway
		else if(halls[i].start.y == halls[i].end.y){
			for(size_t x = halls[i].start.x; x < halls[i].end.x; x++) {
				//top wall
				if(lvl_getTile(self, x, halls[i].start.y - 1) == TILE_NONE)
					lvl_setTile(self, x, halls[i].start.y - 1, TILE_WALL);

				//floor (middle)
				if(lvl_getTile(self, x, halls[i].start.y) != TILE_FLOOR_ROOM)
					lvl_setTile(self, x, halls[i].start.y, TILE_FLOOR_HALL);
	
				//bottom wall
				if(lvl_getTile(self, x, halls[i].start.y + 1) == TILE_NONE)
					lvl_setTile(self, x, halls[i].start.y + 1, TILE_WALL);
			}
		}
		else
			mgba_printf(LOG_ERR, "diagonal hallways are not supported");
	}
}

//iterates over the hall array and 
static inline void lvl_placeDoors(Level* self) {

	//for every hall in the temp array
	for(size_t i = 0; i < numHalls; i++) {

		//vertical hallway
		if(halls[i].start.x == halls[i].end.x) {

			//for every tile in the hallway, from top to bottom
			for(size_t y = halls[i].start.y; y < halls[i].end.y; y++) {
				//if there is a wall to the left and right
				if((lvl_getTile(self, halls[i].start.x - 1, y) == TILE_WALL)
				&& (lvl_getTile(self, halls[i].start.x + 1, y) == TILE_WALL)) {
					//place a door and abort the loop
					lvl_setTile(self, halls[i].start.x, y, TILE_DOOR_CLOSED);
					break;
				}
			}
		}
		//horizontal hallway
		else if(halls[i].start.y == halls[i].end.y){

			//for every tile in the hallway, from left to right
			for(size_t x = halls[i].start.x; x < halls[i].end.x; x++) {
				//if there is a wall above and below
				if((lvl_getTile(self, x, halls[i].start.y - 1) == TILE_WALL)
				&& (lvl_getTile(self, x, halls[i].start.y + 1) == TILE_WALL)) {
					//place a door and abort the loop
					lvl_setTile(self, x, halls[i].start.y, TILE_DOOR_CLOSED);
					break;
				}
			}
		}
		else
			mgba_printf(LOG_ERR, "diagonal hallways are not supported");
	}
}

//iterates over the room array and places items in accordance with the loot tables
static void lvl_placeItems(Level* self) {
	//TODO:
}

//iterates over the room array and places enemies where the conditions are right
//NOTE: this will need significant reworking after the jam
static void lvl_placeActors(Level* self) {
		
	#define SPAWN_CHANCE 153 //60%
	u32 numActors = 0;

	for(size_t i = 0; i < numRooms; i++) {
		if(numActors < MAX_ACTORS) {
			u32 spawnX, spawnY;
			do {
				spawnX = qran_range(rooms[i].bounds.left, rooms[i].bounds.right + 1);
				spawnY = qran_range(rooms[i].bounds.top, rooms[i].bounds.bottom + 1);
			}
			while(lvl_getTile(self, spawnX, spawnY) != TILE_FLOOR_ROOM);

			lvl_createActor(self, ACTOR_SKELETON, spawnX, spawnY);
		}
	}

	//TODO:
}

//splits original rect in half and stores the halves in p1 and p2. attempts to
//satisfy the ratio specified by DISCARD_RATIO, but gives up after numAttempts
static inline void lvl_splitRect(const RECT* original, RECT* p1, RECT* p2, size_t numAttempts) {

	//as long as there are attempts remaining
	for(size_t i = numAttempts; i > 0; i--) {

		//the halvles must start the same size as the original
		*p1 = *original;
		*p2 = *original;

		//choose vertical or horizontal axis
		BOOL axis = qran_range(0, 2);
		if(axis) { //vertical
			u32 cutPosition = qran_range(0, rc_width(p1));
			p1->right -= cutPosition;
			p2->left = p1->right - 1;
		}
		else { //horizontal
			u32 cutPosition = qran_range(0, rc_height(p1));
			p1->bottom -= cutPosition;
			p2->top = p1->bottom - 1;
		}

		//calculate the w/h or h/w ratios of each piece
		FIXED p1Ratio, p2Ratio;
		if(axis) { //vertical
			p1Ratio = fxdiv(int2fx(rc_width(p1)), int2fx(rc_height(p1)));
			p2Ratio = fxdiv(int2fx(rc_width(p2)), int2fx(rc_height(p2)));
		}
		else { //horizontal
			p1Ratio = fxdiv(int2fx(rc_height(p1)), int2fx(rc_width(p1)));
			p2Ratio = fxdiv(int2fx(rc_height(p2)), int2fx(rc_width(p2)));
		}

		//if neither ratio is too small, return
		if((p1Ratio > DISCARD_RATIO) && (p2Ratio > DISCARD_RATIO))
			break;
	}
}
