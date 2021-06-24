//Nick Sells, 2021
//actor_skeleton.h

//NOTE: this file only serves to split up actor behaviour functions instead of
//dumping them all in actor.h. it should never be included into any other file

#ifndef ACTOR_SKELETON_H
#define ACTOR_SKELETON_H

typedef Actor Skeleton;

static const Info actor_skeleton_info = {
	.name = "Skeleton",
	.desc = "The reanimated remains of a long-dead adventurer. Maybe it still has some of it's loot"
};

static void actor_skeleton_move(Skeleton* this) {
	actor_base_move(this);
}

static void actor_skeleton_attack(Skeleton* this, Actor* other) {
	actor_base_attack(this, other);
}

static void actor_skeleton_chase(Skeleton* this, Actor* other) {
	actor_base_chase(this, other);
}

static void actor_skeleton_flee(Skeleton* this, Actor* other) { 
	actor_base_flee(this, other);
}

static void actor_skeleton_idle(Skeleton* this) {
	actor_base_idle(this);
}

static void actor_skeleton_die(Skeleton* this) {
	actor_base_die(this);
}

#endif //ACTOR_SKELETON_H
