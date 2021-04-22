//Nick Sells, 2021
//sprite.h

//sprite linked list z-depth scheme from https://www.gamasutra.com/view/feature/131491/gameboy_advance_resource_management.php

#ifndef SPRITE
#define SPRITE

//=============================================================================
//STRUCTS
//=============================================================================

//ties together an OAM entry and a pointer to the one with the next highest priority
typedef struct Sprite {
	OBJ_ATTR obj;
	OBJ_ATTR* next;
} ALIGN4 Sprite;

//=============================================================================
//GLOBALS
//=============================================================================

//used internally only by this module
static Sprite sprite_buffer[128]; 

//=============================================================================
//FUNCTIONS
//=============================================================================

static inline spr_swap(Sprite* spr_a, Sprite* spr_b) {
	OBJ_ATTR* temp = spr_a->next;
	spr_a->next = spr_b->next;
	spr_b->next = temp;
}

static inline spr_update(void) {
	for(u32 i = 0; i < 128; i++) {

		//TODO: finish this
	}
}

#endif //SPRITE
