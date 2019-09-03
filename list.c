#include "list.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct node_t *Node;

struct node_t{
    Element element;
    Node next;
    Type type;
};

struct list_t{
    Node head;
    int size;
    Node iterator;
};

Node nodeCreate(Element element, Type type){
    Node newNode = malloc(sizeof(*newNode));
    if(!newNode){
        return NULL;
    }
    newNode->element = element;
    newNode->type = type;
    newNode->next = NULL;
    return newNode;
}

void nodeDestroy(Node node){
    free(node);
}

List listCreate(){
    List newList = malloc(sizeof(*newList));
    if(!newList){
        return NULL;
    }
    newList->head = NULL;
    newList->size = 0;
    return newList;
}

void listDestroy(List list){
    if(list == NULL){
        return;
    }
    while(list->head != NULL){
        Node willDelete = list->head;
        list->head = list->head->next;
        nodeDestroy(willDelete);
    }
    free(list);
}

int listSize(List list){
    if(list == NULL){
        return -1;
    }
    return list->size;
}

ListResult listInsert(List list, Element element, Type type){
    if(list == NULL || element == NULL){
        return LIST_NULL_ARGUMENT;
    }
    Node prev_first = list->head;
    Node new_node = nodeCreate(element,type);
    list->head =  new_node;
    new_node->next = prev_first;
    list->size++;
    return LIST_SUCCESS;
}

ListResult listRemove(List list, Element element){
    if(!list || !element){
        return LIST_NULL_ARGUMENT;
    }
    Node tmp = list->head;
    Node last_tmp = tmp;
    while(tmp != NULL){
        if (tmp->element == element) {
            Node newNode = tmp->next;
            nodeDestroy(tmp);
            if(tmp == list->head){
                list->head = newNode;
            }
            else{
                last_tmp->next = newNode;
            }
            list->size--;
            return LIST_SUCCESS;
        }
        last_tmp = tmp;
        tmp = tmp->next;
    }
    return LIST_ITEM_DOES_NOT_EXIST;
}

Element listGetFirst(List list){
    if(list == NULL || list->size == 0){
        return NULL;
    }
    list->iterator = list->head;
    return list->iterator->element;
}

Type listGetIteratorType(List list){
    if(list == NULL || list->size == 0){
        return UNDEF_TYPE;
    }
    list->iterator = list->head;
    return list->iterator->type;
}

Element listGetLast(List list){
    return NULL;
}

Element listGetLastType(List list) {
    return UNDEF_TYPE;
}

Element listGetNext(List list) {
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