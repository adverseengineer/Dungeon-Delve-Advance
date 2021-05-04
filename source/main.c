

#include <AAS.h>
#include <tonc.h>
#include "level.h"
#include "sprite.h"
#include "player.h"
#include "enemy.h"
//gfx
#include "gfx_tiles.h"
#include "gfx_sprites.h"
//data
#include "data/anim_table.h"
#include "data/enemy_table.h"
//libs
#include "lib/posprintf.h"

#define sbb 28

int main(void) {

	GRIT_CPY(tile_mem, gfx_tilesTiles);
	GRIT_CPY(pal_bg_mem, gfx_tilesPal);
	GRIT_CPY(tile_mem_obj, gfx_spritesTiles);
	GRIT_CPY(pal_obj_mem, gfx_spritesPal);

	pal_bg_mem[2] = CLR_GRAY;

	Level* lvl = lvl_create(10, 10);
	lvl->tiles = (u8[]) {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 2, 2, 2, 2, 2, 2, 2, 2, 1,
		1, 2, 2, 2, 2, 2, 2, 2, 2, 1,
		1, 2, 2, 2, 2, 2, 2, 2, 2, 1,
		1, 2, 2, 2, 2, 2, 2, 2, 2, 1,
		1, 2, 2, 2, 2, 2, 2, 2, 2, 1,
		1, 2, 2, 2, 2, 2, 2, 2, 2, 1,
		1, 2, 2, 2, 2, 2, 2, 2, 2, 1,
		1, 2, 2, 2, 2, 2, 2, 2, 2, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};
	
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
	REG_BG0CNT = BG_CBB(0) | BG_SBB(sbb) | BG_REG_64x64 | BG_8BPP;

	irq_init(NULL);
	irq_enable(II_VBLANK);

	lvl_draw(lvl, sbb);

	spr_init();

	//TODO: make a level-coords to screen-coords mapping function 

	while(TRUE) {
		VBlankIntrWait();
		key_poll();
		
		Sprite* spr = NULL;

		if(key_hit(KEY_A)) {
			u32 rx = qran_range(0, SCREEN_WIDTH - 15);
			u32 ry = qran_range(0, SCREEN_HEIGHT - 15);
			spr = spr_create(rx, ry, 0, ANIM_BAT_IDLE, TRUE);
		}

		if(key_hit(KEY_B))
			spr_destroy(spr);

		spr_render();
	}
}
