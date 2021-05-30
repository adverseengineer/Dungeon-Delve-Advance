
#include <tonc.h>

#include "common.h"
#include "config.h"

#include "level.h"

//gfx
#include "gfx_tiles.h"
#include "gfx_sprites.h"

static inline void vid_init(void) {
	//enable vblank interrupts
	irq_init(NULL);
	irq_enable(II_VBLANK);

	//enable all bg layers
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_OBJ | DCNT_OBJ_1D;
	//set up the text layer
	tte_init_chr4c_default(0, BG_CBB(3) | BG_SBB(8));
	tte_set_font(&tahoma9Font);
	//set up the ui layer
	REG_BG1CNT = BG_SBB(9) | BG_REG_32x32 | BG_4BPP;
	//set up the level layer
	REG_BG2CNT = BG_SBB(10) | BG_REG_64x64 | BG_4BPP;

	//copy all the graphics from rom to vram
	GRIT_CPY(tile_mem[0], gfx_tilesTiles);
	GRIT_CPY(pal_bg_mem, gfx_tilesPal);
	GRIT_CPY(tile_mem_obj, gfx_spritesTiles);
	GRIT_CPY(pal_obj_mem, gfx_spritesPal);
}

int main(void) {
	vid_init();

	Level* l = lvl_create();
	lvl_build(l);

	Actor* a = actor_create(ACTOR_BASE, 0 , 0);
	Actor* b = actor_create(ACTOR_SKELETON, 10, 10);

	while(TRUE) {
		VBlankIntrWait();

		lvl_scroll(l);
		lvl_draw(l);

		spr_render();

		key_poll();
		if(key_hit(KEY_A)) {
			actor_move(a, 1, 0);
			a->attack(a, b);
		}
	}
}
