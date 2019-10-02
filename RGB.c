//
// Created by Anabusy on 9/2/2019.
//

#include "RGB.h"
#include <stdlib.h>
#include <string.h>

struct rgb_t{
    byte r;
    byte g;
    byte b;
};

RGB createRGB(byte r, byte g, byte b){
    RGB newRGB = malloc(sizeof(*newRGB));
    if(!newRGB){
        return NULL;
    }
    newRGB->r = r;
    newRGB->g = g;
    newRGB->b = b;
    return newRGB;
}

RGB copyRGB(RGB rgb){
    if(!rgb){
        return NULL;
    }
    RGB newRGB = createRGB(rgb->r, rgb->g, rgb->b);
    if(!newRGB){
        return NULL;
    }
    return newRGB;
}

void destroyRGB(RGB rgb){
    if(!rgb){
        return;
    }
    free(rgb);
}

byte getR(RGB rgb){
    if(!rgb){
        return 0;
    }
    return rgb->r;
}

byte getG(RGB rgb){
    if(!rgb){
        return 0;
    }
    return rgb->g;
}

byte getB(RGB rgb){
    if(!rgb){
        return 0;
    }
    return rgb->b;
}