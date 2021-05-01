//Nick Sells, 2021
//anim_table.c

#include "data/anim_table.h"

const AnimData anim_table[32] = {
	//ANIM_PLAYER_IDLE
	//the player looks left and right
	(AnimData) {
		frames: {0},
		repeats: {0},
		length: 2,
	},
	//ANIM_PLAYER_WALK
	//the player's walk cycle. middle, left, middle, right
	(AnimData) {
		frames: {0},
		repeats: {0},
		length: 4,
	},
	//ANIM_PLAYER_ACTION
	//the player fiddles with something in their hands
	(AnimData) {
		frames: {0},
		repeats: {0},
		length: 2,
	},
	//ANIM_CACO_IDLE
	//the beholder levitates and waves its claws
	(AnimData) {
		frames: {8, 12},
		repeats: {10, 10},
		length: 2,
	},
	//ANIM_RAT_IDLE
	//the rat bobs up and down
	(AnimData) {
		frames: {0},
		repeats: {0},
		length: 2,	
	},
	//ANIM_SKEL_WALK
	//the skeleton's walk cycle
	(AnimData) {
		frames: {0},
		repeats: {0},
		length: 4,
	},
	////ANIM_GOO_IDLE
	//the goo opens and closes it's mouth
	(AnimData) {
		frames: {0},
		repeats: {0},
		length: 2,
	},
	//ANIM_MIMIC_IDLE
	//the mimic poses as a chest and licks its lips
	(AnimData) {
		frames: {0},
		repeats: {0},
		length: 2,
	},
	//ANIM_MIMIC_CHASE
	//the mimic chomps its mouth
	(AnimData) {
		frames: {0},
		repeats: {0},
		length: 5,
	},
	//ANIM_BAT_IDLE
	//the bat flaps it's wings
	(AnimData) {
		frames: {0, 4},
		repeats: {3, 3},
		length: 2,
	},
	//ANIM_GOLEM_WALK
	//the golem's walk cycle
	(AnimData) {
		frames: {0},
		repeats: {0},
		length: 4,
	}
};
