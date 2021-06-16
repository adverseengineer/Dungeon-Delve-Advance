//Nick Sells, 2021
//skeleton.h

//NOTE: this file only serves to split up actor behaviour functions instead of
//dumping them all in actor.h. it should never be included into any other file

#ifndef ACTOR_SKELETON_H
#define ACTOR_SKELETON_H

static const Info actor_info_skeleton = {
	.name = "Skeleton",
	.desc = "The reanimated remains of a long-dead adventurer. Maybe it still has some of it's loot"
};

static void actor_move_skeleton(Actor* self) {
	actor_move_base(self);
}

static void actor_attack_skeleton(Actor* self, Actor* other) {
	actor_attack_base(self, other);
}

static void actor_chase_skeleton(Actor* self, Actor* other) {
	actor_chase_base(self, other);
}

static void actor_flee_skeleton(Actor* self, Actor* other) { 
	actor_flee_base(self, other);
}

static void actor_idle_skeleton(Actor* self) {
	actor_idle_base(self);
}

static void actor_die_skeleton(Actor* self) {
	actor_die_base(self);
}

#endif //ACTOR_SKELETON_H
