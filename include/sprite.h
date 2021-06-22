//Nick Sells, 2021
//sprite.h

//provides a layer of abstraction away from the hardware for dealing with sprites

#ifndef SPRITE_H
#define SPRITE_H

#include <tonc.h>

#include "config.h"

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
extern void spr_destroy(Sprite* self);

//=============================================================================
// INLINES
//=============================================================================

static void spr_setX(Sprite* self, s32 x);
static void spr_setY(Sprite* self, s32 x);
static void spr_setPos(Sprite* self, s32 x, s32 y);
static void spr_move(Sprite* self, s32 dx, s32 dy);

static void spr_flipHorz(Sprite* self);
static void spr_flipVert(Sprite* self);

static void spr_hide(Sprite* self);
static void spr_unhide(Sprite* self);
static void spr_autoHide(Sprite* self);
static BOOL spr_isOnScreen(const Sprite* self);

//sets a sprite's x coord
static inline void spr_setX(Sprite* self, s32 x) {
	BFN_SET(self->obj.attr1, x, ATTR1_X);
	self->pos.x = x;
	spr_autoHide(self);
	self->needsRedraw = TRUE;
}

//sets a sprite's y coord
static inline void spr_setY(Sprite* self, s32 y){
	BFN_SET(self->obj.attr0, y, ATTR0_Y);
	self->pos.y = y;
	spr_autoHide(self);
	self->needsRedraw = TRUE;
}

//moves the given sprite to the given coords
static inline void spr_setPos(Sprite* self, s32 x, s32 y) {
	spr_setX(self, x);
	spr_setY(self, y);
}

//moves the given sprite relative to it's current position
static inline void spr_move(Sprite* self, s32 dx, s32 dy) {
	spr_setX(self, self->pos.x + dx);
	spr_setY(self, self->pos.y + dy);
}

//flips a sprite horizontally
static inline void spr_flipHorz(Sprite* self) {
	self->obj.attr1 ^= ATTR1_HFLIP;
	self->needsRedraw = TRUE;
}

//flips a sprite vertically
static inline void spr_flipVert(Sprite* self) {
	self->obj.attr1 ^= ATTR1_VFLIP;
	self->needsRedraw = TRUE;
}

//hides a sprite's object
static inline void spr_hide(Sprite* self) {
	obj_hide(&self->obj);
	self->needsRedraw = TRUE;
}

//unhides a sprite's object
static inline void spr_unhide(Sprite* self) {
	obj_unhide(&self->obj, 0); //TODO: what is the second param supposed to mean???
	self->needsRedraw = TRUE;
}

//handles the logic for when a sprite should be hidden to avoid coord wrapping
//TODO: find a better name for this function
static inline void spr_autoHide(Sprite* self) {
	//if the sprite just moved onto the screen
	if(!self->wasOnScreen && spr_isOnScreen(self))
		spr_unhide(self);
	//if the sprite just moved off the screen
	else if(self->wasOnScreen && !spr_isOnScreen(self))
		spr_hide(self);

	self->wasOnScreen = spr_isOnScreen(self);
}

//TODO: replace 16 with the width or height of the sprite
//returns true if any part of the sprite is within the screen rect
static inline BOOL spr_isOnScreen(const Sprite* self) {
	return in_range(self->pos.x, -16, SCREEN_WIDTH)
		&& in_range(self->pos.y, -16, SCREEN_HEIGHT);
}

#endif //SPRITE_H
