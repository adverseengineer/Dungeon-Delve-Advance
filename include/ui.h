//Nick Sells, 2021
//ui.h

#ifndef UI_H
#define UI_H

#include "config.h"

//true if the ui needs to be redrawn, false otherwise
static BOOL ui_isDirty;

//renders the ui
static inline void ui_draw(void) {
	tte_write("The Quick Brown Fox Jumped Over The Lazy Dog");
	bg_window(SBB_UI_GFX, 3, 3, 6, 6, 4);
}

#endif //UI_H