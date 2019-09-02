// ****************** Created by Anabusy & Kahana on 09/02/2019 ******************

#ifndef DISPLAY_H
#define DISPLAY_H
#include "RGB.h"
#include "TextObject.h"
#include "PicObject.h"
#include <stdbool.h>




// *************************** Data structures ********************************

typedef struct rgb_t *RGB;
typedef enum pic_Type_e{EXISTING, NEW} Picture_Type;
typedef enum direction_t {UP, RIGHT, DOWN, LEFT} Direction;
typedef enum ledSignResult_t{
    LED_SIGN_SUCCESS,
    LED_SIGN_OUT_OF_MEMORY,
    LED_SIGN_OUT_OF_BOARD_COARDINATES,
    LED_SIGN_DISPLAY_ID_ALREADY_EXIST,
    LED_SIGN_OBJECT_ID_ALREADY_EXIST,
    LED_SIGN_ANOTHER_DISPLAY_LOCATED_THERE,
    LED_SIGN_ANOTHER_OBJECT_LOCATED_THERE,
    LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID,
    LED_SIGN_NO_OBJECT_WITH_THE_GIVEN_ID
} LedSignResult;




// *************************** Function declarations ********************************

// Board

// Allocates space for board struct, fills in screen size values, creates 2 empty screen variables
LedSignResult initBoard(int ip_add, int numPorts, int numLedsInMatrix, char* numMatsInPort, char* directions);

// Frees allocated memory for board
void destroyBoard();

// SubBoard

// Allocates space for display struct, fills in size values, and adds it to sub-board list
LedSignResult addSubBoard(int bID, int x, int y, int lenX, int lenY);

// deletes all the objects in the desired display
LedSignResult cleanSubBoard(int bID);

// Frees allocated memory for disp and deletes from list
LedSignResult deleteSubBoard(int bID);

// Object

// Adds Text to the adequate Display. return -1 if the location is Illegal
LedSignResult addText(int TextID, int x, int y, int lenX, int lenY, byte r, byte g, byte b, bool scrollable, int size, char* data);

// Adds picture to the adequate Display. return -1 if the location is Illegal
LedSignResult addPicture(int pictureID, int x, int y, int lenX, int lenY, byte r, byte g, byte b, Picture_Type type, char* data);

// update the text in the given display and writes a new data
LedSignResult updateText(int bID, int textID, char* data);

// update the picture in the given display and draw a new data
LedSignResult updatePicture(int bID, int pictureID, byte r, byte g, byte b, Picture_Type type, char* data);

// Frees allocated memory for obj and deletes from list
LedSignResult deleteObject(int bID, int objID);

// Draws new screen on LED screen
int swapBuffer();

// Test functions

// Draw one pixel on board
void testOnePixel(int x, int y, byte r, byte g, byte b);
// Draw running pixel on board
void testRunningPixel(int x, int y, byte r, byte g, byte b);
// Draw picture frame all over board
void testPictureFrame(RGB color);
// Draw text in display
void testText(int x, int y, char* text, byte r, byte g, byte b);





#endif //MIXEDLIST_DISPLAY_H
