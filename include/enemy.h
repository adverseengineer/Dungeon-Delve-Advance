//Nick Sells, 2021
//enemy.h

#ifndef ENEMY
#define ENEMY

#include <tonc_types.h>
#include <tonc_math.h>
#include <tonc_mgba.h>

#include "player.h"

//=============================================================================
//STRUCTS
//=============================================================================

typedef struct EnemyData {
	char* name;
	char* desc;
} ALIGN4 EnemyData;

typedef struct Enemy {
	const EnemyData* data;
	POINT16 pos;
} ALIGN4 Enemy;

//=============================================================================
//FUNCTIONS
//=============================================================================

// //pathfinds towards the player
// //gets called once per frame
// //TODO: make this actual pathfinding instead of approaching
// inline void enm_chase(Enemy* enm, Player* plr) {
// 	if(enm->pos.x < plr->pos.x)      enm->pos.x++;
// 	else if(plr->pos.x < enm->pos.x) enm->pos.x--;

// 	if(enm->pos.y < plr->pos.y)      enm->pos.y++;
// 	else if(plr->pos.y < enm->pos.y) enm->pos.y--;

// 	mgba_printf(LOG_INFO, "enm->pos = (%d,%d)", enm->pos);
// }

#endif //ENEMY
