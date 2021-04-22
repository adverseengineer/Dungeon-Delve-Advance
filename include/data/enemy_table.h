//Nick Sells, 2021
//enemy_table.h

#ifndef ENEMY_TABLE
#define ENEMY_TABLE

#include "enemy.h"

#define ENEMY_BEHOLDER  (&enemy_table[0])
#define ENEMY_RAT       (&enemy_table[1])
#define ENEMY_SKELETON  (&enemy_table[2])
#define ENEMY_GOO       (&enemy_table[3])
#define ENEMY_MIMIC     (&enemy_table[4])
#define ENEMY_BAT       (&enemy_table[5])
#define ENEMY_GOLEM     (&enemy_table[6])

extern const EnemyData enemy_table[16];

#endif //ENEMY_TABLE
