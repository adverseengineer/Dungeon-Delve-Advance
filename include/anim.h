//Nick Sells, 2021
//anim.h

//anim_update provided by exelotl

#ifndef ANIM_H
#define ANIM_H

#include <tonc_types.h>

//=============================================================================
//STRUCTS
//=============================================================================

//the "type" of an animation
typedef struct {
	u16* frames;        //obj tid of each frame
	u8* repeats;        //length of each frame
	u8* flips;          //which ways to flip each frame
	u8 length;          //number of frames
	BOOL looped;        //should it play once or loop
} ALIGN4 AnimData;

//an "instance" of an AnimData
typedef struct {
    const AnimData* data;   //pointer to relevant AnimData
	u8 frame;               //current frame index within AnimData
    u8 timer;               //remaining ticks until advancing to the next frame
} ALIGN4 Anim;

//=============================================================================
//FUNCTIONS
//=============================================================================

extern void anim_update(Anim* anim);
extern u32 anim_getTID(const Anim* anim);
extern BOOL anim_isDirty(const Anim* anim);

#endif //ANIM
