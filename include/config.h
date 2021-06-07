//Nick Sells, 2021
//config.h

//this file is meant to group game parameters all in one place for easy tweaking

#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG

//BG layers
#define BG_TXT              0
#define BG_UI               1
#define BG_LVL              2
#define BG_XXX              3 //TODO: use this bg layer for shadows OR overlapping pieces of level that need to overlap the player

//BG charblocks
#define CBB_LVL             0
#define CBB_UI              1
#define CBB_TXT             2
//NOTE: CBB 3 is reserved for use as SBB 24-31

//BG screenblocks
#define SBB_LVL             26 //levels use 4 screenblocks
#define SBB_UI              30
#define SBB_TXT             31

//palette banks
#define PB_LVL             0
#define PB_UI              1

//screen entries
#define SE_NONE             0
//lvl
#define SE_WALL             4  | SE_PALBANK(PB_LVL)
#define SE_WALL_ALT         8  | SE_PALBANK(PB_LVL)
#define SE_DOOR_CLOSED      12 | SE_PALBANK(PB_LVL)
#define SE_DOOR_OPEN        16 | SE_PALBANK(PB_LVL)
#define SE_LADDER_UP        20 | SE_PALBANK(PB_LVL)
#define SE_LADDER_DOWN      24 | SE_PALBANK(PB_LVL)
#define SE_FLOOR            28 | SE_PALBANK(PB_LVL)
#define SE_FOLIAGE          32 | SE_PALBANK(PB_LVL)
#define SE_WATER            36 | SE_PALBANK(PB_LVL)
#define SE_CHASM            40 | SE_PALBANK(PB_LVL)
//ui
#define SE_ITEM_PANE        4  | SE_PALBANK(PB_UI)
#define SE_HP_ICON			8  | SE_PALBANK(PB_UI)
#define SE_MP_ICON          12 | SE_PALBANK(PB_UI)
#define SE_METER_0          16 | SE_PALBANK(PB_UI)
#define SE_METER_1          20 | SE_PALBANK(PB_UI)

//object tid's
//TODO: after copying all obj tiles to vram, assign these
#define TID_ERR             0 //placeholder sprite
#define TID_PLAYER_WARRIOR  0
#define TID_PLAYER_MAGE     0
#define TID_SKELETON        0
#define TID_MIMIC           0
#define TID_CACO            0
#define TID_BAT             0
#define TID_SLIME           0
#define TID_RAT             0
#define TID_GOLD            0
#define TID_SIGN            0

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