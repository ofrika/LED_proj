#include "mixedList.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct node_t *Node;
struct node_t{
    Object_Type type;
    Element element;
    Node next;
};

struct mixedList_t{
    Node head;
    int size;
    Node iterator;
};

Node mixedNodeCreate(Element element, Object_Type type){
    Node newNode = malloc(sizeof(*newNode));
    if(!newNode){
        return NULL;
    }
    if(type == Text){
        newNode->element = copyTextObject(element);
    } else { // type == picture
        newNode->element = copyPicObject(element);
    }
    newNode->type = type;
    newNode->next = NULL;
    return newNode;
}

void mixedNodeDestroy(Node node){
    if(node->type == Text){
        destroyTextObject(node->element);
    } else {
        destroyPicObject(node->element);
    }
    free(node);
}

MixedList mixedListCreate(){
    MixedList newList = malloc(sizeof(*newList));
    if(!newList){
        return NULL;
    }
    newList->head = NULL;
    newList->size = 0;
    return newList;
}

void mixedListDestroy(MixedList list){
    if(list == NULL){
        return;
    }
    while(list->head != NULL){
        Node willDelete = list->head;
        list->head = list->head->next;
        mixedNodeDestroy(willDelete);
    }
    free(list);
}

int mixedListSize(MixedList list){
    if(list == NULL){
        return -1;
    }
    return list->size;
}
int getX(Element element, Object_Type type){
    if(type == Text){
        TextObject textObject = element;
        return getTextX(textObject);
    } else {
        PicObject picObject = element;
        return getPicX(picObject);
    }
}

int getY(Element element, Object_Type type){
    if(type == Text){
        TextObject textObject = element;
        return getTextY(textObject);
    } else {
        PicObject picObject = element;
        return getPicY(picObject);
    }
}

int getLenX(Element element, Object_Type type){
    if(type == Text){
        TextObject textObject = element;
        return getTextLenX(textObject);
    } else {
        PicObject picObject = element;
        return getPicLenY(picObject);
    }
}

int getLenY(Element element, Object_Type type){
    if(type == Text){
        TextObject textObject = element;
        return getTextLenY(textObject);
    } else {
        PicObject picObject = element;
        return getPicLenY(picObject);
    }
}

bool inside_in(Element e1, Object_Type type1, Element e2, Object_Type type2) {
    int x1 = getX(e1,type1);
    int y1 = getY(e1,type1);
    int lenX1 = getLenX(e1,type1);
    int lenY1 = getLenY(e1,type1);
    int x2 = getX(e2,type2);
    int y2 = getY(e2,type2);
    int lenX2 = getLenX(e2,type2);
    int lenY2 = getLenY(e2,type2);

    if((x2>=x1 && x2<=x1+lenX1 &&  y2>=y1 && y2<=y1+lenY1 ) ||
            (x2>=x1 && x2<=x1+lenX1 &&  y2+lenY2>=y1 && y2+lenY2<=y1+lenY1 ) ||
                (x2+lenX2>=x1 && x2+lenX2<=x1+lenX1 && y2>=y1 && y2<=y1+lenY1 ) ||
                    (x2+lenX2>=x1 && x2+lenX2<=x1+lenX1 && y2+lenY2>=y1 && y2+lenY2<=y1+lenY1 )){
        return true;
    }
    return false;
}

bool intersection(Element e1, Object_Type type1, Element e2, Object_Type type2){
    if(inside_in(e1, type1, e2, type2) || inside_in(e2, type2, e1, type1)){
        return true;
    }
    return false;
}

bool locationIsOccupied(Element e1, Object_Type type1, Element e2, Object_Type type2){
    if(intersection(e1, type1, e2, type2)){
        return true;
    }
    return false;
}

