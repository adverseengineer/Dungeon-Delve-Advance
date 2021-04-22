//Nick Sells, 2021
//anim_table.h

#ifndef ANIM_TABLE
#define ANIM_TABLE

#include "anim.h"

#define ANIM_PLAYER_IDLE    (&anim_table[0])
#define ANIM_PLAYER_WALK    (&anim_table[1])
#define ANIM_PLAYER_ACTION  (&anim_table[2])

#define ANIM_CACO_IDLE      (&anim_table[3])
#define ANIM_RAT_IDLE       (&anim_table[4])
#define ANIM_SKEL_WALK      (&anim_table[5])
#define ANIM_GOO_IDLE       (&anim_table[6])
#define ANIM_MIMIC_IDLE     (&anim_table[7])
#define ANIM_MIMIC_CHASE    (&anim_table[8])
#define ANIM_BAT_IDLE       (&anim_table[9])
#define ANIM_GOLEM_WALK     (&anim_table[10])

extern const AnimData anim_table[32];

#endif //ANIM_TABLE
