//Nick Sells, 2021
//actor_base.h

//NOTE: this file only serves to split up actor behaviour functions instead of
//dumping them all in actor.h. it should never be included into any other file

#ifndef ACTOR_BASE_H
#define ACTOR_BASE_H

static const Info actor_base_info = {
	.name = "Base Actor",
	.desc = "A generic actor with no special properties"
};

static void actor_base_move(Actor* this) {
	mgba_printf(LOG_INFO, "%s, moved from it's previous position");
}

static void actor_base_attack(Actor* this, Actor* other) {
	mgba_printf(LOG_INFO, "%s donkey-punched %s!", this->info.name, other->info.name);
}

static void actor_base_chase(Actor* this, Actor* other) {
	mgba_printf(LOG_INFO, "%s relentlessly pursues %s!", this->info.name, other->info.name);
}

static void actor_base_flee(Actor* this, Actor* other) {
	mgba_printf(LOG_INFO, "%s runs for it's life from %s!", this->info.name, other->info.name);
}

static void actor_base_idle(Actor* this) {
	mgba_printf(LOG_INFO, "%s does diddly-squat!", this->info.name);
}

static void actor_base_die(Actor* this) {
	mgba_printf(LOG_INFO, "%s shuffles off this mortal coil!", this->info.name);
}

#endif //ACTOR_BASE_H
