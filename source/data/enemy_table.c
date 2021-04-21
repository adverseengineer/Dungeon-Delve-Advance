//Nick Sells, 2021
//enemy_table.c

#include "data/enemy_table.h"

const EnemyData enemy_table[16] = {
	(EnemyData) {
		name: "Skeleton",
		desc: "The reanimated remains of a long-dead adventurer",
	},
	(EnemyData) {
		name: "Goblin",
		desc: "A vicious little creature with a taste for adventurers",
	},
	(EnemyData) {
		name: "Giant Spider",
		desc: "A spider the size of a wagon wheel",
	},
	(EnemyData) {
		name: "Slime",
		desc: "A sentient mass of who-knows-what",
	},
	(EnemyData) {
		name: "Mimic",
		desc: "A man-eating shapeshifter disguised as a chest"
	},
	(EnemyData) {
		name: "Vampire Bat",
		desc: "A blood-sucking avian terror. Watch the cielings."
	}
};
