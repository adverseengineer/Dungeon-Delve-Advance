//Nick Sells, 2021
//config.h

//this file is meant to group game parameters all in one place for easy tweaking

#ifndef CONFIG_H
#define CONFIG_H

//BG layers
#define BG_UI_TXT           0
#define BG_UI_GFX           1
#define BG_LVL              2
#define BG_XXX              3 //TODO: use this bg layer for shadows OR overlapping pieces of level that need to overlap the player

//BG charblocks
#define CBB_UI_TXT          0
#define CBB_UI_GFX          1
#define CBB_LVL             2
#define CBB_XXX             3 //TODO: unused so far

//OBJ charblocks
#define CBB_SPR             0
#define CBB_YYY             1 //TODO: unused so far

//BG screenblocks
#define SBB_LVL             8 //levels use 4 screenblocks
#define SBB_UI_TXT          12
#define SBB_UI_GFX          13

//screen entries
#define SE_NONE             64
#define SE_WALL             0
#define SE_WALL_ALT         4
#define SE_DOOR_CLOSED      8
#define SE_DOOR_OPEN        12
#define SE_LADDER_UP        16
#define SE_LADDER_DOWN      20
#define SE_FLOOR            24
#define SE_FLOOR_WATER      28

//object tid's
#define TID_ERR             0 //placeholder sprite
#define TID_PLAYER_WARRIOR  1
#define TID_PLAYER_MAGE     2
#define TID_MIMIC           3
#define TID_CACO            4
#define TID_BAT             5
#define TID_SLIME           6
#define TID_RAT             7
#define TID_GOLD            8
#define TID_SIGN            9

//level generator parameters
#define LVL_WIDTH           32
#define LVL_HEIGHT          32
#define NUM_RECUR           3
#define SPLIT_ATTEMPTS      -1
#define DISCARD_RATIO       (float2fx(0.4))
#define PADDING_FACTOR      4
#define MIN_ROOM_SIZE       5
#define MAX_ACTORS          32

#endif //CONFIG_H