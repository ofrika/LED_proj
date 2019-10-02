//
// Created by Anabusy on 9/29/2019.
//
#include "RGB.h"
#include "Image.h"
#include "stdlib.h"

struct image_t{
    int id;
    int width;
    int height;
    RGB** colorArr;
};

Image createImage(int id, int width, int height, RGB** colordata){
    Image newImg = malloc(sizeof(*newImg));
    if(!newImg){
        return NULL;
    }
    newImg->id = id;
    newImg->colorArr = colordata;
    newImg->width = width;
    newImg->height = height;
    return newImg;
}

void destroyImage(Image img){
    for(int i=0; i<img->height; i++){
        free(img->colorArr[i]);
    }
    free(img->colorArr);
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

RGB** getImageRGB(Image img){
    if(!img){
        return NULL;
    }
    return img->colorArr;
}