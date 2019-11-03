#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>


// ************** Data structure handling function declerations *******************


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


// ************** Function declerations - general board handling *******************

// Allocates space for board struct, fills in screen size values, creates 2 empty screen variables
LedSignResult initBoard(int numPorts, int* ports, char* directions);

// Frees allocated memory for board
void destroyBoard();

// Allocates space for sub-board struct, fills in size values, and adds it to sub-board list
LedSignResult addSubBoard(int subBoardID, int x, int y, int lenX, int lenY);

// deletes all the objects in the desired sub-board
LedSignResult cleanSubBoard(int subBoardID);

// Frees allocated memory for sub-board and deletes from list
LedSignResult deleteSubBoard(int subBoardID);

// Frees allocated memory for obj and deletes from list
LedSignResult deleteArea(int subBoardID, int areaID);

// Gets the current status of the system
void getStatus();

// Draws new screen on LED screen
LedSignResult DrawBoard();


// ************** Function declerations - text object handling *******************


// Adds Text to the requested sub-board. Returns -1 if the location is Illegal
LedSignResult createTextArea(int subBoardID, int TextID, int x, int y, int lenX, int lenY, byte r, byte g, byte b, bool scrollable);

// Updates the text in the given sub-board and writes a new data
LedSignResult updateText(int subBoardID, int textID, int* data, int data_len, bool draw);

// Updates the text color 
LedSignResult updateTextColor(int subBoardID, int textID, byte r, byte g, byte b, bool draw);


// ************** Function declerations - picture object handling *******************


// Adds picture to the requested sub-board
LedSignResult createPictureArea(int subBoardID, int pictureID, int x, int y, int lenX, int lenY);

// Adds new image to the database so the user can use "addPicture" operation
LedSignResult addImageToDB(int imageID, int height, int width, byte* rData, byte* gData, byte* bData);

// Updates the picture in the given sub-board and draw a new data
LedSignResult updatePicture(int subBoardID, int pictureID, int index, bool draw);

// Updates the picture color 
LedSignResult updatePictureColor(int subBoardID, int pictureID, byte orig_r, byte orig_g, byte orig_b, byte new_r, byte new_g, byte new_b, bool draw);


#endif