// ******************* Includes ******************* 

#include some linked list library


// ******************* Basic data structures ******************* 

enum picType{exiting, new};
enum objectType{text, picture};
enum existingPics{U, D, L, R, FLOWER, SMILEY}

typedef struct rgb_t 
{ 
	int r;
	int g;
	int b;
}*RGB;

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
   int numMatInPort; // Default: 8
   int numRowInMatrix; // Default: 32
   int numColInMatrix; // Default: 32
   List subBoards;
}*Board;

typedef struct display_t 
{ 
	int id;
	int x;
	int y;
	int lenX;
	int lenY;
	List textObjects;
	List picObjects;
}*Display;

typedef struct textObject_t 
{ 
	int id;
	int x;
	int y;
	int lenX;
	int lenY;
	RGB color;
	bool scrollable;
	int size;
	char* data;
}*TextObject;

typedef struct picObject_t 
{ 
	int id;
	int x;
	int y;
	int lenX;
	int lenY;
	RGB color;
	picType type;
	void* data; // If type = existing then interpret data as string and find in exiting type enum
				// Otherwise, interpret as string containing RGB values seperated by , and ;
}*PicObject;


// ******************* Global variables ******************* 

unsigned char* port1 = (unsigned char *) 0x40000000;
unsigned char* port2 = (unsigned char *) 0x42000000;
unsigned char* port3 = (unsigned char *) 0x44000000;
unsigned char* port4 = (unsigned char *) 0x46000000;

Board mainBoard
Screen oldScreen;
Screean newScreen;


// ******************* Data structure handling function declerations ******************* 

// Allocates space for board struct, fills in screen size values, creates 2 empty screen variables
int initBoard(int numPorts, int numMatInPort, int numRowInMatrix, int numColInMatrix); 
// Allocates space for display struct, fills in size values, and adds it to sub-board list
int addDisplay(int x, int y, int lenX, int lenY);
// Allocates space for display struct, fills in size values, and adds it to sub-board list
// Checks if x,y,lenX,lenY are available
int addObject(objectType t, int id, int x, int y, int lenX, int lenY, bool scrollable, int size);
// Cleans the display and writes the new data
int updateTextObject(int dispNum, int textObjID, char* data, RGB color);
int updatePicObject(int dispNum, int picObjID, picType type, void* data, RGB color);
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
