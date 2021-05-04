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


void spr_print(void) {
	mgba_log(LOG_INFO, " ");
	Sprite* current = head;
	while(current != NULL) {
		mgba_printf(LOG_INFO, "current: %d, next: %d", current, current->next);
		current = current->next;
	}
}

#include <tonc_mgba.h>

//TODO: test
//clears the sprite list to safe values
void spr_init(void) {
	Sprite* spr = head;
	while(spr != NULL) {
   		obj_hide(&spr->obj);
		spr = spr->next;
	}
}

//allocates and initializes a sprite and links it at the start of the list
//returns a pointer to the sprite if there was room, returns NULL otherwise
//NOTE: for now, assumes a square 16x16 sprite, because that's all i'm using
Sprite* spr_create(s32 x, s32 y, u32 tid, const AnimData* data, BOOL loop) {
	//only create if there is room
	if(sprite_counter < MAX_SPRITES) {
		sprite_counter++;
		//allocate a sprite
		Sprite* spr = (Sprite*) malloc(sizeof(Sprite));
		//set up the sprite's obj attributes and animation
		obj_set_attr(&spr->obj, ATTR0_SQUARE, ATTR1_SIZE_16, tid);
		obj_set_pos(&spr->obj, x, y);
		anim_init(&spr->anim, data, loop);
		//insert the sprite at the beginning of the list
		spr->next = head;
		head = spr;
		return spr;
	}
	else {
		mgba_log(LOG_ERR, "failed to allocate new sprite: list is full");
		return NULL;
	}
}


// //TODO: test
//delete a link with given key
void spr_destroy(Sprite* spr) {

	//only destroy if there is at least one sprite
	if((sprite_counter > 0) & (head != NULL)) {
		sprite_counter--;
		Sprite* current = head;
		Sprite* previous = NULL;
		//until we find the target sprite
		while(current != spr) {
			//if we hit the end searching for it, abort
			if(current->next == NULL)
				return;
			
			//otherwise, update the trackers for the next loop iteration
			previous = current;
			current = current->next;
		}
		//once we've found it, current is essentially an alias for spr
		
		//if the target is the head of the list, the head is replaced by it's next.
		if(current == head)
			head = head->next;
		//otherwise, stitch spr's neighbors together, excluding spr
		else {
			previous->next = current->next;
		}
		
		obj_hide(&current->obj);
		free(current);	
	}
	else
		mgba_log(LOG_ERR, "failed to deallocate sprite: list is empty");
	// //found a match, update the link
	// if(current == head)
	// 	//change first to point to next link
	// 	head = head->next;
	// else
	// 	//bypass the current link
	// 	previous->next = current->next; 
}

//TODO: test
//renders all sprites in the list
void spr_render(void) {
	Sprite* spr = head;
	for(u32 i = 0; i < MAX_SPRITES; i++) {
		if(spr != NULL) {
			oam_copy(&oam_mem[i], &spr->obj, 1);
			spr = spr->next;
		}
		else
			oam_init(&oam_mem[i], 1);
	}
	// oam_init(&oam_mem[i], MAX_SPRITES - i);
}

