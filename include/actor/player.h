//Nick Sells, 2021
//player.h

//NOTE: this file only serves to split up actor behaviour functions instead of
//dumping them all in actor.h. it should never be included into any other file

#ifndef ACTOR_PLAYER_H
#define ACTOR_PLAYER_H

static const Info actor_info_player = {
	.name = "Player",
	.desc = "You are an adventurer seeking fame and fortune. Will you find it in the dungeon?"
};

static void actor_move_player(Actor* self) {
	actor_move_base(self);
}

static void actor_attack_player(Actor* self, Actor* other) {
	actor_attack_base(self, other);
}

static void actor_chase_player(Actor* self, Actor* other) {
	actor_chase_base(self, other);
}

static void actor_flee_player(Actor* self, Actor* other) {
	actor_flee_base(self, other);
}

static void actor_idle_player(Actor* self) {
	actor_idle_base(self);
}

static void actor_die_player(Actor* self) {
	actor_die_base(self);
}

#endif //ACTOR_PLAYER_H
