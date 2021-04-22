//Nick Sells, 2021
//level.c

//lvl_splitRect and lvl_carve adapted from https://eskerda.com/bsp-Level-generation/

#include <stdlib.h>
#include <tonc_mgba.h>
#include <tonc_video.h>
#include <tonc_input.h>
#include "level.h"

//=============================================================================
//FORWARD-DECLARATIONS
//=============================================================================

Level* lvl_create(cu32 width, cu32 height);
void lvl_destroy(Level* lvl);
void lvl_carve(Level* lvl, RECT* rect, cu32 iterations);
void lvl_draw(const Level* lvl, cu32 sbb);
static inline void lvl_splitRect(const RECT* original, RECT* p1, RECT* p2, cu32 attempts);
static inline void lvl_padRoom(RECT* rect);
static inline void lvl_carveRoom(Level* lvl, RECT* rect);
static inline void lvl_carveHallway(Level* lvl, POINT* p1, POINT* p2);

//=============================================================================
//FUNCTIONS
//=============================================================================

//allocate all the memory needed for a Level
Level* lvl_create(cu32 width, cu32 height) {
	
	Level* lvl = (Level*) calloc(1, sizeof(Level));
	lvl->width = width;
	lvl->height = height;
	lvl->tiles = (u8*) calloc(1, width * height * sizeof(u8));
	
	//allocate the enemy array itself and then the enemies
	lvl->enemies = (Enemy**) calloc(MAX_ENEMIES, MAX_ENEMIES * sizeof(Enemy*));
	for(u32 i = 0; i < MAX_ENEMIES; i++)
		lvl->enemies[i] = (Enemy*) calloc(1, sizeof(Enemy));
	
	return lvl;
}


//deallocate the level
void lvl_destroy(Level* lvl) {
	
	//free each enemy and then the array itself
	for(u32 i = MAX_ENEMIES; i > 0; i--)
		free(lvl->enemies[i]);
	
	free(lvl->enemies);
	free(lvl->tiles);
	free(lvl);
}


//carves out the rooms and hallways of the level
void lvl_carve(Level* lvl, RECT* rect, cu32 iterations) {
	//recursive case: if we are not on the last iteration
	if(iterations > 0) {

		//split rect into r1 and r2
		RECT r1, r2;
		lvl_splitRect(rect, &r1, &r2, SPLIT_ATTEMPTS);
		
		//calculate the center of each piece
		//NOTE: these points need to be calculated before recursing so that the halls aren't diagonal
		POINT r1_center = {(r1.left + r1.right) / 2, (r1.top + r1.bottom) / 2};
		POINT r2_center = {(r2.left + r2.right) / 2, (r2.top + r2.bottom) / 2};

		//recurse with the two pieces
		lvl_carve(lvl, &r1, iterations - 1);
		lvl_carve(lvl, &r2, iterations - 1);

		//carve the hallway between the two points
		lvl_carveHallway(lvl, &r1_center, &r2_center);
	}
	//base case: if we are on the last iteration (read: a leaf of the tree)
	else {
		lvl_padRoom(rect);
		//TODO: decide the type of room and paint it accordingly
		lvl_carveRoom(lvl, rect);
	}
}


//plots a level to an sbb
void lvl_draw(const Level* lvl, cu32 sbb) {

	//TODO: where the fuck do i put these?
	#define SE_NONE         0
	#define SE_WALL         4
	#define SE_WALL_ALT     8
	#define SE_FLOOR        12
	#define SE_FLOOR_ALT    16
	#define SE_DOOR_CLOSED  20
	#define SE_DOOR_OPEN    24

	for(u32 y = 0; y < lvl->height; y++) {
	for(u32 x = 0; x < lvl->width; x++) {
		switch(LVL_GET_TILE(lvl, x, y)) {
			case TILE_WALL:
				se_plot(se_mem[sbb], x * 2, y * 2, SE_WALL);
				se_plot(se_mem[sbb], x * 2 + 1, y * 2, SE_WALL + 1);
				se_plot(se_mem[sbb], x * 2, y * 2 + 1, SE_WALL + 2);
				se_plot(se_mem[sbb], x * 2 + 1, y * 2 + 1, SE_WALL + 3);
				break;
			case TILE_FLOOR_ROOM:
			case TILE_FLOOR_HALL:
				se_plot(se_mem[sbb], x * 2, y * 2, SE_FLOOR);
				se_plot(se_mem[sbb], x * 2 + 1, y * 2, SE_FLOOR + 1);
				se_plot(se_mem[sbb], x * 2, y * 2 + 1, SE_FLOOR + 2);
				se_plot(se_mem[sbb], x * 2 + 1, y * 2 + 1, SE_FLOOR + 3);
				break;
			case TILE_DOOR_CLOSED:
				se_plot(se_mem[sbb], x * 2, y * 2, SE_DOOR_CLOSED);
				se_plot(se_mem[sbb], x * 2 + 1, y * 2, SE_DOOR_CLOSED + 1);
				se_plot(se_mem[sbb], x * 2, y * 2 + 1, SE_DOOR_CLOSED + 2);
				se_plot(se_mem[sbb], x * 2 + 1, y * 2 + 1, SE_DOOR_CLOSED + 3);
				break;
			case TILE_DOOR_OPEN:
				se_plot(se_mem[sbb], x * 2, y * 2, SE_DOOR_OPEN);
				se_plot(se_mem[sbb], x * 2 + 1, y * 2, SE_DOOR_OPEN + 1);
				se_plot(se_mem[sbb], x * 2, y * 2 + 1, SE_DOOR_OPEN + 2);
				se_plot(se_mem[sbb], x * 2 + 1, y * 2 + 1, SE_DOOR_OPEN + 3);
				break;
		}
	}}
}


