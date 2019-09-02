#include "list.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct node_t *Node;
struct node_t{
    Element element;
    Node next;
};

struct list_t{
    Node head;
    int size;
    Node iterator;
};

Node nodeCreate(Element element){
    Node newNode = malloc(sizeof(*newNode));
    if(!newNode){
        return NULL;
    }
    newNode->element = element;
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

/*
int getX(Element element, Type type){
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
 */

ListResult listInsert(List list, Element element){
    if(list == NULL || element == NULL){
        return LIST_NULL_ARGUMENT;
    }
    Node prev_first = list->head;
    list->head = element;
    list->head->next = prev_first;
    list->size++;
    return LIST_SUCCESS;


}
/*
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
*/

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

Element listGetLast(List list){
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

void listClear(List list){
    if(!list){
        return;
    }

    while (list->head) {
        Node ptr=list->head;
        list->head=ptr->next;
        nodeDestroy(ptr);
    }
    list->size = 0;
    return;
}