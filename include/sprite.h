//Nick Sells, 2021
//sprite.h

//provides a layer of abstraction away from the hardware for dealing with sprites

#ifndef SPRITE_H
#define SPRITE_H

#include <tonc.h>

#include "config.h"
#include "common.h"

//=============================================================================
// DEFINES
//=============================================================================

#define MAX_SPRITES 128

//=============================================================================
// STRUCTS AND TYPEDEFS
//=============================================================================

typedef struct Sprite Sprite;
struct Sprite {
	OBJ_ATTR obj; //a group of attributes that make up a sprite in hardware
	POINT16 pos; //the "true" position. necessary to avoid coordinate wrapping
	BOOL isAvailable; //if the sprite can be safely overwritten
	BOOL needsRedraw; //if we need to rewrite this sprite's obj to oam
	BOOL wasOnScreen; //if the sprite was on screen after it was last updated
} ALIGN4;

//=============================================================================
// EXTERNS
//=============================================================================

extern void spr_init(void);
extern void spr_render(void);

extern Sprite* spr_create(s32 x, s32 y, ObjTile tid, ObjPrio prio);
extern void spr_destroy(Sprite* this);

//=============================================================================
// INLINES
//=============================================================================

static void spr_setX(Sprite* this, s32 x);
static void spr_setY(Sprite* this, s32 x);
static void spr_setPos(Sprite* this, s32 x, s32 y);
static void spr_move(Sprite* this, s32 dx, s32 dy);

static void spr_flipHorz(Sprite* this);
static void spr_flipVert(Sprite* this);

static void spr_hide(Sprite* this);
static void spr_unhide(Sprite* this);
static void spr_autoHide(Sprite* this);
static BOOL spr_isOnScreen(const Sprite* this);

//sets a sprite's x coord
static inline void spr_setX(Sprite* this, s32 x) {
	BFN_SET(this->obj.attr1, x, ATTR1_X);
	this->pos.x = x;
	spr_autoHide(this);
	this->needsRedraw = TRUE;
}

//sets a sprite's y coord
static inline void spr_setY(Sprite* this, s32 y){
	BFN_SET(this->obj.attr0, y, ATTR0_Y);
	this->pos.y = y;
	spr_autoHide(this);
	this->needsRedraw = TRUE;
}

//moves the given sprite to the given coords
static inline void spr_setPos(Sprite* this, s32 x, s32 y) {
	spr_setX(this, x);
	spr_setY(this, y);
}

//moves the given sprite relative to it's current position
static inline void spr_move(Sprite* this, s32 dx, s32 dy) {
	spr_setX(this, this->pos.x + dx);
	spr_setY(this, this->pos.y + dy);
}

//flips a sprite horizontally
static inline void spr_flipHorz(Sprite* this) {
	this->obj.attr1 ^= ATTR1_HFLIP;
	this->needsRedraw = TRUE;
}

//flips a sprite vertically
static inline void spr_flipVert(Sprite* this) {
	this->obj.attr1 ^= ATTR1_VFLIP;
	this->needsRedraw = TRUE;
}

//hides a sprite's object
static inline void spr_hide(Sprite* this) {
	obj_hide(&this->obj);
	this->needsRedraw = TRUE;
}

//unhides a sprite's object
static inline void spr_unhide(Sprite* this) {
	obj_unhide(&this->obj, 0); //TODO: what is the second param supposed to mean???
	this->needsRedraw = TRUE;
}

//handles the logic for when a sprite should be hidden to avoid coord wrapping
//TODO: find a better name for this function
static inline void spr_autoHide(Sprite* this) {
	//if the sprite just moved onto the screen
	if(!this->wasOnScreen && spr_isOnScreen(this))
		spr_unhide(this);
	//if the sprite just moved off the screen
	else if(this->wasOnScreen && !spr_isOnScreen(this))
		spr_hide(this);

	this->wasOnScreen = spr_isOnScreen(this);
}

//TODO: replace 16 with the width or height of the sprite
//returns true if any part of the sprite is within the screen rect
static inline BOOL spr_isOnScreen(const Sprite* this) {
	return in_range(this->pos.x, -16, SCREEN_WIDTH)
		&& in_range(this->pos.y, -16, SCREEN_HEIGHT);
}

#endif //SPRITE_H
