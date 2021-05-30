//Nick Sells, 2021
//item.h

//represents an item type and instances of that item

#ifndef ITEM_H
#define ITEM_H

#include "info.h"

// TODO: items will cost 1.25x their price and sell for 0.8x their price (rounded up)

//=============================================================================
//STRUCTS
//=============================================================================

typedef struct ItemData {
	Info info;
	u8 category;
	u8 price;
} ALIGN4 ItemData;

typedef struct Item {
	ItemData* data;
	u8 amount;
} ALIGN4 Item;

#endif //ITEM_H
