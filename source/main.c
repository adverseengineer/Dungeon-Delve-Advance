
#include <tonc.h>

#include "common.h"
#include "config.h"

#include "level.h"
#include "ui.h"

//gfx
#include "gfx_lvl.h"
#include "gfx_ui.h"

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

	spr_init();
}

int main(void) {
	vid_init();

	Level* l = lvl_create();
	lvl_build(l);
	lvl_draw(l);

	Actor* a = actor_create(ACTOR_BASE, 0, 0);
	Actor* b = actor_create(ACTOR_SKELETON, 10, 10);

	while(TRUE) {
		VBlankIntrWait();
		key_poll();

		lvl_scroll(l);

		ui_draw();
		ui_update();

		spr_render();

		if(key_hit(KEY_A)) {
			actor_move(a, 1, 0);
			a->attack(a, b);
		}
	}
}
