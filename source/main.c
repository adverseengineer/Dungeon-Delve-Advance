
#include <tonc.h>

#include "common.h"
#include "config.h"

#include "actor_level_common.h"
#include "ui.h"

//gfx
#include "gfx_lvl.h"
#include "gfx_ui.h"

#include "gfx_player_warrior.h"

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
	REG_BG1CNT = BG_CBB(CBB_UI) | BG_SBB(SBB_UI) | BG_REG_32x32 | BG_4BPP;
	//set up the level layer
	REG_BG2CNT = BG_CBB(CBB_LVL) | BG_SBB(SBB_LVL) | BG_REG_64x64 | BG_4BPP;

	//copy lvl gfx
	GRIT_CPY(tile_mem[CBB_LVL], gfx_lvlTiles);
	GRIT_CPY(pal_bg_bank[PB_LVL], gfx_lvlPal);
	//copy ui gfx
	GRIT_CPY(tile_mem[CBB_UI], gfx_uiTiles);
	GRIT_CPY(pal_bg_bank[PB_UI], gfx_uiPal);
	//copy sprite gfx
	GRIT_CPY(tile_mem_obj, gfx_player_warriorTiles);
	GRIT_CPY(pal_obj_mem, gfx_player_warriorPal);

	spr_init();
}

int main(void) {

	vid_init();

	Level* lvl = lvl_create();
	Actor* atr1 = lvl_spawnActor(lvl, ACTOR_BASE, 1, 1);
	Actor* atr2 = lvl_spawnActor(lvl, ACTOR_BASE, 3, 2);

	spr_link(atr1->sprite);
	spr_link(atr2->sprite);

	lvl_build(lvl);
	lvl_draw(lvl);

	while(TRUE) {
		VBlankIntrWait();
		key_poll();

		lvl_scroll(lvl);
		spr_render();

		// ui_draw();
		// ui_update();
	}
}
