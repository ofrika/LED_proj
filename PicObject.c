//
// Created by Anabusy on 9/2/2019.
//

#include "PicObject.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct picObject_t{
    int id;
    int x;
    int y;
    int lenX;
    int lenY;
    RGB color;
    int picNumber;
//    void* data;         // If type = existing then interpret data as enum value
    // Otherwise, interpret as 2D array of RGB
};

enum existing_Pics_e{
    U,
    D,
    L,
    R,
    FLOWER,
    SMILEY,
    UNDEFINED
};

Existing_Pics convertPicStrToEnumExisting(char* data){
    if(strcmp(data, "up") == 0){
        return U;
    } else if(strcmp(data, "down") == 0){
        return D;
    } else if(strcmp(data, "left") == 0){
        return L;
    } else if(strcmp(data, "right") == 0){
        return R;
    } else if(strcmp(data, "smiley") == 0){
        return SMILEY;
    } else if(strcmp(data, "flower") == 0){
        return FLOWER;
    } else {
        return UNDEFINED;
    }
}

RGB** convertPicStrToRGBArray(char* data, int width, int height){
    RGB** arr = malloc(sizeof(*arr)*height);
    for(int i=0; i<height; i++){
        arr[i] = malloc(sizeof(**arr)*width);
    }
    char* tmp = malloc(strlen(data)+1);
    strcpy(tmp,data);
    char *ptr = strtok(tmp, ",;");
    int count = 0;
    byte r=0, g=0, b=0;
    for(int i=0; ptr != NULL && i<height; i++){
        for(int j=0; ptr != NULL && j<width; j++){
            byte x = atoi(ptr);
            if(count%3 == 0){
                r = x;
                printf("r == '%d'\n", r);
            } else if(count%3 == 1){
                g = x;
                printf("g == '%d'\n", g);
            } else if(count%3 == 2){
                b = x;
                printf("b == '%d'\n", b);
            } else {
                return NULL;
            }
            ptr = strtok(NULL, ",;");
            if(count == 2) {         // then we can construct an RGB item
                RGB newRGB = createRGB(r, g, b);
                arr[i][j] = newRGB;
            }
            count++;
        }
    }
    return arr;
}


PicObject createPicObject(int id, int x, int y, int lenX, int lenY, RGB color, int picNumber){
    PicObject newPic = malloc(sizeof(*newPic));
    if(!newPic){
        return NULL;
    }
    newPic->id = id;
    newPic->x = x;
    newPic->y = y;
    newPic->lenX = lenX;
    newPic->lenY = lenY;
    newPic->color = copyRGB(color);
    if(!newPic->color){
        free(newPic);
        return NULL;
    }
    newPic->picNumber = picNumber;
    // we will call this function when adding a new image to the magar
    //    newPic->data = (void*)convertPicStrToRGBArray(data, lenX, lenY);
    return newPic;
}

PicObject copyPicObject(PicObject picObject){
    PicObject newPic = createPicObject(picObject->id, picObject->x, picObject->y, picObject->lenX, picObject->lenY, picObject->color,picObject->picNumber);
    if(!newPic){
        return NULL;
    }
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

int updatePicNumber(PicObject picObject, RGB newColor, int newPicNumber){
    if(!picObject || !newColor){
        return -1;
    }
    destroyRGB(picObject->color);
    picObject->picNumber = newPicNumber;
    return 0;
}