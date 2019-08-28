// Created by Anabusy on 8/29/2019.
#ifndef MIXEDLIST_DISPLAY_H
#define MIXEDLIST_DISPLAY_H

#include "helper.h"

// ******************* Data structure handling function declerations *******************

// Allocates space for board struct, fills in screen size values, creates 2 empty screen variables
int initBoard(char* ip_add, int numPorts, int numLedsInMatrix, int num_port1, int num_port2, int num_port3, int num_port4);

// Allocates space for display struct, fills in size values, and adds it to sub-board list
int addDisplay(int x, int y, int lenX, int lenY);

// Checks if x,y,lenX,lenY are available
int addObject(Object_Type o, int id, int x, int y, int lenX, int lenY, bool scrollable, int size);

// Cleans the display and writes the new data
int updateTextObject(int dispNum, int textObjID, char* data, RGB color);

int updatePicObject(int dispNum, int picObjID, Picture_Type type, void* data, RGB color);

int cleanObject(int dispNum, int objID);

int cleanDisplay(int dispNum);

// Frees allocated memory for obj and deletes from list
int deleteObject(int dispNum, int objID);

// Frees allocated memory for disp and deletes from list
int deleteDisplay(int dispNum);

// Frees allocated memory for board
int destroyBoard();

// Draw new screen on LED screen
int swapBuffer();



// ******************* Test functions *******************

// Draw one pixel on board
void testOnePixel(int x, int y, RGB color);
// Draw running pixel on board
void testRunningPixel(int x, int y, RGB color);
// Draw picture frame all over board
void testPictureFrame(RGB color);
// Draw text in display
void testText(int x, int y, char* text, RGB color);

#endif //MIXEDLIST_DISPLAY_H
