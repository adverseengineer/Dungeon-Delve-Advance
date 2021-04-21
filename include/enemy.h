//Nick Sells, 2021
//enemy.h

#ifndef ENEMY
#define ENEMY

#include <tonc_types.h>

//=============================================================================
//STRUCTS
//=============================================================================

typedef struct {
	char* name;
	char* desc;
} ALIGN4 EnemyData;

typedef struct {
	EnemyData* data;
	POINT16 pos;
} ALIGN4 Enemy;

#endif //ENEMY
