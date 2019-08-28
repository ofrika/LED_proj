#ifndef HELPER_H_
#define HELPER_H_

#include <stdbool.h>

typedef unsigned char byte;

typedef enum pic_Type_e{EXISTING, NEW} Picture_Type;
typedef enum object_Type_e{Text , Picture} Object_Type;
typedef enum existing_Pics_e{U, D, L, R, FLOWER, SMILEY, UNDEFINED} Existing_Pics;

/* *************************** RGB Declerations **************************** */
typedef struct rgb_t *RGB;

RGB createRGB(byte r, byte g, byte b);
RGB copyRGB(RGB rgb);
void destroyRGB(RGB rgb);

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

/* *************************** PictureObject Declerations**************************** */
typedef struct picObject_t *PicObject;

PicObject createPicObject(int id, int x, int y, int lenX, int lenY, RGB color, Picture_Type type, void* data);
PicObject copyPicObject(PicObject picObject);
void destroyPicObject(PicObject picObject);
int getPicID(PicObject picObject);
int getPicX(PicObject picObject);
int getPicY(PicObject picObject);
int getPicLenX(PicObject picObject);
int getPicLenY(PicObject picObject);

/* ********************************************************************************* */





#endif
