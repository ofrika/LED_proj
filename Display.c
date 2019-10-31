// Created by Anabusy on 8/29/2019.
#include "Display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xil_io.h"
#include "Image.h"
#include "list.h"
#include "PicObject.h"
#include "TextObject.h"
#include "RGB.h"
#include "Chars.h"
#include <sleep.h>

#define N 32

#define MIN(a,b) (((a)<(b))?(a):(b))

// ******************* Global variables *******************

typedef struct display_t *Display;
typedef struct board_t *Board;
typedef struct buffer_t *Buffer;

Board mainBoard;

int *copy_cntl = (int *) 0x43C00000;

unsigned char* port1 = (unsigned char *) 0x40000000;
unsigned char* port2 = (unsigned char *) 0x42000000;
unsigned char* port3 = (unsigned char *) 0x44000000;
unsigned char* port4 = (unsigned char *) 0x46000000;

int board_rgb[4*N][8*N];

// *************************** Data structures ********************************

struct display_t{
    int id;
    int x;
    int y;
    int lenX;
    int lenY;
    List objects;
};

struct board_t
{
    int numPorts; // Default: 4
    int* MatsPerLine;
    List subBoards;
    List imagesStock;
    Direction** matrixDirections;
};

struct buffer_t{
    byte** R;
    byte** G;
    byte** B;
};

// ************************** Display handling methods declarations *******************************

Display createDisplay(int id, int x, int y, int lenX, int lenY);
void destroyDisplay(Display d);

// ************************* Object handling methods implementations ******************************

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

void destroyDisplay(Display disp){
    if(!disp){
        return;
    }
    listDestroy(disp->objects);
    free(disp);
}

// ************************** Message parsing methods declarations *******************************

Direction convertDirStrToEnumDir(char* data);
Direction** parseDirections(char* directionsStr, int* ports, int numPorts);
RGB* convertPicStrToRGBArray(int width, int height, char* data);

// ************************** Message parsing methods implementation *******************************

Direction convertDirStrToEnumDir(char* data){
    if(strcmp(data, "U") == 0){
        return UP;
    } else if(strcmp(data, "D") == 0){
        return DOWN;
    } else if(strcmp(data, "L") == 0){
        return LEFT;
    } else if(strcmp(data, "R") == 0){
        return RIGHT;
    }
    return UNDEFINED_DIR;
}

Direction** parseDirections(char* directionsStr, int* ports, int numPorts){
    Direction** arr = malloc(sizeof(*arr)*numPorts);
    for(int i=0; i<numPorts; i++){
        arr[i] = malloc(sizeof(**arr)*ports[i]);
    }
    for(int i=0; i<numPorts; i++){
        for(int j=0; j<ports[i]; j++){
            *(*(arr+i)+j) = RIGHT;
        }
    }
    if(strlen(directionsStr) == 0) {
        return arr;
    }
    char* tmp = malloc(strlen(directionsStr)+1);
    strcpy(tmp,directionsStr);
    char *ptr = strtok(tmp, ",;");
    int count = 0;
    int row=0, column=0;
    Direction dir = RIGHT;
    while(ptr != NULL){
        if(count%3 == 0){
            int x = atoi(ptr);
            row = x;
        } else if(count%3 == 1){
            int x = atoi(ptr);
            column = x;
        } else if(count%3 == 2){
            Direction x = convertDirStrToEnumDir(ptr);
            if(x == UNDEFINED_DIR){
                for(int i=0; i<numPorts; i++){
                    free(arr[i]);
                }
                free(arr);
                return NULL;
            }
            dir = x;
        } else {
            return NULL;
        }
        ptr = strtok(NULL, ",;");
        if(count%3 == 2) {         // then we can change direction
        	xil_printf("OLALA \n");
            arr[row][column] = dir;
        }
        count++;
    }
    free(tmp);

    for(int i=0; i<numPorts; i++){
        for(int j=0; j<ports[i]; j++){
            xil_printf("i= %d, j= %d : %d\n", i,j,*(*(arr+i)+j));
        }
    }
    return arr;
}

