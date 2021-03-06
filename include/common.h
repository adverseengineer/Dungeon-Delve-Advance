//Nick Sells, 2021
//common.h

//this header is mostly just meant for named constants like cardinal directions and such

#ifndef COMMON_H
#define COMMON_H

#include <tonc.h>
#include "config.h"

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

#ifdef DEBUG
#define DEBUG_BLOCK(...) __VA_ARGS__
#else
#define DEBUG_BLOCK(...)
#endif

#endif //COMMON_H
