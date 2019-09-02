
#include "uniqueOrderedList.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct node_t *Node;
struct node_t{
    Element element;
    Node next;
};


struct uniqueOrderedList_t{
    Node head;
    int size;
    copyElements copyFunc;
    freeElements freeFunc;
    Node iterator;
};

Node nodeCreate(Element element, copyElements copy){
    Node newNode = malloc(sizeof(*newNode));
    if(newNode == NULL){
        // lazem yem7a kolshe 3mlo l7d hassa

        return NULL;
    }
    newNode->element = copy(element);
    newNode->next = NULL;
    return newNode;
}

void nodeDestroy(Node node, freeElements freeFun){
    freeFun(node->element);
    free(node);
}

UniqueOrderedList uniqueOrderedListCreate(copyElements copyFF, freeElements freeFF){
    if(!copyFF || !freeFF){
        return NULL;
    }

    UniqueOrderedList newList = malloc(sizeof(*newList));
    if(!newList){
        return NULL;
    }
    newList->head = NULL;
    newList->size = 0;
    newList->copyFunc = copyFF;
    newList->freeFunc = freeFF;
    //bl3md lo et7alte el-iterator
    return newList;
}

void uniqueOrderedListDestroy(UniqueOrderedList list){
    if(list == NULL){
        return;
    }
    while(list->head != NULL){
        Node willDelete = list->head;
        list->head = list->head->next;
        list->freeFunc(willDelete->element);
        free(willDelete);
    }
    free(list);
}

UniqueOrderedList uniqueOrderedListCopy(UniqueOrderedList list){
    if(!list){
        return NULL;
    }
    UniqueOrderedList newList = uniqueOrderedListCreate(list->copyFunc, list->freeFunc);
    newList->size = list->size;

    Node tmp = list->head;
    Node tmpNew = NULL;
    while(tmp != NULL){
        Node newNode = nodeCreate(tmp->element, list->copyFunc);
        if(newList->head == NULL){
            newList->head = newNode;
            tmpNew = newList->head;
        }
        else{
            tmpNew->next = newNode;
            tmpNew = newNode;
        }
        tmp = tmp->next;
    }

    return newList;

}

int uniqueOrderedListSize(UniqueOrderedList list){
    if(list == NULL){
        return -1;
    }
    return list->size;
}

bool uniqueOrderedListContains(UniqueOrderedList list, Element element, elementsEquals equFunc){
    if(list == NULL || element == NULL){
        return false;
    }
    Node tmp = list->head;
    while(tmp != NULL){
        if(equFunc(tmp->element, element) == true){
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

UniqueOrderedListResult uniqueOrderedListInsert(UniqueOrderedList list, Element element, elementsEquals equFunc, elementGreaterThan greaFunc){
    if(list == NULL || element == NULL){
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }
    if(list->size != 0) {
        Node tmp = list->head;
        Node last_tmp = tmp;
        while (tmp != NULL) {
            if (equFunc(tmp->element, element) == true) {
                return UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS;
            }
            if (greaFunc(element, tmp->element)) {
                last_tmp = tmp;
                tmp = tmp->next;
            }
            else{
                Node newNode = nodeCreate(element, list->copyFunc);
                if(tmp == list->head){
                    newNode->next = list->head;
                    list->head = newNode;
                }
                else{
                    newNode->next = tmp;
                    last_tmp->next = newNode;
                }
                list->size++;
                return UNIQUE_ORDERED_LIST_SUCCESS;
            }
        }
        Node newNode = nodeCreate(element, list->copyFunc);
        last_tmp->next = newNode;
        list->size++;
        return UNIQUE_ORDERED_LIST_SUCCESS;

    }
    else{
        list->head = nodeCreate(element, list->copyFunc); // what if the allocation failed?
        list->size++;
        return UNIQUE_ORDERED_LIST_SUCCESS;
    }

}

UniqueOrderedListResult uniqueOrderedListRemove(UniqueOrderedList list, Element element, elementsEquals equFunc){
    if(list == NULL || element == NULL){
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }

    Node tmp = list->head;
    Node last_tmp = tmp;
    while(tmp != NULL){
        if (equFunc(tmp->element, element) == true) {
            Node newNode = tmp->next;
            nodeDestroy(tmp, list->freeFunc);
            if(tmp == list->head){
                list->head = newNode;
            }
            else{
                last_tmp->next = newNode;
            }
            list->size--;
            return UNIQUE_ORDERED_LIST_SUCCESS;
        }
        last_tmp = tmp;
        tmp = tmp->next;
    }
    return UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST;
}

Element uniqueOrderedListGetLowest(UniqueOrderedList list){
    if(list == NULL || list->size == 0){
        return NULL;
    }
    list->iterator = list->head;
    return list->iterator->element;
}

Element uniqueOrderedListGetGreatest(UniqueOrderedList list){
    if(list == NULL || list->size == 0){
        return NULL;
    }
    list->iterator = list->head;
    if(list->size == 1){
        return list->iterator->element;
    }
    while(list->iterator->next != NULL){
        list->iterator = list->iterator->next;
    }
    return list->iterator->element;
}

Element uniqueOrderedListGetNext(UniqueOrderedList list) {
    if (!list || list->size == 0) {
        return NULL;
    }

    if (list->iterator != NULL) {
        list->iterator = list->iterator->next;
        if(list->iterator != NULL){
            return list->iterator->element;
        }
        return NULL;
    }
    return NULL;
}

void uniqueOrderedListClear(UniqueOrderedList list){
    if(!list){
        return;
    }

    while (list->head) {
        Node ptr=list->head;
        list->head=ptr->next;
        list->freeFunc(ptr->element);
        free(ptr);
    }
    list->size = 0;
}