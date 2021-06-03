//Nick Sells, 2021
//sprite.h

//provides a layer of abstraction away from the hardware for dealing with sprites
//uses the linked-list scheme described at https://www.gamasutra.com/view/feature/131491/gameboy_advance_resource_management.php

#ifndef SPRITE_H
#define SPRITE_H

#include <stdlib.h>
#include <tonc.h>

//=============================================================================
//DEFINES
//=============================================================================

#define MAX_SPRITES 128

//=============================================================================
//STRUCTS AND TYPEDEFS
//=============================================================================

typedef struct Sprite Sprite;
struct Sprite {
	OBJ_ATTR obj;
	Sprite* next;
} ALIGN4;

//=============================================================================
//EXTERNS
//=============================================================================

extern BOOL spr_needRedraw;

extern void spr_init(void);
extern void spr_link(Sprite* self);
extern void spr_unlink(Sprite* self);
extern void spr_render(void);

//=============================================================================
//INLINES
//=============================================================================

//allocates and initializes a sprite
//NOTE: for now, assumes a square 16x16 sprite, because that's all i'm using
static inline Sprite* spr_create(s32 x, s32 y, u32 tid, u32 prio) {
	//allocate a sprite
	Sprite* self = (Sprite*) malloc(sizeof(Sprite));
	obj_set_attr(&self->obj, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_BUILD(tid, 0, prio));
	obj_set_pos(&self->obj, x, y);
	return self;
}

//deallocates a sprite
static inline void spr_destroy(Sprite* self) {
	free(self);
}

//gets a sprite's x coord
static inline s32 spr_getX(const Sprite* self) {
	return BFN_GET(self->obj.attr1, ATTR1_X);
}

//sets a sprite's x coord
inline void spr_setX(Sprite* self, s32 x) {
	BFN_SET(self->obj.attr1, x, ATTR1_X);
	spr_needRedraw = TRUE;
}

//gets a sprite's y coord
inline s32 spr_getY(const Sprite* self) {
	return BFN_GET(self->obj.attr0, ATTR0_Y);
}

//sets a sprite's y coord
inline void spr_setY(Sprite* self, s32 x){
	BFN_SET(self->obj.attr0, x, ATTR0_Y);
	spr_needRedraw = TRUE;
}

//moves the given sprite to the given coords
static inline void spr_setPos(Sprite* self, s32 x, s32 y) {
	obj_set_pos(&self->obj, x, y);
	spr_needRedraw = TRUE;
}

//moves the given sprite relative to it's current position
static inline void spr_move(Sprite* self, s32 dx, s32 dy) {
	spr_setX(self, dx + spr_getX(self));
	spr_setY(self, dy + spr_getY(self));
	spr_needRedraw = TRUE;
}

//flips a sprite horizontally
inline void spr_flipHorz(Sprite* self) {
	self->obj.attr1 ^= ATTR1_HFLIP;
	spr_needRedraw = TRUE;
}

//flips a sprite vertically
inline void spr_flipVert(Sprite* self) {
	self->obj.attr1 ^= ATTR1_VFLIP;
	spr_needRedraw = TRUE;
}

#endif //SPRITE_H
