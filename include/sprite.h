//Nick Sells, 2021
//sprite.h

#ifndef SPRITE
#define SPRITE

// #include <tonc_math.h>
#include <tonc_oam.h>
#include "anim.h"

//=============================================================================
//DEFINES
//=============================================================================

#define MAX_SPRITES 128

//=============================================================================
//STRUCTS AND TYPEDEFS
//=============================================================================

typedef struct Sprite Sprite;
struct Sprite {
	OBJ_ATTR obj;
	Anim anim;
	Sprite* next;
} ALIGN4;


//=============================================================================
//FORWARD-DECLARATIONS
//=============================================================================

extern Sprite* spr_create(s32 x, s32 y, u32 tid, const AnimData* data);
extern void spr_destroy(Sprite* spr);
extern Sprite* spr_pop(void);
extern void spr_render(void);

//=============================================================================
//INLINES
//=============================================================================

//gets a sprite's x coord
inline s32 spr_getX(const Sprite* spr) {
	return BFN_GET(spr->obj.attr1, ATTR1_X);
}

//gets a sprite's y coord
inline s32 spr_getY(const Sprite* spr) {
	return BFN_GET(spr->obj.attr0, ATTR0_Y);
}

//gets a sprite's tid
inline s32 spr_getTid(const Sprite* spr) {
	return BFN_GET(spr->obj.attr2, ATTR2_ID);
}

//sets a sprite's x coord
inline void spr_setX(Sprite* spr, s32 x) {
	BFN_SET(spr->obj.attr1, x, ATTR1_X);
}

//sets a sprite's y coord
inline void spr_setY(Sprite* spr, s32 x){
	BFN_SET(spr->obj.attr0, x, ATTR0_Y);
}

//sets a sprite's tid
inline void spr_setTid(Sprite* spr, s32 x) {
	BFN_SET(spr->obj.attr2, x, ATTR2_ID);
}

//flips a sprite horizontally
inline void spr_flipHorz(Sprite* spr) {
	spr->obj.attr1 ^= ATTR1_HFLIP;
}

//flips a sprite vertically
inline void spr_flipVert(Sprite* spr) {
	spr->obj.attr1 ^= ATTR1_VFLIP;
}

//moves the given sprite to the given coords
static inline void spr_setPos(Sprite* spr, s32 x, s32 y) {
	obj_set_pos(&spr->obj, x, y);
}

//moves the given sprite relative to it's current position
static inline void spr_move(Sprite* spr, s32 dx, s32 dy) {
	spr_setX(spr, dx + spr_getX(spr));
	spr_setY(spr, dy + spr_getY(spr));
}

//advances a sprite to it's next frame of animation
static inline void spr_animStep(Sprite* spr) {
	anim_update(&spr->anim);
	spr_setTid(spr, anim_getTid(&spr->anim));
}

#endif //SPRITE
