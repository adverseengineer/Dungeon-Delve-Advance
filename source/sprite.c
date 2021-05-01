//Nick Sells, 2021
//sprite.c

#include <stdlib.h>
#include "sprite.h"

//=============================================================================
//GLOBALS
//=============================================================================

static Sprite* head;
static u8 sprite_counter;

//=============================================================================
//FUNCTIONS
//=============================================================================

//allocates and initializes a sprite and links it at the start of the list
//returns a pointer to the sprite if there was room, returns NULL otherwise
//NOTE: for now, assumes a square 16x16 sprite, because that's all i'm using
Sprite* spr_create(s32 x, s32 y, u32 tid, const AnimData* data) {
	//only create if there is room
	if(sprite_counter < MAX_SPRITES) {
		sprite_counter++;
		//allocate a sprite
		Sprite* spr = (Sprite*) malloc(sizeof(Sprite));
		//set up the sprite's obj attributes and animation
		obj_set_attr(&spr->obj, ATTR0_SQUARE, ATTR1_SIZE_16, tid);
		obj_set_pos(&spr->obj, x, y);
		anim_init(&spr->anim, data);
		spr->anim.looped = TRUE;
		//insert the sprite at the beginning of the linked list
		spr->next = head;
		head = spr;
		return spr;
	}
	else
		return NULL;
}


//TODO: test
//deallocates a sprite, hides it, and unlinks it from the list
void spr_destroy(Sprite* spr) {
	//only destroy if there is at least one sprite
	if(sprite_counter > 0) {
		sprite_counter--;
		Sprite* temp = spr->next;
		*spr = *temp;
		spr->next = temp->next;
		free(temp);
	}
}

//TODO: test
//renders all sprites in the list
void spr_render(void) {
	Sprite* spr = head;
	for(int i = 0; spr != NULL; i++) {
   		oam_copy(&oam_mem[i], &spr->obj, 1);
		spr = spr->next;
	}
}
