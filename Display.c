// Created by Anabusy on 8/29/2019.
#include "Display.h"
#include "mixedList.h"
#include "uniqueOrderedList.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

static int static_id = 1;
// ******************* Basic data structures *******************

typedef struct screen_t
{
    char** R;
    char** G;
    char** B;
    int lenRow; // Default: 4*32
    int lenCol; // Default: 8*32
}*Screen;

typedef struct board_t
{
    char* ip_add;
    int numPorts; // Default: 4
    int* MatsPerLine;
    int numLedsInMatrix; // Default: 32
    UniqueOrderedList subBoards;
}*Board;

typedef struct display_t
{
    int id;
    int x;
    int y;
    int lenX;
    int lenY;
    MixedList objects;
}*Display;

/* ***************************** Implementations ******************************* */

// ******************* Global variables *******************

unsigned char* port1 = (unsigned char *) 0x40000000;
unsigned char* port2 = (unsigned char *) 0x42000000;
unsigned char* port3 = (unsigned char *) 0x44000000;
unsigned char* port4 = (unsigned char *) 0x46000000;

Board mainBoard;
Screen oldScreen;
Screen newScreen;

// ********************************************************
Display createDisplay(int x, int y, int lenX, int lenY){
    Display new_disp = malloc(sizeof(*new_disp));
    if(!new_disp){
        return NULL;
    }
    new_disp->id = static_id;
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

Display copyDisplay(Display disp){
    if(!disp){
        return NULL;
    }
    Display new_disp = createDisplay(disp->x, disp->y, disp->lenX, disp->lenY);
    if(!new_disp){
        return NULL;
    }
    new_disp->id = disp->id;
    return new_disp;
}

void destroyDisplay(Display disp){
    if(!disp){
        return;
    }
    mixedListDestroy(disp->objects);
    free(disp);
    return;
}

// I should recheck this function
bool is_in(Display disp1, Display disp2){
    if((disp1->x >= disp2->x) && (disp1->x <= disp2->x + disp2->lenX) &&
       (disp1->y >= disp2->y) && (disp1->y <= disp2->y + disp2->lenY)) {
        return true;
    }
    return false;
}

bool disp_is_equal(Display disp1, Display disp2){
    if(is_in(disp1, disp2) || is_in(disp2, disp2)){
        return true;
    } else {
        return false;
    }
}

bool disp_is_greater(Display disp1, Display disp2){
    if(disp1->y > disp2->y){
        return true;
    } else if(disp1->y==disp2->y && disp1->x > disp2->x){
        return true;
    }
    else {
        return false;
    }
}

int initBoard(char* ip_add, int numPorts, int numLedsInMatrix, int num_port1, int num_port2, int num_port3, int num_port4)
{
    mainBoard = malloc(sizeof(*mainBoard));
    if(!mainBoard){
        return -1;
    }
    mainBoard->ip_add = ip_add;
    mainBoard->numPorts = numPorts; // Default: 4
    mainBoard->numLedsInMatrix = numLedsInMatrix; // Default: 32
    mainBoard->MatsPerLine = malloc(numPorts*sizeof(int));
    if(!(mainBoard->MatsPerLine)){
        free(mainBoard);
        return -1;
    }
    //for(int i=0; i<numPorts; i++){
    mainBoard->MatsPerLine[0] = num_port1;
    mainBoard->MatsPerLine[1] = num_port2;
    mainBoard->MatsPerLine[2] = num_port3;
    mainBoard->MatsPerLine[3] = num_port4;
    //}
    mainBoard->subBoards = uniqueOrderedListCreate(copyDisplay, destroyDisplay, disp_is_equal, disp_is_greater);
    return 1;
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

int addDisplay(int x, int y, int lenX, int lenY){ //indeces starts from 1
    if(!is_legal_location(x,y,lenX,lenY)){
        return -2;		// return "ILLEGAL LOCATION"
    }
    Display disp = createDisplay(x,y,lenX,lenY);
    if(!disp){
        return -1;		// return :PROBLEM ALLOCATING DISP"
    }
    if(uniqueOrderedListContains(mainBoard->subBoards, disp)){
        return -2;		//return "ILLEGAL LOCATION"
    }
    uniqueOrderedListInsert(mainBoard->subBoards, disp);
    destroyDisplay(disp);
    return 0;

}

int addObject(Object_Type o, int id, int x, int y, int lenX, int lenY, bool scrollable, int size){

}