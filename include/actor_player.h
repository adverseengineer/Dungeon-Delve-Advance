//Nick Sells, 2021
//actor_player.h

//NOTE: this file only serves to split up actor behaviour functions instead of
//dumping them all in actor.h. it should never be included into any other file

#ifndef ACTOR_PLAYER_H
#define ACTOR_PLAYER_H

typedef Actor Player;

static const Info actor_player_info = {
	.name = "Player",
	.desc = "You are an adventurer seeking fame and fortune. Will you find it in the dungeon?"
};

static void actor_player_move(Player* this) {
	actor_base_move(this);
}

static void actor_player_attack(Player* this, Actor* other) {
	actor_base_attack(this, other);
}

static void actor_player_chase(Player* this, Actor* other) {
	actor_base_chase(this, other);
}

static void actor_player_flee(Player* this, Actor* other) {
	actor_base_flee(this, other);
}

static void actor_player_idle(Player* this) {
	actor_base_idle(this);
}

static void actor_player_die(Player* this) {
	actor_base_die(this);
}

#endif //ACTOR_PLAYER_H
