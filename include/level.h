//Nick Sells, 2021
//level.h

#ifndef LEVEL
#define LEVEL

#include <tonc_math.h>

//=============================================================================
//STRUCTS
//=============================================================================

typedef struct {
	u8 theme;
	u8 width;
	u8 height;
	u8* tiles;
} ALIGN4 Level;

//=============================================================================
//OPTIONS
//=============================================================================

#define NUM_RECUR           3
#define SPLIT_ATTEMPTS      -1
#define DISCARD_RATIO       (float2fx(0.4))
#define PADDING_FACTOR      4
#define MIN_ROOM_SIZE       5

//=============================================================================
//LEVEL TYPES
//=============================================================================

#define THEME_NORMAL        0
#define THEME_FLOODED       1
#define THEME_OVERGROWN     2
#define THEME_CHASM         3
#define THEME_BOSS          4

//=============================================================================
//ROOM TYPES
//=============================================================================

#define ROOM_NORMAL         0
#define ROOM_ENTRY          1
#define ROOM_EXIT           2

//=============================================================================
//TILE TYPES
//=============================================================================

#define TILE_NONE           0
#define TILE_WALL           1
#define TILE_FLOOR_ROOM     2   //two different types of floor tiles are necessary for the door placement logic
#define TILE_FLOOR_HALL     3	//it's sloppy, but necessary. kind of like cave air in minecraft
#define TILE_WATER          4
#define TILE_FOLIAGE        5
#define TILE_CHASM          6
#define TILE_DOOR_CLOSED    7
#define TILE_DOOR_OPEN      8
#define TILE_ENTRY          9
#define TILE_EXIT           10

//=============================================================================
//MACROS
//=============================================================================

#define LVL_GET_TILE(lvl, x, y)         ((lvl)->tiles[(x) + (y) * (lvl)->width])
#define LVL_SET_TILE(lvl, x, y, val)    (LVL_GET_TILE((lvl), (x), (y)) = (val))

#define LVL_IS_WALKABLE(lvl, x, y)      ((LVL_GET_TILE((lvl), (x), (y)) == TILE_FLOOR_HALL) \
                                        | (LVL_GET_TILE((lvl), (x), (y)) == TILE_FLOOR_ROOM))

//TODO: consider either switching to this approach or using a collision array along with the tiles
// switch(GET_TILE(lvl, x, y)) {
// 	case TILE_NONE:
// 	case TILE_WALL:
// 		return false;
// 	default:
// 		return true;
// }

//=============================================================================
//FUNCTIONS
//=============================================================================

extern Level* lvl_create(cu32 width, cu32 height);
extern void lvl_destroy(Level* lvl);
extern void lvl_carve(Level* lvl, RECT* rect, cu32 iterations);
extern void lvl_draw(const Level* lvl, cu32 sbb);

#endif //LEVEL
