//Nick Sells, 2021
//item.h

#ifndef ITEM
#define ITEM

// TODO: items will cost 1.25x their price and sell for 0.8x their price \
certain perks can improve this exchange rate

//=============================================================================
//STRUCTS
//=============================================================================

typedef struct ItemData {
	char* name;
	char* desc;
	u8 category;
	u8 price;
} ALIGN4 ItemData;

typedef struct Item {
	ItemData* data;
	u8 amount;
} ALIGN4 Item;

#endif //ITEM
