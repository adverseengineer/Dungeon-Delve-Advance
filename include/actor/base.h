//Nick Sells, 2021
//base.h

//NOTE: this file only serves to split up actor behaviour functions instead of
//dumping them all in actor.h. it should never be included into any other file

#ifndef ACTOR_BASE_H
#define ACTOR_BASE_H

static const Info actor_info_base = {
	.name = "Base Actor",
	.desc = "A generic actor with no special properties"
};

static void actor_move_base(Actor* self) {
	mgba_printf(LOG_INFO, "%s, moved from it's previous position");
}

static void actor_attack_base(Actor* self, Actor* other) {
	mgba_printf(LOG_INFO, "%s donkey-punched %s!", self->info.name, other->info.name);
}

static void actor_chase_base(Actor* self, Actor* other) {
	mgba_printf(LOG_INFO, "%s relentlessly pursues %s!", self->info.name, other->info.name);
}

static void actor_flee_base(Actor* self, Actor* other) {
	mgba_printf(LOG_INFO, "%s runs for it's life from %s!", self->info.name, other->info.name);
}

static void actor_idle_base(Actor* self) {
	mgba_printf(LOG_INFO, "%s does diddly-squat!", self->info.name);
}

static void actor_die_base(Actor* self) {
	mgba_printf(LOG_INFO, "%s shuffles off this mortal coil!", self->info.name);
}

#endif //ACTOR_BASE_H
