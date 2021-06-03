//Nick Sells, 2021
//ui.h

#ifndef UI_H
#define UI_H

#include "config.h"

//true if the ui needs to be redrawn, false otherwise
static BOOL ui_needRedraw = TRUE;
static BOOL ui_inventoryVisible;

static POINT16 ui_selectedPanel;

//renders the ui
static inline void ui_draw(void) {
	if(ui_needRedraw) {
		bg_plot_m(SBB_UI, 0, 0, SE_HP_ICON);
		bg_plot_m(SBB_UI, 0, 1, SE_MP_ICON);

		if(ui_inventoryVisible)
			bg_rect_m(SBB_UI, 3, 2, 11, 8, SE_ITEM_PANE);
		else
			bg_rect_m(SBB_UI, 3, 2, 11, 8, SE_NONE);

		ui_needRedraw = FALSE;
	}
}

//updates the ui based on player inputs
static inline void ui_update(void) {
	if(key_hit(KEY_START)) {
		if(!ui_inventoryVisible)
			ui_inventoryVisible = TRUE;
		else
			ui_inventoryVisible = FALSE;

		ui_needRedraw = TRUE;
	}
}

#endif //UI_H