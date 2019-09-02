// Created by Anabusy on 8/29/2019.
#include "Display.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
typedef unsigned char byte;
typedef enum type_e{Text , Picture, SubBoard} Type;
typedef enum existing_Pics_e{U, D, L, R, FLOWER, SMILEY, UNDEFINED} Existing_Pics;

/* *************************** RGB Declerations **************************** */
struct rgb_t{
    byte r;
    byte g;
    byte b;
};

RGB createRGB(byte r, byte g, byte b){
    RGB newRGB = malloc(sizeof(*newRGB));
    if(!newRGB){
        return NULL;
    }
    newRGB->r = r;
    newRGB->g = g;
    newRGB->b = b;
    return newRGB;
}

RGB copyRGB(RGB rgb){
    if(!rgb){
        return NULL;
    }
    RGB newRGB = createRGB(rgb->r, rgb->g, rgb->b);
    if(!newRGB){
        return NULL;
    }
    return newRGB;
}

void destroyRGB(RGB rgb){
    free(rgb);
}

/* *************************** TextObject Declerations **************************** */
typedef struct textObject_t *TextObject;

TextObject createTextObject(int id, int x, int y, int lenX, int lenY, RGB color, bool scrollable, int size, char* data);
TextObject copyTextObject(TextObject textObject);
void destroyTextObject(TextObject textObject);
int getTextID(TextObject textObject);
int getTextX(TextObject textObject);
int getTextY(TextObject textObject);
int getTextLenX(TextObject textObject);
int getTextLenY(TextObject textObject);

struct textObject_t{
    int id;
    int x;
    int y;
    int lenX;
    int lenY;
    RGB color;
    bool scrollable;
    int size;
    char* data;
};

TextObject createTextObject(int id, int x, int y, int lenX, int lenY, RGB color, bool scrollable, int size, char* data){
    TextObject newText = malloc(sizeof(*newText));
    if(!newText){
        return NULL;
    }
    newText->id = id;
    newText->x = x;
    newText->y = y;
    newText->lenX = lenX;
    newText->lenY = lenY;
    newText->color = copyRGB(color);
    newText->scrollable = scrollable;
    newText->size = size;
    newText->data = malloc(sizeof(char)*(strlen(data)+1));
    if(!newText->data){
        free(newText);
        return NULL;
    }
    strcpy(newText->data, data);
    return newText;
}

TextObject copyTextObject(TextObject textObject){
    TextObject newText = createTextObject(textObject->id, textObject->x, textObject->y, textObject->lenX, textObject->lenY, textObject->color, textObject->scrollable, textObject->size, textObject->data);
    if(!newText){
        return NULL;
    }
    return newText;
}

void destroyTextObject(TextObject textObject){
    destroyRGB(textObject->color);
    free(textObject->data);
    free(textObject);
}

int getTextID(TextObject textObject){
    if(!textObject){
        return -1;
    }
    return textObject->id;
}

int getTextX(TextObject textObject){
    if(!textObject){
        return -1;
    }
    return textObject->x;
}

int getTextY(TextObject textObject){
    if(!textObject){
        return -1;
    }
    return textObject->y;
}

int getTextLenX(TextObject textObject){
    if(!textObject){
        return -1;
    }
    return textObject->lenX;
}

int getTextLenY(TextObject textObject){
    if(!textObject){
        return -1;
    }
    return textObject->lenY;
}



/* *************************** PictureObject Declerations**************************** */
typedef struct picObject_t *PicObject;

PicObject createPicObject(int id, int x, int y, int lenX, int lenY, RGB color, Picture_Type type, char* data);
PicObject copyPicObject(PicObject picObject);
void destroyPicObject(PicObject picObject);
int getPicID(PicObject picObject);
int getPicX(PicObject picObject);
int getPicY(PicObject picObject);
int getPicLenX(PicObject picObject);
int getPicLenY(PicObject picObject);

struct picObject_t{
    int id;
    int x;
    int y;
    int lenX;
    int lenY;
    RGB color;
    Picture_Type type;
    void* data;         // If type = existing then interpret data as enum value
    // Otherwise, interpret as 2D array of RGB
};

Existing_Pics convertPicStrToEnumExisting(char* data){
    if(strcmp(data, "up") == 0){
        return U;
    } else if(strcmp(data, "down") == 0){
        return D;
    } else if(strcmp(data, "left") == 0){
        return L;
    } else if(strcmp(data, "right") == 0){
        return R;
    } else if(strcmp(data, "smiley") == 0){
        return SMILEY;
    } else if(strcmp(data, "flower") == 0){
        return FLOWER;
    } else {
        return UNDEFINED;
    }
}

