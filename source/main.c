
#include <tonc.h>

#include "common.h"
#include "config.h"

#include "level.h"
#include "ui.h"

#include "player.h"

//gfx
#include "gfx_lvl.h"
#include "gfx_ui.h"

#include "gfx_player_warrior.h"
#include "gfx_player_mage.h"
#include "gfx_skeleton.h"

static inline void vid_init(void) {

	//enable vblank interrupts
	irq_init(NULL);
	irq_enable(II_VBLANK);

	//enable all bg layers
	REG_DISPCNT = DCNT_MODE0
				| BG(BG_TXT)
				| BG(BG_UI)
				| BG(BG_LVL)
				| DCNT_OBJ | DCNT_OBJ_1D;
	//set up the text layer
	tte_init_chr4c_default(BG_TXT, BG_CBB(CBB_TXT) | BG_SBB(SBB_TXT));
	tte_set_font(&tahoma9Font);
	//set up the ui layer
	REG_BGCNT[BG_UI] = BG_CBB(CBB_UI) | BG_SBB(SBB_UI) | BG_REG_32x32 | BG_4BPP;
	//set up the level layer
	REG_BGCNT[BG_LVL] = BG_CBB(CBB_LVL) | BG_SBB(SBB_LVL) | BG_REG_64x64 | BG_4BPP;

	//copy lvl gfx
	GRIT_CPY(tile_mem[CBB_LVL], gfx_lvlTiles);
	GRIT_CPY(pal_bg_bank[PB_LVL], gfx_lvlPal);
	//copy ui gfx
	GRIT_CPY(tile_mem[CBB_UI], gfx_uiTiles);
	GRIT_CPY(pal_bg_bank[PB_UI], gfx_uiPal);
	//copy sprite gfx
	GRIT_CPY(tile_mem_obj, gfx_player_warriorTiles);
	GRIT_CPY(pal_obj_mem, gfx_player_warriorPal);
	//
	//set up sprite list and oam
	spr_init();
}

int main(void) {

	vid_init();

	Player* plr = plr_create();
	Level* lvl = lvl_create();
	lvl->player = plr;

	lvl_build(lvl);
	lvl_draw(lvl);

	while(TRUE) {
		VBlankIntrWait();
		key_poll();

		if(key_hit(KEY_A | KEY_B)) {
			lvl_build(lvl);

			lvl_erase();
			lvl_draw(lvl);
		}

		lvl_scroll(lvl);

		ui_draw();
		ui_update();

		spr_render();
	}
}

// TODO: each room except entrance and exits have a chance to have enemies in them. when
//generating a room, spawn a random number of enemies in there. dynamically allocate the
//entities and add them to the entity array. the mob cap is 32

// TODO: place entries/exits
// TODO: room types
// TODO: decor tiles (grass, water)
// TODO: place n chests in every map with at least m empty spaces in every direction
// TODO: implement level themes
// TODO: consider scrapping chasm levels. i really like the tightly clustered result of 3 recursions on a 32x32 map

// TODO: in most cases, water and foliage will cost more than regular terrain
// TODO: all terrain except walls and none will cost the same for flying enemies
// TODO: lightning will do double damage if the target is standing on water
// TODO: flying enemies will take double damage from lightning
// TODO: slimes will prefer water tiles
// TODO: all enemies will prefer water tiles when on fire
// TODO: foliage tiles will be flammable and spread
// TODO: some enemies can't go through doors because they are "too big"
// TODO: place a door to a shop somewhere on the outer edge of a level. leads to a shop level that you come ang go from

// TODO: flooded will use cellular automata to place water tiles throughout the level
// TODO: overgrown will randomly place foliage everywhere
// TODO: ruined will knock out huge (non-essential) chunks of floor
// TODO: chasm will make the hallways catwalks and the rooms hollow pillars

// TODO: entry will contain the stairs to the floor above
// TODO: exit will contain the stairs to the floor below

// TODO: the level details like cracks and moldy walls will be randomly generated but every time the player leaves a floor, it's state will be saved to sram

//TODO: (JUN 14th) if the wall between two rooms is only one block thick, roll to make it destructible by a bomb
