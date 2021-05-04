//Nick Sells, 2021
//anim.h

#ifndef ANIM_H
#define ANIM_H

#include <tonc_types.h>

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

static inline void anim_init(Anim* anim, const AnimData* data, BOOL loop);
static inline void anim_update(Anim* anim);
static inline void anim_restart(Anim* anim);
static inline void anim_setData(Anim* anim, const AnimData* data);
static inline void anim_setLoop(Anim* anim, BOOL loop);
static inline void anim_toggleLoop(Anim* anim);
static inline u32 anim_getTid(const Anim* anim);

//=============================================================================
//FUNCTIONS
//=============================================================================

//sets an animation's data
static inline void anim_init(Anim* anim, const AnimData* data, BOOL loop) {
	anim_setData(anim, data);
	anim_setLoop(anim, loop);
	anim_restart(anim);
}

// TODO: if i ever go the route of copying in anim tiles on the fly, consider reimplementing dirty()

//advances the animation logic by one frame
static inline void anim_update(Anim* anim) {
	if(anim->data != NULL) {
		//loop mode
		if(anim->loop) {
			if(anim->timer > 0)
				anim->timer--;
			else { //when it hits zero, jump one frame
				anim->frame++;
				//if the anim is over, start over and set the next timer
				if(anim->frame >= anim->data->length)
					anim->frame = 0;
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
}


//restarts an animation. mostly used for replaying a non-looping animation
static inline void anim_restart(Anim* anim) {
	anim->frame = 0;
	anim->timer = 0;
}


//changes the animation's data pointer
static inline void anim_setData(Anim* anim, const AnimData* data) {
	anim->data = data;
}


static inline void anim_setLoop(Anim* anim, BOOL loop) {
	anim->loop = loop;
}


static inline void anim_toggleLoop(Anim* anim) {
	anim->loop = !anim->loop;
}


//returns the current tid of the animation
static inline u32 anim_getTid(const Anim* anim) {
	return anim->data->frames[anim->frame];
}


#endif //ANIM