RGB* convertPicStrToRGBArray(int width, int height, char* data){
    RGB* arr = malloc(sizeof(*arr)*height*width);
    char* tmp = malloc(strlen(data)+1);
    strcpy(tmp,data);
    char *ptr = strtok(tmp, ",;");
    int count = 0;
    byte r=0, g=0, b=0;
    int i=0;
    while (ptr != NULL){
        byte x = atoi(ptr);
        if(count%3 == 0){
            r = x;
//            printf("r == '%d'\n", r);
        } else if(count%3 == 1){
            g = x;
//            printf("g == '%d'\n", g);
        } else if(count%3 == 2){
            b = x;
//            printf("b == '%d'\n", b);
            RGB newRGB = createRGB(r, g, b);
            arr[i/3] = newRGB;
        } else {
            return NULL;
        }
        ptr = strtok(NULL, ",;");
        i++;
        if(i==3*height*width){
            break;
        }

        count++;
    }
    free(tmp);
    return arr;

    for(int q=0; q<width*height; q++){
        xil_printf("rgb arr : %d \n", arr[q]);
    }
}

// ************************** private function *******************************

byte* enlargeImage(int orgLenX, int orgLenY, int finalLenX, int finalLenY, byte rdata[], byte gdata[], byte bdata[]){
    int triSize = 3 * finalLenX * finalLenY;
    byte* finalData = malloc(sizeof(byte)*triSize); // allocate 3 bytes per pixel

    for (int i = 0; i < triSize; ++i) {
        finalData[i] = 0;
    }

    double d_x = (double)finalLenX/orgLenX;
    byte* intermData =  malloc(sizeof(byte)*3*orgLenY*finalLenX);

    // iterate over infile's scanlines
    for (int i = 0; i < orgLenY; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < orgLenX; j++)
        {
            for (int k = j*d_x; k < d_x*(j+1); k++)
            {
                intermData[(finalLenX-k-1)*3+3*i*finalLenX] = *(rdata+i*orgLenX+j);
                intermData[(finalLenX-k-1)*3+3*i*finalLenX+1] = *(gdata+i*orgLenX+j);
                intermData[(finalLenX-k-1)*3+3*i*finalLenX+2] = *(bdata+i*orgLenX+j);
            }
        }
        // now I have read the entire line
    }

    double d_y = (double)finalLenY/orgLenY;

    for (int j = 0; j < finalLenX; j++){
        for (int i = 0; i < orgLenY; i++){
            for (int k = i*d_y; k < d_y*(i+1); k++)
            {
                *(finalData+3*k*finalLenX+3*j) = *(intermData+3*i*finalLenX+3*j);
                *(finalData+3*k*finalLenX+3*j+1) = *(intermData+3*i*finalLenX+3*j+1);
                *(finalData+3*k*finalLenX+3*j+2) = *(intermData+3*i*finalLenX+3*j+2);
            }
        }
    }

    free(intermData);
    return finalData;
}

