// Created by Anabusy on 8/29/2019.
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>

typedef unsigned char byte;

typedef enum ledSignResult_t LedSignResult;
typedef enum Direction_t{UP, DOWN, LEFT, RIGHT, UNDEFINED_DIR} Direction;


enum ledSignResult_t {
    LED_SIGN_SUCCESS,
    LED_SIGN_OUT_OF_MEMORY,
    LED_SIGN_ILLEGAL_ARGUMENTS,
    LED_SIGN_ERROR_WHILE_PARSING_DIRECTIONS,
    LED_SIGN_IMAGE_DOESNT_EXIST_IN_STOCK,
    LED_SIGN_OUT_OF_BOARD_COARDINATES,
    LED_SIGN_DISPLAY_CAN_NOT_CONTAIN_THAT_TEXT,
    LED_SIGN_DISPLAY_CAN_NOT_CONTAIN_THAT_PICTURE,
    LED_SIGN_DISPLAY_ID_ALREADY_EXIST,
    LED_SIGN_OBJECT_ID_ALREADY_EXIST,
    LED_SIGN_ANOTHER_DISPLAY_LOCATED_THERE,
    LED_SIGN_ANOTHER_OBJECT_LOCATED_THERE,
    LED_SIGN_OBJECT_WITH_THE_GIVEN_ID,
    LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID,
    LED_SIGN_NO_PICTURE_WITH_THE_GIVEN_ID,
    LED_SIGN_NO_TEXT_WITH_THE_GIVEN_ID,
    LED_SIGN_PICTURE_DIMENSIONS_ARE_TOO_MUCH_SMALL
};


// ************** Data structure handling function declerations *******************

// Allocates space for board struct, fills in screen size values, creates 2 empty screen variables
LedSignResult initBoard(int numPorts, int* ports, char* directions);

// Frees allocated memory for board
void destroyBoard();

// Allocates space for display struct, fills in size values, and adds it to sub-board list
LedSignResult addSubBoard(int dispID, int x, int y, int lenX, int lenY);

// deletes all the objects in the desired display
LedSignResult cleanSubBoard(int subBoardID);

// Frees allocated memory for disp and deletes from list
LedSignResult deleteSubBoard(int subBoardID);

// Adds Text to the adequate Display. return -1 if the location is Illegal
LedSignResult createTextArea(int dispID, int TextID, int x, int y, int lenX, int lenY, byte r, byte g, byte b, bool scrollable);

// Adds new image to the stock so the user can use "addPicture" operation
LedSignResult addImageToStock(int imageID, int height, int width, byte* rData, byte* gData, byte* bData);

// Adds picture to the adequate Display
LedSignResult createPictureArea(int dispID, int pictureID, int x, int y, int lenX, int lenY, bool newColor, byte r, byte g, byte b);

// updates the text in the given display and writes a new data
LedSignResult updateText(int dispID, int textID, int* data, int data_len);

// updates the text color 
LedSignResult updateTextColor(int dispID, int textID, byte r, byte g, byte b);

// update the picture in the given display and draw a new data
LedSignResult updatePicture(int dispID, int pictureID, int newImgId);

// updates the picture color 
LedSignResult updatePictureColor(int dispID, int pictureID, byte r, byte g, byte b);

// Frees allocated memory for obj and deletes from list
LedSignResult deleteArea(int dispID, int areaID);

// gets the current status of the system
void getStatus();

// Draws new screen on LED screen
LedSignResult DrawBoard();

#endif
