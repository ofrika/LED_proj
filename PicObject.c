//
// Created by Anabusy on 9/2/2019.
//

#include "PicObject.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Image.h"

struct picObject_t{
    int id;
    int x;
    int y;
    int lenX;
    int lenY;
    RGB color;
    Image imgPtr;
//    void* data;         // If type = existing then interpret data as enum value
    // Otherwise, interpret as 2D array of RGB
};

PicObject createPicObject(int id, int x, int y, int lenX, int lenY, RGB color, Image imgPtr){
    PicObject newPic = malloc(sizeof(*newPic));
    if(!newPic){
        return NULL;
    }
    newPic->id = id;
    newPic->x = x;
    newPic->y = y;
    newPic->lenX = lenX;
    newPic->lenY = lenY;
    newPic->color = color;
    newPic->imgPtr = imgPtr;
    return newPic;
}

void destroyPicObject(PicObject picObject){
    if(!picObject){
        return;
    }
    destroyRGB(picObject->color);
    // we will put this when destroying the images list in the mainBoard
//    if(picObject->type == NEW){
//        RGB** arr = picObject->data;
//        for (int i = 0; i < picObject->lenY ; ++i) {
//            for (int j = 0; j < picObject->lenX; ++j) {
//                destroyRGB(arr[i][j]);
//            }
//            free(arr[i]);
//        }
//        free(arr);
//    }
    free(picObject);
}

int getPicID(PicObject picObject){
    if(!picObject){
        return -1;
    }
    return picObject->id;
}

int getPicX(PicObject picObject){
    if(!picObject){
        return -1;
    }
    return picObject->x;
}

int getPicY(PicObject picObject){
    if(!picObject){
        return -1;
    }
    return picObject->y;
}

int getPicLenX(PicObject picObject){
    if(!picObject){
        return -1;
    }
    return picObject->lenX;
}

int getPicLenY(PicObject picObject){
    if(!picObject){
        return -1;
    }
    return picObject->lenY;
}

Image getPicImg(PicObject picObject){
    if(!picObject){
        return NULL;
    }
    return picObject->imgPtr;
}

int updatePicImage(PicObject picObject, RGB newColor, Image newImgPtr){
    if(!picObject || !newColor){
        return -1;
    }
    destroyRGB(picObject->color);
    picObject->color = newColor;
    picObject->imgPtr = newImgPtr;
    return 0;
}
