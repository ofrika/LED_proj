//
// Created by Anabusy on 9/29/2019.
//
#include "Image.h"

#include "stdlib.h"
#include "RGB.h"
#include "xil_io.h"

struct image_t{
    int id;
    int width;
    int height;
    byte* colorR;
    byte* colorG;
    byte* colorB;
};

Image createImage(int id, int width, int height, byte* rData, byte* gData, byte* bData){
    Image newImg = malloc(sizeof(*newImg));
    if(!newImg){
    	xil_printf("haa??\n");
        return NULL;
    }
    newImg->id = id;
    newImg->colorR = rData;
    newImg->colorG = gData;
    newImg->colorB = bData;
    newImg->width = width;
    newImg->height = height;
    return newImg;
}

void destroyImage(Image img){
    for(int i=0; i<img->height; i++){
        free(img->colorR);
        free(img->colorG);
        free(img->colorB);
    }
    free(img);
}

int getImageId(Image img){
    if(!img){
        return -1;
    }
    return img->id;
}

int getImageWidth(Image img){
    if(!img){
        return -1;
    }
    return img->width;
}

int getImageHeight(Image img){
    if(!img){
        return -1;
    }
    return img->height;
}

byte* getImageR(Image img){
    if(!img){
        return NULL;
    }
    return img->colorR;
}

byte* getImageG(Image img){
    if(!img){
        return NULL;
    }
    return img->colorG;
}

byte* getImageB(Image img){
    if(!img){
        return NULL;
    }
    return img->colorB;
}
