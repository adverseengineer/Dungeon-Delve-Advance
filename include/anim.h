//Nick Sells, 2021
//anim.h

//provides structs and functions for handling animations

#ifndef ANIM_H
#define ANIM_H

#include <tonc.h>

//=============================================================================
//STRUCTS
//=============================================================================

typedef struct AnimData {
	u16 frames[16];     //obj tid of each frame
	u8 repeats[16];     //length of each frame
	u8 length;          //number of frames
} ALIGN4 AnimData;

typedef struct Anim {
    const AnimData* data;   //pointer to relevant AnimData
    BOOL loop;              //whether or not the animation should replay automatically
	u8 frame;               //current frame index within AnimData
    u8 timer;               //remaining ticks until advancing to the next frame
} ALIGN4 Anim;

//=============================================================================
//FORWARD-DECLARATIONS
//=============================================================================

static inline void anim_restart(Anim* self);
static inline void anim_setData(Anim* self, const AnimData* data);
static inline void anim_setLoop(Anim* self, BOOL loop);

//=============================================================================
//FUNCTIONS
//=============================================================================

//sets an animation's data
static inline void anim_init(Anim* self, const AnimData* data, BOOL loop) {
	anim_setData(self, data);
	anim_setLoop(self, loop);
	anim_restart(self);
}

// TODO: if i ever go the route of copying in anim tiles on the fly, consider reimplementing dirty()

//advances the animation logic by one frame
static inline void anim_update(Anim* self) {
	if(self->data != NULL) {
		//loop mode
		if(self->loop) {
			if(self->timer > 0)
				self->timer--;
			else { //when it hits zero, jump one frame
				self->frame++;
				//if the anim is over, start over and set the next timer
				if(self->frame >= self->data->length)
					self->frame = 0;
	    		self->timer = self->data->repeats[self->frame];
			}
		}
		//non-looping mode
		else {
			//decrement the timer to zero
			if (self->timer > 0)
				self->timer--;
			//when it hits zero and the anim is not finished,
			else if(self->frame < self->data->length - 1) {
			//jump one frame and set the next timer
				self->frame++;
				self->timer = self->data->repeats[self->frame];
			}
		}

	}
}


//restarts an animation. mostly used for replaying a non-looping animation
static inline void anim_restart(Anim* self) {
	self->frame = 0;
	self->timer = 0;
}


//changes the animation's data pointer
static inline void anim_setData(Anim* self, const AnimData* data) {
	self->data = data;
}


static inline void anim_setLoop(Anim* self, BOOL loop) {
	self->loop = loop;
}


static inline void anim_toggleLoop(Anim* self) {
	self->loop = !self->loop;
}


//returns the current tid of the animation
static inline u32 anim_getTid(const Anim* self) {
	return self->data->frames[self->frame];
}

#endif //ANIM_H