bool locationIsAfter(Element e1, Object_Type type1, Element e2, Object_Type type2){
    int x1 = getX(e1,type1);
    int y1 = getY(e1,type1);
    int x2 = getX(e2,type2);
    int y2 = getY(e2,type2);

    if(y2>y1){
        return true;
    } else if(y2==y1 && x2>x1){
        return true;
    }
    return false;
}
MixedListResult mixedListInsert(MixedList list, Element element, Object_Type type){
    if(list == NULL || element == NULL){
        return MIXED_LIST_NULL_ARGUMENT;
    }
    if(list->size != 0) {
        Node tmp = list->head;
        Node last_tmp = tmp;
        while (tmp != NULL) {
            if (locationIsOccupied(tmp->element, tmp->type, element, type) == true) {
                return MIXED_LIST_ANOTHER_ITEM_ALREADY_LOCATED_THERE;
            }
            if (locationIsAfter(tmp->element, tmp->type, element, type)) {
                last_tmp = tmp;
                tmp = tmp->next;
            }
            else{
                Node newNode = mixedNodeCreate(element, type);
                if(tmp == list->head){
                    newNode->next = list->head;
                    list->head = newNode;
                }
                else{
                    newNode->next = tmp;
                    last_tmp->next = newNode;
                }
                list->size++;
                return MIXED_LIST_SUCCESS;
            }
        }
        Node newNode = mixedNodeCreate(element, type);
        last_tmp->next = newNode;
        list->size++;
        return MIXED_LIST_SUCCESS;

    }
    else{
        list->head = mixedNodeCreate(element, type); // what if the allocation failed?
        list->size++;
        return MIXED_LIST_SUCCESS;
    }

}

bool objectsAreEqual(Element element, Object_Type type, int id){
    if(type == Text){
        TextObject newText = element;
        if(getTextID(newText) == id){
            return true;
        }
        return false;
    } else {
        PicObject newPic = element;
        if(getPicID(newPic) == id){
            return true;
        }
        return false;
    }
}

MixedListResult mixedListRemove(MixedList list, int id){
    if(!list){
        return MIXED_LIST_NULL_ARGUMENT;
    }

    Node tmp = list->head;
    Node last_tmp = tmp;
    while(tmp != NULL){
        if (objectsAreEqual(tmp->element, tmp->type, id)) {
            Node newNode = tmp->next;
            mixedNodeDestroy(tmp);
            if(tmp == list->head){
                list->head = newNode;
            }
            else{
                last_tmp->next = newNode;
            }
            list->size--;
            return MIXED_LIST_SUCCESS;
        }
        last_tmp = tmp;
        tmp = tmp->next;
    }
    return MIXED_LIST_ITEM_DOES_NOT_EXIST;
}

int mixedListGetFirstID(MixedList list){
    if(list == NULL || list->size == 0){
        return -1;
    }
    list->iterator = list->head;
    if(list->iterator->type == Text){
        TextObject textObject = list->iterator->element;
        return getTextID(textObject);
    } else {
        PicObject picObject = list->iterator->element;
        return getPicID(picObject);
    }
}

int mixedListGetLastID(MixedList list){
    if(list == NULL || list->size == 0){
        return -1;
    }
    list->iterator = list->head;
    if(list->size == 1){
        if(list->iterator->type == Text){
            TextObject textObject = list->iterator->element;
            return getTextID(textObject);
        } else {
            PicObject picObject = list->iterator->element;
            return getPicID(picObject);
        }
    }
    while(list->iterator->next != NULL){
        list->iterator = list->iterator->next;
    }
    if(list->iterator->type == Text){
        TextObject textObject = list->iterator->element;
        return getTextID(textObject);
    } else {
        PicObject picObject = list->iterator->element;
        return getPicID(picObject);
    }
}

int mixedListGetNextID(MixedList list) {
    if (!list || list->size == 0) {
        return -1;
    }

    if (list->iterator != NULL) {
        list->iterator = list->iterator->next;
        if(list->iterator != NULL){
            if(list->iterator->type == Text){
                TextObject textObject = list->iterator->element;
                return getTextID(textObject);
            } else {
                PicObject picObject = list->iterator->element;
                return getPicID(picObject);
            }
        }
        return -1;
    }
    return -1;
}

void mixedListClear(MixedList list){
    if(!list){
        return;
    }

    while (list->head) {
        Node ptr=list->head;
        list->head=ptr->next;
        mixedNodeDestroy(ptr);
    }
    list->size = 0;

    return;
}