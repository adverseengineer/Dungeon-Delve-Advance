//Nick Sells, 2021
//sprite.c

#include "sprite.h"

//=============================================================================
//GLOBALS
//=============================================================================

BOOL spr_needRedraw;

static Sprite* head;
static u8 sprite_counter;

//=============================================================================
//FUNCTIONS
//=============================================================================

//set the proper video flags and clears the sprite list AND the real oam to safe values
void spr_init(void) {
	Sprite* current = head;
	while(current != NULL) {
   		obj_hide(&current->obj);
		current = current->next;
	}
	oam_init(oam_mem, MAX_SPRITES);
}

//links a sprite into the shadow oam list
void spr_link(Sprite* spr) {
	//only create if there is room
	if(sprite_counter < MAX_SPRITES) {
		sprite_counter++;
		//link spr into the beginning of the list
		spr->next = head;
		head = spr;

		spr_needRedraw = TRUE;
	}
}


//delete a link with given key
void spr_unlink(Sprite* spr) {
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
		//if the target is the head of the list, the head is replaced by it's next.
		if(current == head)
			head = head->next;
		//otherwise, stitch spr's neighbors together, excluding spr
		else
			previous->next = current->next;

		spr_needRedraw = TRUE;
	}
}


//renders all sprites in the list
void spr_render(void) {
	if(spr_needRedraw) {
		Sprite* current = head;
		u32 i;
		for(i = 0; current != NULL; i++) {
			oam_copy(&oam_mem[i], &current->obj, 1);
			current = current->next;
		}
		
		obj_hide_multi(&oam_mem[i], MAX_SPRITES - i);

		spr_needRedraw = FALSE;
	}
}

