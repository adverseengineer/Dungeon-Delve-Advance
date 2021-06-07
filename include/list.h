//Nick Sells, 2021
//list.h

#ifndef LIST_H
#define LIST_H

typedef struct List {
	void* data;
	u32 size; //how many elements
	u32 capacity; //max elements
} ALIGN4 List;

//TODO: finish writing this and use it to contain the actors bound to a level
//TODO: also use this to replace the OAM linked list scheme

#endif //LIST_H