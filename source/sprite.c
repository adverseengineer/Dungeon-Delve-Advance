//Nick Sells, 2021
//sprite.c

#include "sprite.h"

//=============================================================================
//GLOBALS
//=============================================================================

//NOTE: sprites are stored contiguously, by value, not by pointer
EWRAM_DATA static Sprite sprites[MAX_SPRITES];

//=============================================================================
//FUNCTIONS
//=============================================================================

//clears the sprite list AND the real oam to safe values
void spr_init(void) {

	for(size_t i = 0; i < MAX_SPRITES; i++) {
		obj_hide(&sprites[i].obj);
		sprites[i].isAvailable = TRUE;
		sprites[i].wasOnScreen = spr_isOnScreen(&sprites[i]);
	}

	oam_init(oam_mem, MAX_SPRITES);
	
	DEBUG_BLOCK(
		mgba_printf(LOG_INFO, "sprite list was successfully initialized");
	);
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

//claims the next available sprite slot, sets it up, and returns a pointer to it
Sprite* spr_create(s32 x, s32 y, ObjTile tid, ObjPrio prio) {
	
	for(size_t i = 0; i < MAX_SPRITES; i++) {
		if(sprites[i].isAvailable) {
			
			//claim the slot
			Sprite* this = &sprites[i];
			this->isAvailable = FALSE;
			this->needsRedraw = TRUE;
			
			//set up the sprite
			obj_set_attr(&this->obj, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_BUILD(tid, 0, prio));
			spr_setPos(this, x, y);
			
			return this;
		}
	}
	//if no slot was available
	DEBUG_BLOCK(
		mgba_printf(LOG_ERR, "failed to create sprite: no sprite slots available");
	);
	return NULL;
}

//relinquishes a sprite slot
void spr_destroy(Sprite* this) {
	this->isAvailable = TRUE;
	obj_hide(&this->obj);
}
