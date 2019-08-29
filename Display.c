// Created by Anabusy on 8/29/2019.
#include "Display.h"
#include "mixedList.h"
#include "uniqueOrderedList.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


// ******************* Global variables *******************

unsigned char* port1 = (unsigned char *) 0x40000000;
unsigned char* port2 = (unsigned char *) 0x42000000;
unsigned char* port3 = (unsigned char *) 0x44000000;
unsigned char* port4 = (unsigned char *) 0x46000000;

typedef struct screen_t *Screen;
typedef struct board_t *Board;
typedef struct display_t *Display;
Board mainBoard;
Screen oldScreen;
Screen newScreen;

// *************************** Data structures ********************************


// *************************** Screen ********************************
struct screen_t{
    char** R;
    char** G;
    char** B;
    int lenRow; // Default: 4*32
    int lenCol; // Default: 8*32
};

// ********************************** Board ************************************
struct board_t
{
    char* ip_add;
    int numPorts; // Default: 4
    int* MatsPerLine;
    int numLedsInMatrix; // Default: 32
    UniqueOrderedList subBoards;
};

// ********************************* Display ***********************************
struct display_t{
    int id;
    int x;
    int y;
    int lenX;
    int lenY;
    MixedList objects;
};

Display createDisplay(int id, int x, int y, int lenX, int lenY){
    Display new_disp = malloc(sizeof(*new_disp));
    if(!new_disp){
        return NULL;
    }
    new_disp->id = id;
    new_disp->x = x;
    new_disp->y = y;
    new_disp->lenX = lenX;
    new_disp->lenY = lenY;
    new_disp->objects = mixedListCreate();
    if(!(new_disp->objects)){
        free(new_disp);
        return NULL;
    }
    return new_disp;
}

Element copyDisplay(Element d){
    Display disp = d;
    if(!disp){
        return NULL;
    }
    Display new_disp = createDisplay(disp->id, disp->x, disp->y, disp->lenX, disp->lenY);
    if(!new_disp){
        return NULL;
    }
    new_disp->id = disp->id;
    return new_disp;
}

void destroyDisplay(Element d){
    Display disp = d;
    if(!disp){
        return;
    }
    mixedListDestroy(disp->objects);
    free(disp);
}

bool is_disp_ID(Display disp, int ID){
    if(disp->id == ID){
        return true;
    }
    return false;
}

void clearDisplayObjects(Display disp){
    uniqueOrderedListClear(disp->objects);
}

// I should recheck this function
bool is_in(Display disp1, Display disp2){
    if((disp1->x >= disp2->x) && (disp1->x <= disp2->x + disp2->lenX) &&
       (disp1->y >= disp2->y) && (disp1->y <= disp2->y + disp2->lenY)) {
        return true;
    }
    return false;
}

bool disps_intersect(Element d1, Element d2){
    Display disp1 = d1;
    Display disp2 = d2;
    if(is_in(disp1, disp2) || is_in(disp2, disp2)){
        return true;
    } else {
        return false;
    }
}

bool disp_is_greater(Element d1, Element d2){
    Display disp1 = d1;
    Display disp2 = d2;
    if(disp1->y > disp2->y){
        return true;
    } else if(disp1->y==disp2->y && disp1->x > disp2->x){
        return true;
    }
    else {
        return false;
    }
}

LedSignResult initBoard(char* ip_add, int numPorts, int numLedsInMatrix, int num_port1, int num_port2, int num_port3, int num_port4)
{
    mainBoard = malloc(sizeof(*mainBoard));
    if(!mainBoard){
        return LED_SIGN_OUT_OF_MEMORY;
    }
    mainBoard->ip_add = ip_add;
    mainBoard->numPorts = numPorts; // Default: 4
    mainBoard->numLedsInMatrix = numLedsInMatrix; // Default: 32
    mainBoard->MatsPerLine = malloc(numPorts*sizeof(int));
    if(!(mainBoard->MatsPerLine)){
        free(mainBoard);
        return LED_SIGN_OUT_OF_MEMORY;
    }
    //for(int i=0; i<numPorts; i++){
    mainBoard->MatsPerLine[0] = num_port1;
    mainBoard->MatsPerLine[1] = num_port2;
    mainBoard->MatsPerLine[2] = num_port3;
    mainBoard->MatsPerLine[3] = num_port4;
    //}
    mainBoard->subBoards = uniqueOrderedListCreate(copyDisplay, destroyDisplay, disps_intersect, disp_is_greater);
    return LED_SIGN_SUCCESS;
}

