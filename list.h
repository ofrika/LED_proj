#ifndef LIST_H_
#define LIST_H_
#include <stdbool.h>

typedef struct list_t* List;

typedef enum ListResult_t {
    LIST_SUCCESS,
    LIST_NULL_ARGUMENT,
    LIST_ITEM_DOES_NOT_EXIST,
    LIST_ITEM_ALREADY_EXISTS,
    LIST_ANOTHER_ITEM_ALREADY_LOCATED_THERE
} ListResult;

typedef void* Element;

List listCreate();

void listDestroy(List);

int listSize(List);

ListResult listInsert(List, Element);

ListResult listRemove(List, Element);

Element listGetFirst(List);

Element listGetLast(List);

Element listGetNext(List);

void listClear(List);

#endif
