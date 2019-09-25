#ifndef MIXEDLIST_PICOBJECT_H
#define MIXEDLIST_PICOBJECT_H

#include "RGB.h"
typedef enum existing_Pics_e Existing_Pics;
typedef enum pic_Type_e{EXISTING, NEW} Picture_Type;
typedef struct picObject_t *PicObject;

PicObject createPicObject(int id, int x, int y, int lenX, int lenY, RGB color, int picNumber);
PicObject copyPicObject(PicObject picObject);
void destroyPicObject(PicObject picObject);
int getPicID(PicObject picObject);
int getPicX(PicObject picObject);
int getPicY(PicObject picObject);
int getPicLenX(PicObject picObject);
int getPicLenY(PicObject picObject);
int updatePicNumber(PicObject picObject, RGB newColor, int newPicNumber);

#endif //MIXEDLIST_PICOBJECT_H