void destroyBoard(){
    free(mainBoard->MatsPerLine);
    uniqueOrderedListDestroy(mainBoard->subBoards);
    free(mainBoard);
}

/* ************************************************************ */
bool is_legal_location(int x, int y, int lenX, int lenY){
    bool y_is_legal = false;
    for(int i=0; i<mainBoard->numPorts; i++){
        if(y>i*mainBoard->numLedsInMatrix && y<=(i+1)*mainBoard->numLedsInMatrix){
            if(y+lenY > 0 && y+lenY > (mainBoard->numPorts)*(mainBoard->numLedsInMatrix)){
                return false;
            }
            y_is_legal = true;
            if(!(x>0 && x<=(mainBoard->MatsPerLine[i])*(mainBoard->numLedsInMatrix)
                 && x+lenX>0 && x+lenX<=(mainBoard->MatsPerLine[i])*(mainBoard->numLedsInMatrix))){
                return false;
            }
        }
    }
    if(!y_is_legal){
        return false;
    }
    return true;
}

bool disp_is_equal(Element d1, Element d2){
    Display disp1 = d1;
    Display disp2 = d2;
    if(disp1->id == disp2->id){
        return true;
    }
    return false;
}


LedSignResult addDisplay(int dispID, int x, int y, int lenX, int lenY){ //indeces starts from 1
    Display disp = createDisplay(dispID, x,y,lenX,lenY);
    if(!disp){
        return LED_SIGN_OUT_OF_MEMORY;
    }
    if(uniqueOrderedListContains(mainBoard->subBoards, disp, disp_is_equal)){
        destroyDisplay(disp);
        return LED_SIGN_DISPLAY_ID_ALREADY_EXIST;
    }
    if(!is_legal_location(x,y,lenX,lenY)){
        destroyDisplay(disp);
        return LED_SIGN_OUT_OF_BOARD_COARDINATES;
    }
    if(uniqueOrderedListInsert(mainBoard->subBoards, disp) == UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS){
        destroyDisplay(disp);
        return LED_SIGN_ANOTHER_DISPLAY_LOCATED_THERE;
    }
    destroyDisplay(disp);
    return 0;

}

/* ************************************************************ */
LedSignResult cleanDisplay(int dispID){
    Display lastDisp = uniqueOrderedListGetGreatest(mainBoard->subBoards);
    if(!lastDisp){
        return LED_SIGN_SUCCESS;
    }
    for(Display itr = uniqueOrderedListGetLowest(mainBoard->subBoards);  ; uniqueOrderedListGetNext(mainBoard->subBoards)){
        if(is_disp_ID(itr, dispID)){
            clearDisplayObjects(itr);
            return LED_SIGN_SUCCESS;
        }
        if(itr == lastDisp){
            break;
        }
    }
    return LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID;
}

LedSignResult deleteDisplay(int dispID){
    Display lastDisp = uniqueOrderedListGetGreatest(mainBoard->subBoards);
    if(!lastDisp){
        return LED_SIGN_SUCCESS;
    }
    for(Display itr = uniqueOrderedListGetLowest(mainBoard->subBoards);  ; uniqueOrderedListGetNext(mainBoard->subBoards)){
        if(is_disp_ID(itr, dispID)){
            uniqueOrderedListRemove(mainBoard->subBoards, itr, disp_is_equal);
            return LED_SIGN_SUCCESS;
        }
        if(itr == lastDisp){
            break;
        }
    }
    return LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID;
}