//Nick Sells, 2021
//anim.c

#include "anim.h"

//=============================================================================
//FUNCTIONS
//=============================================================================

//advances the animation logic by one frame
void anim_update(Anim* anim) {
	//loop mode
	if(anim->data->looped) {
		//decrement the timer to zero
		if(anim->timer > 0)
			anim->timer--;
		//when it hits zero, jump one frame
		else {
			anim->frame++;
			//if the anim is over, start back at zero
			if(anim->frame >= anim->data->length)
				anim->frame = 0;
			//and set the next timer
    		anim->timer = anim->data->repeats[anim->frame];
		}
	}
	//non-looping mode
	else {
		//decrement the timer to zero
		if (anim->timer > 0)
			anim->timer--;
		//when it hits zero and the anim is not finished,
		else if(anim->frame < anim->data->length - 1) {
			//jump one frame and set the next timer
			anim->frame++;
			anim->timer = anim->data->repeats[anim->frame];
		}
	}
}


//returns the tid of the current animation frame
u32 anim_get_tid(const Anim* anim) {
	return anim->data->frames[anim->frame];
}


//whether or not the animation needs to update on-screen
BOOL anim_is_dirty(const Anim* anim) {
	return anim->timer == anim->data->repeats[anim->frame];
}