//splits original rect in half and stores the halves in p1 and p2. attempts to
//satisfy the ratio specified by DISCARD_RATIO, but gives up after attempts is zero
//you can pass -1 for attempts, but it can potentially cause a stack overflow
static inline void lvl_splitRect(const RECT* original, RECT* p1, RECT* p2, cu32 attempts) {

	//the halvles must start the same size as the original
	*p1 = *original;
	*p2 = *original;

	//choose vertical or horizontal axis
	BOOL axis = qran_range(0, 2);	
	if(axis) {
		u32 amount = qran_range(0, rc_width(p1) + 1);
		p1->right -= amount;
		p2->left = p1->right;
	}
	else {
		u32 amount = qran_range(0, rc_height(p1) + 1);
		p1->bottom -= amount;
		p2->top = p1->bottom;
	}

	//calculate the w/h or h/w ratios of each piece
	FIXED p1_ratio, p2_ratio;
	if(axis) { //vertical
		p1_ratio = fxdiv(int2fx(rc_width(p1)), int2fx(rc_height(p1)));
		p2_ratio = fxdiv(int2fx(rc_width(p2)), int2fx(rc_height(p2)));
	}
	else { //horizontal
		p1_ratio = fxdiv(int2fx(rc_height(p1)), int2fx(rc_width(p1)));
		p2_ratio = fxdiv(int2fx(rc_height(p2)), int2fx(rc_width(p2)));	
	}

	//if either exceeds the limit, try again
	if((p1_ratio < DISCARD_RATIO) | (p2_ratio < DISCARD_RATIO))
		if(attempts > 0)
			lvl_splitRect(original, p1, p2, attempts - 1);
}


//randomizes the position and size of a room
static inline void lvl_padRoom(RECT* rect) {
	
	//randomly pad the room on each side
	u32 hpad_limit = rc_width(rect) / PADDING_FACTOR + 1;
	u32 vpad_limit = rc_height(rect) / PADDING_FACTOR + 1;
	u32 pad_l = qran_range(0, hpad_limit);
	u32 pad_t = qran_range(0, vpad_limit);
	u32 pad_r = qran_range(0, hpad_limit);
	u32 pad_b = qran_range(0, vpad_limit);

	//if the padding would make the room too small
	if((rc_width(rect) - pad_l - pad_r) > MIN_ROOM_SIZE) {
		rect->left += pad_l;
		rect->right -= pad_r;
	}
	if((rc_height(rect) - pad_t - pad_b) > MIN_ROOM_SIZE) {
		rect->top += pad_t;
		rect->bottom -= pad_b;
	}

	//NOTE: this prevents any room from touching. it makes the levels really boring.
	//this is a temporary fix until i can figure out how to allow rooms to touch
	//without interfering with door placement logic
	rect->right--;
	rect->bottom--;
}


