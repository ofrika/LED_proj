#ifndef MIXEDLIST_H_
#define MIXEDLIST_H_
#include <stdbool.h>
#include "helper.h"

typedef struct mixedList_t* MixedList;

typedef enum MixedListResult_t {
    MIXED_LIST_SUCCESS,
    MIXED_LIST_NULL_ARGUMENT,
    MIXED_LIST_ITEM_DOES_NOT_EXIST,
    MIXED_LIST_ITEM_ALREADY_EXISTS,
    MIXED_LIST_ANOTHER_ITEM_ALREADY_LOCATED_THERE
} MixedListResult;

typedef void* Element;

MixedList mixedListCreate();

void mixedListDestroy(MixedList);

int mixedListSize(MixedList);

MixedListResult mixedListInsert(MixedList, Element, Object_Type);

MixedListResult mixedListRemove(MixedList, int);

int mixedListGetFirstID(MixedList);

int mixedListGetLastID(MixedList);

int mixedListGetNextID(MixedList);

void mixedListClear(MixedList);

#endif
