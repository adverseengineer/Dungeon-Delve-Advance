//Nick Sells, 2021
//sprite.h

//provides a layer of abstraction away from the hardware for dealing with sprites

#ifndef SPRITE_H
#define SPRITE_H

#include <tonc.h>

#include "config.h"

//=============================================================================
//DEFINES
//=============================================================================

#define MAX_SPRITES 128

//=============================================================================
//STRUCTS AND TYPEDEFS
//=============================================================================

typedef struct Sprite Sprite;
struct Sprite {
	OBJ_ATTR obj; //a group of attributes that make up a sprite in hardware
	POINT16 pos; //the "true" position. necessary to avoid coordinate wrapping
	BOOL isAvailable; //whether or not the sprite can be safely overwritten
	BOOL needsRedraw; //whether or not to rewrite this sprite's obj to oam
} ALIGN4;

//=============================================================================
//FUNCTION DECLARATIONS
//=============================================================================

void spr_init(void);

void spr_render(void);

Sprite* spr_create(s32 x, s32 y, ObjTile tid, ObjPrio prio);
void spr_destroy(Sprite* self);

s32 spr_getX(const Sprite* self);
void spr_setX(Sprite* self, s32 x);
s32 spr_getY(const Sprite* self);
void spr_setY(Sprite* self, s32 x);

void spr_setPos(Sprite* self, s32 x, s32 y);
void spr_move(Sprite* self, s32 dx, s32 dy);

void spr_flipHorz(Sprite* self);
void spr_flipVert(Sprite* self);

BOOL spr_isOnScreen(const Sprite* self);

#endif //SPRITE_H