//carves a room into the level according to the passed rectangle
static inline void lvl_carveRoom(Level* lvl, RECT* rect) {
	//place floor tiles
	for(u32 y = rect->top + 1; y < rect->bottom - 1; y++)
	for(u32 x = rect->left + 1; x < rect->right - 1; x++)
		LVL_SET_TILE(lvl, x, y, TILE_FLOOR_ROOM);				

	//place vertical wall tiles
	for(u32 y = rect->top + 1; y < rect->bottom - 1; y++) {
		//west wall
		if(LVL_GET_TILE(lvl, rect->left - 1, y) == TILE_FLOOR_HALL)
			LVL_SET_TILE(lvl, rect->left - 1, y, TILE_DOOR_CLOSED);
		else
			LVL_SET_TILE(lvl, rect->left, y, TILE_WALL);
		
		//east wall
		if(LVL_GET_TILE(lvl, rect->right, y) == TILE_FLOOR_HALL)
			LVL_SET_TILE(lvl, rect->right, y, TILE_DOOR_CLOSED);
		else
			LVL_SET_TILE(lvl, rect->right - 1, y, TILE_WALL);
	}

	// //place horizontal wall tiles
	for(u32 x = rect->left; x < rect->right; x++) {
		//north wall
		if(LVL_GET_TILE(lvl, x, rect->top) == TILE_FLOOR_HALL)
			LVL_SET_TILE(lvl, x, rect->top, TILE_DOOR_CLOSED);
		else
			LVL_SET_TILE(lvl, x, rect->top, TILE_WALL);

		//south wall
		if(LVL_GET_TILE(lvl, x, rect->bottom) == TILE_FLOOR_HALL)
			LVL_SET_TILE(lvl, x, rect->bottom, TILE_DOOR_CLOSED);
		else
			LVL_SET_TILE(lvl, x, rect->bottom - 1, TILE_WALL);
	}
}


//carves a hallway into the level between two points
static inline void lvl_carveHallway(Level* lvl, POINT* p1, POINT* p2) {
	
	BOOL axis = p1->x == p2->x;

	//if the hallway is vertical
	if(axis) {
		for(u32 y = p1->y; y < p2->y; y++) {

			//place hallway floor tiles
			LVL_SET_TILE(lvl, p1->x, y, TILE_FLOOR_HALL);

			//place wall tiles only if there is nothing already there
			//left wall of hallway
			if(LVL_GET_TILE(lvl, p1->x - 1, y) == TILE_NONE)
				LVL_SET_TILE(lvl, p1->x - 1, y, TILE_WALL);
			//right wall of hallway
			if(LVL_GET_TILE(lvl, p1->x + 1, y) == TILE_NONE)
				LVL_SET_TILE(lvl, p1->x + 1, y, TILE_WALL);
		}
	}
	//else if the hallway is horizontal
	else {
		for(u32 x = p1->x; x < p2->x; x++) {

			//place floor tiles
			//overwrite any tile except room floors
			if(LVL_GET_TILE(lvl, x, p1->y) != TILE_FLOOR_ROOM)
				LVL_SET_TILE(lvl, x, p1->y, TILE_FLOOR_HALL);

			//place the left wall of the hallway
			if(LVL_GET_TILE(lvl, x, p1->y - 1) == TILE_NONE)
				LVL_SET_TILE(lvl, x, p1->y - 1, TILE_WALL);
			//place the right wall of the hallway
			if(LVL_GET_TILE(lvl, x, p1->y + 1) == TILE_NONE)
				LVL_SET_TILE(lvl, x, p1->y + 1, TILE_WALL);
		}
	}
}

// TODO: each room except entrance and exits have a chance to have enemies in them. when
//generating a room, spawn a random number of enemies in there. dynamically allocate the
//enemies and add them to the enemy array. the mob cap is 32

// TODO: add support for maps that load on the fly so they can be bigger than 32x32 meta tiles

// TODO: place entries/exits
// TODO: room types
// TODO: decor tiles (grass, water)
// TODO: place n chests in every map with at least m empty spaces in every direction
// TODO: implement astar that uses the collision array
// TODO: implement level themes
// TODO: consider scrapping chasm levels. i really like the tightly clustered result of 3 recursions on a 32x32 map

// TODO: in most cases, water and foliage will cost more than regular terrain
// TODO: all terrain except walls and none will cost the same for flying enemies
// TODO: lightning will do double damage if the target is standing on water
// TODO: flying enemies will take double damage from lightning
// TODO: slimes will prefer water tiles
// TODO: all enemies will prefer water tiles when on fire
// TODO: foliage tiles will be flammable and spread
// TODO: some enemies can't go through doors because they are "too big"
// TODO: place a door to a shop somewhere on the outer edge of a level. leads to a shop level that you come ang go from

// TODO: flooded will use cellular automata to place water tiles throughout the level
// TODO: overgrown will randomly place foliage everywhere
// TODO: ruined will knock out huge (non-essential) chunks of floor
// TODO: chasm will make the hallways catwalks and the rooms hollow pillars

// TODO: entry will contain the stairs to the floor above
// TODO: exit will contain the stairs to the floor below

// TODO: the level details like cracks and moldy walls will be randomly generated but every time the player leaves a floor, it's state will be saved to sram