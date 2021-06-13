//Nick Sells, 2021
//sprite.c

#include "sprite.h"

//=============================================================================
//GLOBALS
//=============================================================================

static Sprite sprites[MAX_SPRITES];

//=============================================================================
//FUNCTIONS
//=============================================================================

//clears the sprite list AND the real oam to safe values
void spr_init(void) {
	for(size_t i = 0; i < MAX_SPRITES; i++) {
		obj_hide(&sprites[i].obj);
		sprites[i].isAvailable = TRUE;
	}

	oam_init(oam_mem, MAX_SPRITES);
}

//claims the next available sprite slot, sets it up, and returns a pointer to it
Sprite* spr_create(s32 x, s32 y, ObjTile tid, ObjPrio prio) {
	
	for(size_t i = 0; i < MAX_SPRITES; i++) {
		if(sprites[i].isAvailable) {
			//claim the slot
			Sprite* self = &sprites[i];
			self->isAvailable = FALSE;
			self->needsRedraw = TRUE;
			//set up the sprite
			obj_set_attr(&self->obj, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_BUILD(tid, 0, prio));
			spr_setPos(self, x, y);
			return self;
		}
	}
	//if no slot was available,
	mgba_printf(LOG_ERR, "failed to create sprite: no sprite slots available");
	return NULL;
}

//relinquishes a sprite slot
void spr_destroy(Sprite* self) {
	self->isAvailable = TRUE;
	obj_hide(&self->obj);
}

//renders all sprites in the list
void spr_render(void) {
	for(size_t i = 0; i < MAX_SPRITES; i++) {
		if(!sprites[i].isAvailable && sprites[i].needsRedraw) {
			oam_copy(&oam_mem[i], &sprites[i].obj, 1);
			sprites[i].needsRedraw = FALSE;
		}
	}
}

//gets a sprite's x coord
s32 spr_getX(const Sprite* self) {
	return BFN_GET(self->obj.attr1, ATTR1_X);
}

//sets a sprite's x coord
void spr_setX(Sprite* self, s32 x) {
	BFN_SET(self->obj.attr1, x, ATTR1_X);
	self->pos.x = x;
	self->needsRedraw = TRUE;
}

//gets a sprite's y coord
s32 spr_getY(const Sprite* self) {
	return BFN_GET(self->obj.attr0, ATTR0_Y);
}

//sets a sprite's y coord
void spr_setY(Sprite* self, s32 y){
	BFN_SET(self->obj.attr0, y, ATTR0_Y);
	self->pos.y = y;
	self->needsRedraw = TRUE;
}

//moves the given sprite to the given coords
void spr_setPos(Sprite* self, s32 x, s32 y) {
	spr_setX(self, x);
	spr_setY(self, y);
}

//moves the given sprite relative to it's current position
void spr_move(Sprite* self, s32 dx, s32 dy) {
	spr_setX(self, dx + spr_getX(self));
	spr_setY(self, dy + spr_getY(self));
}

//flips a sprite horizontally
void spr_flipHorz(Sprite* self) {
	self->obj.attr1 ^= ATTR1_HFLIP;
	self->needsRedraw = TRUE;
}

//flips a sprite vertically
void spr_flipVert(Sprite* self) {
	self->obj.attr1 ^= ATTR1_VFLIP;
	self->needsRedraw = TRUE;
}

void spr_setVisibility(Sprite* self, BOOL visible) {
	if(visible)
		obj_unhide(&self->obj, 0); //TODO: what is the second param supposed to be???
	else
		obj_hide(&self->obj);

	self->needsRedraw = TRUE;
}

//TODO: replace 16 with the width or height of the sprite
//returns true if any part of the sprite is within the screen rect
BOOL spr_isOnScreen(const Sprite* self) {
	return in_range(self->pos.x, -16, SCREEN_WIDTH)
		&& in_range(self->pos.y, -16, SCREEN_HEIGHT);
}

