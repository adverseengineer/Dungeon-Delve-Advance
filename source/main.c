

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

	Player plr = {
		pos: {-64, -16}
	};

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

	*lvl->enemies[0] = (Enemy) {
		data: ENEMY_BEHOLDER,
		pos: {0, 0}
	};


	//TODO: make a level-coords to screen-coords mapping function 
	Sprite* plr_sprite = spr_create(plr_scrPosX, plr_scrPosY, 8, ANIM_CACO_IDLE);
	
	spr_render();

	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
	REG_BG0CNT = BG_CBB(0) | BG_SBB(sbb) | BG_REG_64x64 | BG_8BPP;

	irq_init(NULL);
	irq_enable(II_VBLANK);

	lvl_draw(lvl, sbb);

	while(TRUE) {
		Sprite* pSpr = NULL;

		VBlankIntrWait();
		key_poll();
		plr_move(&plr, lvl, 0);
		
		if(key_hit(KEY_A)) {
			u32 rx = qran_range(0, 225);
			u32 ry = qran_range(0, 145);
			pSpr = spr_create(rx, ry, 0, ANIM_BAT_IDLE);
		}

		if(key_hit(KEY_B)) {
			spr_destroy(pSpr);
		}

		spr_animStep(plr_sprite);
		spr_render();
	}
}
