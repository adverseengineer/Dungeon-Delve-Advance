//Nick Sells, 2021
//common.h

//this header is mostly just meant for named constants like cardinal directions and such

#ifndef COMMON_H
#define COMMON_H

#include <tonc.h>

#define ERR			            0xB00B

#define DIR_NONE                0
#define DIR_UP                  1
#define DIR_DOWN                2
#define DIR_LEFT                3
#define DIR_RIGHT               4

#define MTILE_SIZE              16

//screen size in meta-tiles
#define SCREEN_WIDTH_M          (SCREEN_WIDTH / MTILE_SIZE)
#define SCREEN_HEIGHT_M         (SCREEN_HEIGHT / MTILE_SIZE)

//TODO: do i still need this?
//maps from level space to screen space. coord is the level x or y and offset is the level's scroll amount
// #define lvl2scr(coord, offset)  ((coord) * (MTILE_SIZE) + (offset))

#endif //COMMON_H
