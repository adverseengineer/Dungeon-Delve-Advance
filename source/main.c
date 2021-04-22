

#include <AAS.h>
#include <tonc.h>
#include "level.h"
#include "player.h"
#include "enemy.h"
//gfx
#include "gfx_dungeon.h"
#include "gfx_actors.h"
//data
#include "data/enemy_table.h"

#define sbb 28

// a shadow copy of oam
OBJ_ATTR obj_buffer[128];

int main(void) {
	GRIT_CPY(tile_mem, gfx_dungeonTiles);
	GRIT_CPY(pal_bg_mem, gfx_dungeonPal);
	GRIT_CPY(tile_mem_obj, gfx_actorsTiles);
	GRIT_CPY(pal_obj_mem, gfx_actorsPal);

	Player plr = {
		pos: {-64, -16}
	};

	Level* lvl = lvl_create(8, 8);
	lvl->tiles = (u8[]) {
		1, 1, 1, 1, 1, 1, 1, 1, 
		1, 2, 2, 2, 2, 2, 2, 1, 
		1, 2, 1, 2, 2, 1, 2, 1, 
		1, 2, 2, 2, 2, 2, 2, 1, 
		1, 2, 2, 2, 2, 2, 2, 1, 
		1, 2, 1, 2, 2, 1, 2, 1, 
		1, 2, 2, 2, 2, 2, 2, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 
	};

	Enemy* enm = lvl->enemies[0];

	*enm = (Enemy) {
		data: ENEMY_BEHOLDER,
		pos: {-64, 0}
	};

	//clear the shadow copy to safe values
	oam_init(obj_buffer, 128);

	OBJ_ATTR* plr_sprite = &obj_buffer[0];
	obj_set_attr(plr_sprite, ATTR0_SQUARE, ATTR1_SIZE_16, 0);
	obj_set_pos(plr_sprite, PLR_SCR_POS_X, PLR_SCR_POS_Y);

	OBJ_ATTR* enm_sprite = &obj_buffer[1];
	obj_set_attr(enm_sprite, ATTR0_SQUARE, ATTR1_SIZE_16, 512);
	obj_set_pos(enm_sprite, PLR_SCR_POS_X + 16, PLR_SCR_POS_Y + 16);

	oam_copy(oam_mem, obj_buffer, 2);

	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
	REG_BG0CNT = BG_CBB(0) | BG_SBB(sbb) | BG_REG_64x64;

	irq_init(NULL);
	irq_enable(II_VBLANK);

	lvl_draw(lvl, sbb);

	while(TRUE) {
		VBlankIntrWait();
		key_poll();
		plr_move(&plr, lvl, 0);
		obj_set_pos(enm_sprite, enm->pos.x, enm->pos.y);
		oam_copy(&oam_mem[1], &obj_buffer[1], 1);
	}
}
