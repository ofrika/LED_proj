//
// Created by Anabusy on 9/29/2019.
//

#include "RGB.h"
typedef struct image_t *Image;

Image createImage(int id, int width, int height, byte* rData, byte* gData, byte* bData);
void destroyImage(Image img);
int getImageId(Image img);
int getImageWidth(Image img);
int getImageHeight(Image img);
byte* getImageR(Image img);
byte* getImageG(Image img);
byte* getImageB(Image img);
