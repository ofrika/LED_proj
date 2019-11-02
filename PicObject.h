#ifndef MIXEDLIST_PICOBJECT_H
#define MIXEDLIST_PICOBJECT_H

#include "Image.h"
#include "RGB.h"

typedef struct picObject_t *PicObject;

PicObject createPicObject(int id, int x, int y, int lenX, int lenY, RGB color, Image picturePtr);
void destroyPicObject(PicObject picObject);

int getPicID(PicObject picObject);
int getPicX(PicObject picObject);
int getPicY(PicObject picObject);
int getPicLenX(PicObject picObject);
int getPicLenY(PicObject picObject);
Image getPicImg(PicObject picObject);
int updatePicImage(PicObject picObject, Image newImgPtr);
int updatePicColor(PicObject picObject, RGB newColor);

#endif //MIXEDLIST_PICOBJECT_H
