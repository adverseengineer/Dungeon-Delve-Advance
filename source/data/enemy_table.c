//Nick Sells, 2021
//enemy_table.c

#include "data/enemy_table.h"

const EnemyData enemy_table[16] = {
	(EnemyData) {
		name: "Beholder",
		desc: "A massive eyeball creature that uses psionic powers"
	},
	(EnemyData) {
		name: "Sewer Rat",
		desc: "A rodent of unusual size."
	},
	(EnemyData) {
		name: "Skeleton",
		desc: "The reanimated remains of a long-dead adventurer",
	},
	(EnemyData) {
		name: "Goo",
		desc: "A sentient mass of who-knows-what",
	},
	(EnemyData) {
		name: "Mimic",
		desc: "A man-eating shapeshifter disguised as a chest"
	},
	(EnemyData) {
		name: "Vampire Bat",
		desc: "A blood-sucking avian terror. Watch the cielings."
	},
	(EnemyData) {
		name: "Stone Golem",
		desc: "A stone statue brought to life by dark magic",
	},
};
