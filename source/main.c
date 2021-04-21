

#include <AAS.h>
#include <tonc.h>
#include "level.h"
#include "player.h"
//gfx
#include "gfx_dungeon.h"
#include "gfx_player.h"

#define sbb 28

// a shadow copy of oam
OBJ_ATTR obj_buffer[128];

int main(void) {
	GRIT_CPY(tile_mem, gfx_dungeonTiles);
	GRIT_CPY(pal_bg_mem, gfx_dungeonPal);
	GRIT_CPY(tile_mem_obj, gfx_playerTiles);
	GRIT_CPY(pal_obj_mem, gfx_playerPal);

	Player plr = {
		pos: {0, 0}
	};

	//clear the shadow copy to safe values
	oam_init(obj_buffer, 128);

	OBJ_ATTR* sprite = &obj_buffer[0];
	obj_set_attr(sprite, ATTR0_SQUARE, ATTR1_SIZE_16, 0);
	obj_set_pos(sprite, PLR_GET_X(&plr), PLR_GET_Y(&plr));

	oam_copy(oam_mem, obj_buffer, 1);

	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
	REG_BG0CNT = BG_CBB(0) | BG_SBB(sbb) | BG_REG_64x64;

	irq_init(NULL);
	irq_enable(II_VBLANK);

	Level* lvl = lvl_create(32, 32);

	while(TRUE) {
		VBlankIntrWait();
		key_poll();
		plr_move(&plr, lvl, 0);
		if(key_hit(KEY_START)) {

			SBB_CLEAR(sbb);
			SBB_CLEAR(sbb+1);
			SBB_CLEAR(sbb+2);
			SBB_CLEAR(sbb+3);
			memset32(lvl->tiles, 0, (lvl->width * lvl->height) / 4);
			RECT original = {1, 1, lvl->width - 1, lvl->height - 1};
			lvl_carve(lvl, &original, NUM_RECUR);
			lvl_draw(lvl, sbb);
		}
	}

	while(TRUE);
}