RGB** convertPicStrToRGBArray(char* data, int width, int height){
    RGB** arr = malloc(sizeof(*arr)*height);
    for(int i=0; i<height; i++){
        arr[i] = malloc(sizeof(**arr)*width);
    }
    char *ptr = strtok(data, ",;");
    int count = 0;
    byte r=0, g=0, b=0;
    for(int i=0; ptr != NULL && i<height; i++){
        for(int j=0; ptr != NULL && j<width; j++){
            byte x = atoi(ptr);
            if(count%3 == 0){
                r = x;
                printf("r == '%d'\n", r);
            } else if(count%3 == 1){
                g = x;
                printf("g == '%d'\n", g);
            } else if(count%3 == 2){
                b = x;
                printf("b == '%d'\n", b);
            } else {
                return NULL;
            }
            ptr = strtok(NULL, ",;");
            if(count == 2) {         // then we can construct an RGB item
                RGB newRGB = createRGB(r, g, b);
                arr[i][j] = newRGB;
            }
            count++;
        }
    }
    return arr;
}

PicObject createPicObject(int id, int x, int y, int lenX, int lenY, RGB color, Picture_Type type, char* data){
    PicObject newPic = malloc(sizeof(*newPic));
    if(!newPic){
        return NULL;
    }
    newPic->id = id;
    newPic->x = x;
    newPic->y = y;
    newPic->lenX = lenX;
    newPic->lenY = lenY;
    if(type == EXISTING){
        newPic->color = copyRGB(color);
        if(!newPic->color){
            free(newPic);
            return NULL;
        }
    }
    newPic->type = type;
    if(type == EXISTING){
        newPic->data = (void*)convertPicStrToEnumExisting(data);
    } else {                //type = NEW
        newPic->data = (void*)convertPicStrToRGBArray(data, lenX, lenY);
    }
}

PicObject copyPicObject(PicObject picObject){
    PicObject newPic = createPicObject(picObject->id, picObject->x, picObject->y, picObject->lenX, picObject->lenY, picObject->color,picObject->type, picObject->data);
    if(!newPic){
        return NULL;
    }
    return newPic;
}

void destroyPicObject(PicObject picObject){
    if(!picObject){
        return;
    }
    destroyRGB(picObject->color);
    if(picObject->type == NEW){
        RGB** arr = picObject->data;
        for (int i = 0; i < picObject->lenY ; ++i) {
            for (int j = 0; j < picObject->lenX; ++j) {
                destroyRGB(arr[i][j]);
            }
            free(arr[i]);
        }
        free(arr);
    }
    free(picObject);
}

int getPicID(PicObject picObject){
    if(!picObject){
        return -1;
    }
    return picObject->id;
}

int getPicX(PicObject picObject){
    if(!picObject){
        return -1;
    }
    return picObject->x;
}

int getPicY(PicObject picObject){
    if(!picObject){
        return -1;
    }
    return picObject->y;
}

int getPicLenX(PicObject picObject){
    if(!picObject){
        return -1;
    }
    return picObject->lenX;
}

int getPicLenY(PicObject picObject){
    if(!picObject){
        return -1;
    }
    return picObject->lenY;
}

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
    List subBoards;
};

// ********************************* Display ***********************************
struct display_t{
    int id;
    int x;
    int y;
    int lenX;
    int lenY;
    List objects;
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
    new_disp->objects = listCreate();
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
    listDestroy(disp->objects);
    free(disp);
}

bool is_disp_ID(Display disp, int ID){
    if(disp->id == ID){
        return true;
    }
    return false;
}

void clearDisplayObjects(Display disp){
    listClear(disp->objects);
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
    mainBoard->subBoards = listCreate();
    return LED_SIGN_SUCCESS;
}

void destroyBoard(){
    free(mainBoard->MatsPerLine);
    listDestroy(mainBoard->subBoards);
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
    if(listContains(mainBoard->subBoards, disp, disp_is_equal)){
        destroyDisplay(disp);
        return LED_SIGN_DISPLAY_ID_ALREADY_EXIST;
    }
    if(!is_legal_location(x,y,lenX,lenY)){
        destroyDisplay(disp);
        return LED_SIGN_OUT_OF_BOARD_COARDINATES;
    }
    if(uniqueOrderedListInsert(mainBoard->subBoards, disp, disps_intersect, disp_is_greater) == UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS){
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
    Display lastDisp = listGetFirst(mainBoard->subBoards);
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