#ifndef SRC_LIST_H_
#define SRC_LIST_H_
#include <stdbool.h>

typedef struct list_t* List;
typedef enum type_e{
    Text,
    Picture,
    SubBoard,
    UNDEF_TYPE
}Type;

typedef enum ListResult_t {
    LIST_SUCCESS,
    LIST_NULL_ARGUMENT,
    LIST_ITEM_DOES_NOT_EXIST
} ListResult;

typedef void* Element;

List listCreate();

void listDestroy(List);

int listSize(List);

ListResult listInsert(List, Element, Type);

ListResult listRemove(List, Element);

Element listGetFirst(List);

Type listGetIteratorType(List list);

Element listGetLast(List);

Element listGetNext(List);


#endif
