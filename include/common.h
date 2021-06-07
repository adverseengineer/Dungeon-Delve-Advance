//Nick Sells, 2021
//common.h

//this header is mostly just meant for named constants like cardinal directions and such

#ifndef COMMON_H
#define COMMON_H

#include <tonc.h>

#define ERR1			        0xB00B1E5
#define ERR2                    0xBAD1DEA

typedef enum Direction {
	DIR_NONE,
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	NUM_DIRS = 5 //used for the upper bound when randomly selecting a direction
} ALIGN4 Direction;

//screen size in meta-tiles
#define SCREEN_WIDTH_M          (SCREEN_WIDTH / 16)
#define SCREEN_HEIGHT_M         (SCREEN_HEIGHT / 16)

//TODO: do i still need this?
//maps from level space to screen space. coord is the level x or y and offset is the level's scroll amount
// #define lvl2scr(coord, offset)  ((coord) * (MTILE_SIZE) + (offset))

#endif //COMMON_H