byte* text2rgb(int* data, int n, int finalLenX, int finalLenY){

    // the input is an array of int, each cell contains the sequence number of the Hebrew Char
    int* ch_w = malloc(sizeof(int)*n);
    if(!ch_w){
        printf("error allocating memory..\n");
    }
    int w = 0;
    for (int i = 0; i < n ; ++i) {
        if (data[i] == 1 || data[i] == 2 || data[i] == 4 || data[i] == 5 || data[i] == 8 || data[i] == 12 || data[i] == 15 || data[i] == 18 || data[i] == 22 || data[i] == 23 || data[i] == 26 || data[i] == 27){
            w+=E;
            ch_w[i] = E;
        } else if(data[i] == 3 || data[i] == 9 || data[i] == 11 || data[i] == 13 || data[i] == 14 || data[i] == 19 || data[i] == 20 || data[i] == 21 || data[i] == 25){
            w+=S;
            ch_w[i] = S;
        } else if(data[i] == 6 || data[i] == 7 || data[i] == 10 || data[i] == 16 || data[i] == 17 || data[i] == 24){
            w+=F;
            ch_w[i] = F;
        }
    }

    byte *rgb_arr = malloc(sizeof(byte)*E*w);
    if(!rgb_arr){
        printf("error allocating memory..\n");
    }

    int offset = 0;
    for (int k = 0; k < n ; ++k) {
        switch(data[k]){
            case 1:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Alef[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 2:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Bet[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 3:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Gimel[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 4:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Dalet[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 5:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = He[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 6:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Vav[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 7:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Zain[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 8:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Het[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 9:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Tet[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 10:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Yod[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 11:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Khaf[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 12:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Khaf_s[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 13:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Lamed[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 14:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Mem[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 15:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Mem_s[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 16:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Non[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 17:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Non_s[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 18:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Samekh[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 19:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Aien[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 20:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Fe[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 21:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Fe_s[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 22:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Tsade[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 23:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Tsade_s[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 24:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Kof[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 25:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Resh[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 26:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Shen[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
            case 27:
                for(int i=0; i < E; i++){
                    for (int j = 0; j < ch_w[k]; ++j) {
                        *(rgb_arr+i*w+j+offset) = Tav[i][j];
                    }
                }
                offset+=ch_w[k];
                break;
        }

    }

    free(ch_w);
    byte* res = enlargeImage(w,E,finalLenX,finalLenY,rgb_arr,rgb_arr,rgb_arr);
    free(rgb_arr);
    return res;
}

bool is_legal_location(int x, int y, int lenX, int lenY){
    bool y_is_legal = false;
    if(y<0 || y+lenY-1 >= (mainBoard->numPorts)*N){
        return false;
    }
    for(int i=0; i<mainBoard->numPorts; i++){
        if(y>=i*N && y<(i+1)*N){
            y_is_legal = true;
            if(!(x>=0 && x<(mainBoard->MatsPerLine[i])*N && x+lenX>0 && x+lenX-1<(mainBoard->MatsPerLine[i])*N)){
                return false;
            }
        }
    }
    if(!y_is_legal){
        return false;
    }
    return true;
}


bool inside_in(int x1, int y1, int lenX1, int lenY1, int x2, int y2, int lenX2, int lenY2) {
    if((x2>=x1 && x2<x1+lenX1 &&  y2>=y1 && y2<y1+lenY1 ) ||
       (x2>=x1 && x2<x1+lenX1 &&  y2+lenY2>y1 && y2+lenY2<=y1+lenY1 ) ||
       (x2+lenX2>x1 && x2+lenX2<=x1+lenX1 && y2>=y1 && y2<y1+lenY1 ) ||
       (x2+lenX2>x1 && x2+lenX2<=x1+lenX1 && y2+lenY2>y1 && y2+lenY2<=y1+lenY1 )){
        return true;
    }
    return false;
}

bool intersection(int x1, int y1, int lenX1, int lenY1, int x2, int y2, int lenX2, int lenY2){
    if(inside_in(x1, y1, lenX1, lenY1, x2, y2, lenX2, lenY2) || inside_in(x2, y2, lenX2, lenY2, x1, y1, lenX1, lenY1)){
        return true;
    }
    return false;
}

bool shapes_intersect(int x1, int y1, int lenX1, int lenY1, int x2, int y2, int lenX2, int lenY2){
    if(intersection(x1, y1, lenX1, lenY1, x2, y2, lenX2, lenY2)){
        return true;
    }
    return false;
}


Display find_containing_Display(int x, int y, int lenX, int lenY){
    for(Display itr_disp = listGetFirst(mainBoard->subBoards); itr_disp != listGetLast(mainBoard->subBoards); itr_disp = listGetNext(mainBoard->subBoards)){
        if(x>=itr_disp->x && x+lenX<=(itr_disp->x)+(itr_disp->lenX) && y>=itr_disp->y && y+lenY<=(itr_disp->y)+(itr_disp->lenY)){
            return itr_disp;
        }
    }
    return NULL;
}

void deleteAllSubBoards(){
    for(Display itr_disp = listGetFirst(mainBoard->subBoards); itr_disp != listGetLast(mainBoard->subBoards) ; itr_disp = listGetFirst(mainBoard->subBoards)){
        for(Element e = listGetFirst(itr_disp->objects); listSize(itr_disp->objects) != 0 ; e = listGetFirst(itr_disp->objects)){
            listRemove(itr_disp->objects,e);
            if(listGetIteratorType(itr_disp->objects) == Text){
                destroyTextObject((TextObject)e);
            } else {
                destroyPicObject((PicObject)e);
            }
        }
        destroyDisplay(itr_disp);
        listRemove(mainBoard->subBoards,itr_disp);
    }
}

// ************************* Called by user functions implementations *****************************

LedSignResult initBoard(int numPorts, int* ports, char* directions)
{
	set_Full_Brightness();

    mainBoard = (Board)malloc(sizeof(*mainBoard));
    if(!mainBoard){
        return LED_SIGN_OUT_OF_MEMORY;
    }
    mainBoard->numPorts = numPorts; // Default: 4
    mainBoard->MatsPerLine = malloc(numPorts*sizeof(int));
    if(!(mainBoard->MatsPerLine)){
        free(mainBoard);
        return LED_SIGN_OUT_OF_MEMORY;
    }
    for(int i=0; i<numPorts; i++){
        mainBoard->MatsPerLine[i] = ports[i];
    }
    mainBoard->matrixDirections = parseDirections(directions, ports, numPorts);
    if(!mainBoard->matrixDirections){
        return LED_SIGN_ERROR_WHILE_PARSING_DIRECTIONS;
    }
    mainBoard->subBoards = listCreate();
    mainBoard->imagesStock = listCreate();

    // I ignored Initializing buffer and handling all related issues
    return LED_SIGN_SUCCESS;
}

void destroyBoard(){
    for (int i = 0; i < mainBoard->numPorts ; ++i) {
        free(mainBoard->matrixDirections[i]);
    }
    free(mainBoard->matrixDirections);
    free(mainBoard->MatsPerLine);
    deleteAllSubBoards();
    listDestroy(mainBoard->subBoards);
    free(mainBoard);
}

LedSignResult addSubBoard(int dispID, int x, int y, int lenX, int lenY){ //indeces starts from 0, as usual

    if(lenX <=0 || lenY <=0 ){
        return LED_SIGN_ILLEGAL_ARGUMENTS;
    }
    Display new_disp = createDisplay(dispID, x,y,lenX,lenY);
    if(!new_disp){
        return LED_SIGN_OUT_OF_MEMORY;
    }
    if(!is_legal_location(x,y,lenX,lenY)){
        destroyDisplay(new_disp);
        return LED_SIGN_OUT_OF_BOARD_COARDINATES;
    }
    for(Display itr_disp = listGetFirst(mainBoard->subBoards); itr_disp != listGetLast(mainBoard->subBoards) ; itr_disp = listGetNext(mainBoard->subBoards)){
        if(itr_disp->id == dispID){
            destroyDisplay(new_disp);
            return LED_SIGN_DISPLAY_ID_ALREADY_EXIST;
        }
        if(shapes_intersect(x,y,lenX,lenY,itr_disp->x, itr_disp->y, itr_disp->lenX, itr_disp->lenY)){
            destroyDisplay(new_disp);
            return LED_SIGN_ANOTHER_DISPLAY_LOCATED_THERE;
        }
    }
    listInsert(mainBoard->subBoards,new_disp,SubBoard);

    return LED_SIGN_SUCCESS;

}

LedSignResult cleanSubBoard(int dispID){
    int num_of_displays = listSize(mainBoard->subBoards);
    if(num_of_displays == 0){
        return LED_SIGN_SUCCESS;
    }
    for(Display itr_disp = listGetFirst(mainBoard->subBoards); itr_disp != listGetLast(mainBoard->subBoards) ; itr_disp = listGetNext(mainBoard->subBoards)){
        if(itr_disp->id == dispID){
            for(Element e = listGetFirst(itr_disp->objects); listSize(itr_disp->objects) != 0 ; e = listGetFirst(itr_disp->objects)){
                listRemove(itr_disp->objects,e);
                if(listGetIteratorType(itr_disp->objects) == Text){
                    destroyTextObject((TextObject)e);
                } else {
                    destroyPicObject((PicObject)e);
                }
            }
            return LED_SIGN_SUCCESS;
        }
    }
    return LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID;
}

LedSignResult deleteSubBoard(int dispID){
    for(Display itr_disp = listGetFirst(mainBoard->subBoards); itr_disp != listGetLast(mainBoard->subBoards) ; itr_disp = listGetNext(mainBoard->subBoards)){
        if(itr_disp->id == dispID){
            for(Element e = listGetFirst(itr_disp->objects); listSize(itr_disp->objects) != 0 ; e = listGetFirst(itr_disp->objects)){
                listRemove(itr_disp->objects,e);
                if(listGetIteratorType(itr_disp->objects) == Text){
                    destroyTextObject((TextObject)e);
                } else {
                    destroyPicObject((PicObject)e);
                }
            }
            destroyDisplay(itr_disp);
            listRemove(mainBoard->subBoards,itr_disp);
            return LED_SIGN_SUCCESS;
        }
    }
    return LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID;
}

LedSignResult addText(int dispID, int textID, int x, int y, int lenX, int lenY, byte r, byte g, byte b, bool scrollable, int* data, int data_len){
    if(lenX <=0 || lenY <=0 ){
        return LED_SIGN_ILLEGAL_ARGUMENTS;
    }
    TextObject textObject = createTextObject(textID,x,y,lenX,lenY,createRGB(r,g,b),scrollable,data,data_len);
    if(!textObject){
        return LED_SIGN_OUT_OF_MEMORY;
    }
    Display disp = NULL;
    for(Display itr_disp = listGetFirst(mainBoard->subBoards); itr_disp != listGetLast(mainBoard->subBoards) ; itr_disp = listGetNext(mainBoard->subBoards)) {
        if (itr_disp->id == dispID) {
            disp = itr_disp;
            if(!(x>=itr_disp->x && x+lenX<=(itr_disp->x)+(itr_disp->lenX) && y>=itr_disp->y && y+lenY<=(itr_disp->y)+(itr_disp->lenY))){
                return LED_SIGN_DISPLAY_CAN_NOT_CONTAIN_THAT_TEXT;
            }
        }
    }
    if(!disp){
        destroyTextObject(textObject);
        return LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID;
    }
    for(Element e = listGetFirst(disp->objects); e != listGetLast(disp->objects); e = listGetNext(disp->objects)){
        if(listGetIteratorType(disp->objects) == Text){
            if(textID == getTextID((TextObject)e)){
                destroyTextObject(textObject);
                return LED_SIGN_OBJECT_ID_ALREADY_EXIST;
            }
            int x2 = getTextX((TextObject)e);
            int y2 = getTextY((TextObject)e);
            int lenX2 = getTextLenX((TextObject)e);
            int lenY2 = getTextLenY((TextObject)e);
            if(shapes_intersect(x,y,lenX,lenY, x2,y2,lenX2,lenY2)){
                destroyTextObject(textObject);
                return LED_SIGN_ANOTHER_OBJECT_LOCATED_THERE;
            }
        } else {
            if(textID == getPicID((PicObject)e)){
                destroyTextObject(textObject);
                return LED_SIGN_OBJECT_ID_ALREADY_EXIST;
            }
            int x2 = getPicX((PicObject)e);
            int y2 = getPicY((PicObject)e);
            int lenX2 = getPicLenX((PicObject)e);
            int lenY2 = getPicLenY((PicObject)e);
            if(shapes_intersect(x,y,lenX,lenY, x2,y2,lenX2,lenY2)){
                destroyTextObject(textObject);
                return LED_SIGN_ANOTHER_OBJECT_LOCATED_THERE;
            }
        }
    }
    listInsert(disp->objects, textObject, Text);
    return LED_SIGN_SUCCESS;
}

Image find_image(int imgId){
    for(Element e = listGetFirst(mainBoard->imagesStock); e != listGetLast(mainBoard->imagesStock); e = listGetNext(mainBoard->imagesStock)){
        if(getImageId((Image)e) == imgId){
            return e;
        }
    }
    return NULL;
}


LedSignResult addImageToStock(int imageID, int height, int width, byte* rData, byte* gData, byte* bData){

    if(width<=0|| height<=0){
        return LED_SIGN_ILLEGAL_ARGUMENTS;
    }

    // we should check that the imageID doesn't exist in the stock yet
    byte* rArr = malloc(sizeof(byte)*height*width);
    byte* gArr = malloc(sizeof(byte)*height*width);
    byte* bArr = malloc(sizeof(byte)*height*width);

    for (int i = 0; i < height*width ; ++i) {
    	rArr[i] = rData[i];
        gArr[i] = gData[i];
        bArr[i] = bData[i];
    }

    Image newImg = createImage(imageID, width, height, rArr, gArr, bArr);
    if(!newImg){
        return LED_SIGN_OUT_OF_MEMORY;
    }

    listInsert(mainBoard->imagesStock,newImg,UNDEF_TYPE);
    return LED_SIGN_SUCCESS;
}


LedSignResult addPicture(int dispID, int pictureID, int imgId, int x, int y, int lenX, int lenY, bool newColor, byte r, byte g, byte b){

    RGB rgb = NULL;
    Image img = find_image(imgId);
    if(!img){
        return LED_SIGN_IMAGE_DOESNT_EXIST_IN_STOCK;
    }
    if(getImageHeight(img)>lenY || getImageWidth(img)>lenX){
        return LED_SIGN_PICTURE_DIMENSIONS_ARE_TOO_MUCH_SMALL;
    }
    if(newColor){
        rgb = createRGB(r,g,b);
    }
    PicObject picObject = createPicObject(pictureID,x,y,lenX,lenY,rgb,img);
    if(!picObject){
        destroyRGB(rgb);
        return LED_SIGN_OUT_OF_MEMORY;
    }
    Display disp = NULL;
    for(Display itr_disp = listGetFirst(mainBoard->subBoards); itr_disp != listGetLast(mainBoard->subBoards) ; itr_disp = listGetNext(mainBoard->subBoards)) {
        if (itr_disp->id == dispID) {
            disp = itr_disp;
            if(!(x>=itr_disp->x && x+lenX<=(itr_disp->x)+(itr_disp->lenX) && y>=itr_disp->y && y+lenY<=(itr_disp->y)+(itr_disp->lenY))){
                return LED_SIGN_DISPLAY_CAN_NOT_CONTAIN_THAT_PICTURE;
            }
        }
    }
    if(!disp){
        destroyPicObject(picObject);
        return LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID;
    }

    for(Element e = listGetFirst(disp->objects); e != listGetLast(disp->objects); e = listGetNext(disp->objects)){
        if(listGetIteratorType(disp->objects) == Text){
            if(pictureID == getTextID((TextObject)e)){
                destroyPicObject(picObject);
                return LED_SIGN_OBJECT_ID_ALREADY_EXIST;
            }
            int x2 = getTextX((TextObject)e);
            int y2 = getTextY((TextObject)e);
            int lenX2 = getTextLenX((TextObject)e);
            int lenY2 = getTextLenY((TextObject)e);
            if(shapes_intersect(x,y,lenX,lenY, x2,y2,lenX2,lenY2)){
                destroyPicObject(picObject);
                return LED_SIGN_ANOTHER_OBJECT_LOCATED_THERE;
            }
        } else {
            if(pictureID == getPicID((PicObject)e)){
                destroyPicObject(picObject);
                return LED_SIGN_OBJECT_ID_ALREADY_EXIST;
            }
            int x2 = getPicX((PicObject)e);
            int y2 = getPicY((PicObject)e);
            int lenX2 = getPicLenX((PicObject)e);
            int lenY2 = getPicLenY((PicObject)e);
            if(shapes_intersect(x,y,lenX,lenY, x2,y2,lenX2,lenY2)){
                destroyPicObject(picObject);
                return LED_SIGN_ANOTHER_OBJECT_LOCATED_THERE;
            }
        }
    }
    listInsert(disp->objects, picObject, Picture);
    return LED_SIGN_SUCCESS;
}

LedSignResult updateText(int dispID, int textID, int* new_data, int data_len){
    for(Display itr_disp = listGetFirst(mainBoard->subBoards); itr_disp != listGetLast(mainBoard->subBoards) ; itr_disp = listGetNext(mainBoard->subBoards)){
        if(itr_disp->id == dispID){
            for(Element e = listGetFirst(itr_disp->objects); e != listGetLast(itr_disp->objects); e = listGetNext(itr_disp->objects)){
                Type type = listGetIteratorType(itr_disp->objects);
                if(type == Text){
                    if(textID == getTextID((TextObject)e)){
                        if(updateTextData((TextObject)e, new_data,data_len) != -1){
                            return LED_SIGN_SUCCESS;
                        }
                        return LED_SIGN_OUT_OF_MEMORY;
                    }
                }
            }
            return LED_SIGN_NO_TEXT_WITH_THE_GIVEN_ID;
        }
    }
    return LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID;
}

LedSignResult updatePicture(int dispID, int pictureID, byte r, byte g, byte b, int newImgId){
    Image newImg = find_image(newImgId);
    if(!newImg){
        return LED_SIGN_IMAGE_DOESNT_EXIST_IN_STOCK;
    }
    for(Display itr_disp = listGetFirst(mainBoard->subBoards); itr_disp != listGetLast(mainBoard->subBoards) ; itr_disp = listGetNext(mainBoard->subBoards)){
        if(itr_disp->id == dispID){
            for(Element e = listGetFirst(itr_disp->objects); e != listGetLast(itr_disp->objects); e = listGetNext(itr_disp->objects)){
                Type type = listGetIteratorType(itr_disp->objects);
                if(type == Picture){
                    if(pictureID == getPicID((PicObject)e)){
                        if(updatePicImage((PicObject)e, createRGB(r,g,b),newImg) != -1){
                            return LED_SIGN_SUCCESS;
                        }
                        return LED_SIGN_OUT_OF_MEMORY;
                    }
                }

            }
            return LED_SIGN_NO_PICTURE_WITH_THE_GIVEN_ID;
        }
    }
    return LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID;
}

LedSignResult deleteObject(int dispID, int objID){
    for(Display itr_disp = listGetFirst(mainBoard->subBoards); itr_disp != listGetLast(mainBoard->subBoards) ; itr_disp = listGetNext(mainBoard->subBoards)){
        if(itr_disp->id == dispID){
            for(Element e = listGetFirst(itr_disp->objects); e != listGetLast(itr_disp->objects); e = listGetNext(itr_disp->objects)){
                Type type = listGetIteratorType(itr_disp->objects);
                if(type == Picture){
                    if(objID == getPicID((PicObject)e)){
                        listRemove(itr_disp->objects,e);
                        destroyPicObject((PicObject)e);
                        return LED_SIGN_SUCCESS;
                    }
                } else {
                    if(objID == getTextID((TextObject)e)){
                        listRemove(itr_disp->objects,e);
                        destroyTextObject((TextObject)e);
                        return LED_SIGN_SUCCESS;
                    }
                }
            }
            return LED_SIGN_OBJECT_WITH_THE_GIVEN_ID;
        }
    }
    return LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID;
}

void set_Full_Brightness(){
    *(copy_cntl+2) = 0X0000007F;
}

void swapBuffer(){
    *(copy_cntl+0) = 0Xffffffff;
    *(copy_cntl+0) = 0;
}

void rotate_90_clockwise(int board_rgb[4*N][8*N], int matrix_line, int matrix_column){
    int tmp[N][N];
    for (int i = N-1; i >= 0; i--) {
        for (int j = N-1; j >= 0; j--) {
            tmp[i][j] = board_rgb[matrix_line*N + N-1-j][matrix_column*N + i];
        }
    }
    for (int i = 0; i < N ; ++i) {
        for (int j = 0; j < N; ++j) {
            board_rgb[matrix_line*N+i][matrix_column*N+j]=tmp[i][j];
        }
    }
}
void rotate_180(int board_rgb[4*N][8*N], int matrix_line, int matrix_column){
    for (int i = 0; i < N/2 ; ++i) {
        for (int j = N-1; j >= 0; j--) {
            int tmp = board_rgb[matrix_line*N + j][matrix_column*N + i];
            board_rgb[matrix_line*N+j][matrix_column*N+i] = board_rgb[matrix_line*N + N-1-j][matrix_column*N + N-1-i];
            board_rgb[matrix_line*N + N-1-j][matrix_column*N + N-1-i] = tmp;
        }
    }
}

void rotate_90_counter_clockwise(int board_rgb[4*N][8*N], int matrix_line, int matrix_column){
    int tmp[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = N-1; j >= 0; j--) {
            tmp[i][N-1-j] = board_rgb[matrix_line*N + N-1-j][matrix_column*N + N-1-i];
        }
    }
    for (int i = 0; i < N ; ++i) {
        for (int j = 0; j < N; ++j) {
            board_rgb[matrix_line*N+i][matrix_column*N+j]=tmp[i][j];
        }
    }
}


void print_board(){

    for(int i=31; i>=0; i--) {
        for (int j = 255; j >= 0; j--) {
            Xil_Out32((u32) (port1 + i*1024 + j*4) ,board_rgb[i][j]); // multiplied by 4 cuz port1 is of type char*
        }
    }
    for(int i=31; i>=0; i--) {
        for (int j = 255; j >= 0; j--) {
            Xil_Out32((u32) (port2 + i*1024 + j*4) ,board_rgb[i+N][j]); // multiplied by 4 cuz port1 is of type char*
        }
    }
    for(int i=31; i>=0; i--) {
        for (int j = 255; j >= 0; j--) {
            Xil_Out32((u32) (port3 + i*1024 + j*4) ,board_rgb[i+2*N][j]); // multiplied by 4 cuz port1 is of type char*
        }
    }
    for(int i=31; i>=0; i--) {
        for (int j = 255; j >= 0; j--) {
            Xil_Out32((u32) (port4 + i*1024 + j*4) ,board_rgb[i+3*N][j]); // multiplied by 4 cuz port1 is of type char*
        }
    }

    xil_printf("finished printing\n");

}

void rotate_board_matrices()
{
    for(int i=0; i<mainBoard->numPorts; i++){
        for(int j=0; j<mainBoard->MatsPerLine[i]; j++){

            switch(*(*(mainBoard->matrixDirections+i)+j)){
                case RIGHT:
                    break;
                case DOWN:
                    rotate_90_clockwise(board_rgb,i,j);
                    break;
                case LEFT:
                	xil_printf("got left\n");
                    rotate_180(board_rgb,i,j);
                    break;
                case UP:
                    rotate_90_counter_clockwise(board_rgb,i,j);
                    break;
                default:
                    break;
            };
        }
    }
};


void scroll_func(byte* rgb_arr, int x, int lenX, int lenY){

	// mirror the rgb array
	for(int i=0; i<lenY; i++){
		for(int j=0; j<lenX/2; j++){
			for(int m=0; m<3; m++){
				byte tmp = *(rgb_arr+3*i*lenX+3*j+m);
				*(rgb_arr+3*i*lenX+3*j+m) = *(rgb_arr+3*i*lenX+3*(lenX-j-1)+m);
				*(rgb_arr+3*i*lenX+3*(lenX-j-1)+m) = tmp;
			}
		}
	}

	for(int k=lenX; k>0; k--){
		for(int j=x+lenX-1; j-k+1 >= x; j--){
			for(int i=0; i<lenY; i++){

				unsigned char bg_pixel[4];
				bg_pixel[3] = *(rgb_arr+3*i*lenX+3*(j-x));	// r
				bg_pixel[2] = *(rgb_arr+3*i*lenX+3*(j-x)+1);	// g
				bg_pixel[1] = *(rgb_arr+3*i*lenX+3*(j-x)+2);	// b
				bg_pixel[0] = 0;	// b

	            Xil_Out32((u32) (port4 + i*1024 + (j-k+1)*4) ,*((int*)bg_pixel)); // multiplied by 4 cuz port1 is of type char*
			}
		}
		swapBuffer();
		sleep(1);
	}
}

LedSignResult DrawBoard() {
	xil_printf("Hey Draw Board ! \n");

	for(int i=0; i<4*N; i++){
		for(int j=0; j<8*N; j++){
			board_rgb[i][j]=0;
		}
	}
	print_board();
	xil_printf("number of sub boards %d  \n",listSize(mainBoard->subBoards));

    for (Display itr_disp = listGetFirst(mainBoard->subBoards); itr_disp != listGetLast(mainBoard->subBoards); itr_disp = listGetNext(mainBoard->subBoards)) {
        for (Element itr_obj = listGetFirst(itr_disp->objects); itr_obj != listGetLast(itr_disp->objects); itr_obj = listGetNext(itr_disp->objects)) {

            if (listGetIteratorType(itr_disp->objects) == Text) {
                TextObject text_obj = (TextObject)itr_obj;
                int x = getTextX(text_obj);
                int y = getTextY(text_obj);
                int lenX = getTextLenX(text_obj);
                int lenY = getTextLenY(text_obj);

                int len = getTextLen(text_obj);
                int* t = getTextData(text_obj);

                byte* rgb_data = text2rgb(t,len,lenX,lenY);

                RGB color = getTextColor(text_obj);

                for(int i=0; i<3*lenX*lenY; i+=3){
                           	rgb_data[i] *=getR(color);
           					rgb_data[i+1] *=getG(color);
           					rgb_data[i+2] *=getB(color);
                }

//            	if(isTextScrollable(text_obj)){
//            		scroll_func(rgb_data,x,lenX, lenY);
//            	}

                int k=0;
                for (int i = y; i < y+lenY; ++i) {
                    for (int j = x+lenX-1; j >=x ; j--) {
                    	if(isTextScrollable(text_obj) == false){
							byte* ptr = (byte*)(&(board_rgb[i][j]));
							ptr++;
							*ptr = *(rgb_data+k+2);
							ptr++;
							*ptr = *(rgb_data+k+1);
							ptr++;
							*ptr = *(rgb_data+k);
							k+=3;
                    	}

                    }
                }
                free(rgb_data);
            } else {
                PicObject pic_obj = (PicObject)itr_obj;
                int x = getPicX(pic_obj);
                int y = getPicY(pic_obj);
                Image imgPtr = getPicImg(pic_obj);
                int imgLenX = getImageWidth(imgPtr);
                int imgLenY = getImageHeight(imgPtr);
                int lenX = getPicLenX(pic_obj);
                int lenY = getPicLenY(pic_obj);

                byte* r = getImageR(imgPtr);
                byte* g = getImageG(imgPtr);
                byte* b = getImageB(imgPtr);

                byte* rgb_data = enlargeImage(imgLenX, imgLenY, lenX, lenY, r,g,b);

                int k = 0;
                for (int i = y; i < y+lenY; ++i) {
                    for (int j = x+lenX-1; j >=x ; j--) {
                        byte* ptr = (byte*)(&(board_rgb[i][j]));
                        ptr++;
                        *ptr = *(rgb_data+k+2);
                        ptr++;
                        *ptr = *(rgb_data+k+1);
                        ptr++;
                        *ptr = *(rgb_data+k);
                        k+=3;
                    }
                }
                free(rgb_data);

            }
        }
    }

    rotate_board_matrices();
    print_board();
    swapBuffer();

    xil_printf("******* HERE2 ******* \n");

    return LED_SIGN_SUCCESS;
}

void putPixelPort4(int row, int col, byte r, byte g, byte b){

	unsigned char bg_pixel[4];
	bg_pixel[3] = r;	// r
	bg_pixel[2] = g;	// g
	bg_pixel[1] = b;	// b
	bg_pixel[0] = 0;	// b

    for(int i=31; i>=0; i--) {
        for (int j = 255; j >= 0; j--) {
        	Xil_Out32((u32) (port4 + row*1024 + col*4) ,*((int*)bg_pixel));
        }
    }

}

