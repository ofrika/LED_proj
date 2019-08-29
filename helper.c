//
// Created by Anabusy on 8/28/2019.
//
#include "helper.h"
#include <stdlib.h>
#include "string.h"
#include <stdio.h>

/* *************************** RGB **************************** */
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
    free(rgb);
}

/* *************************** TextObject **************************** */
struct textObject_t{
    int id;
    int x;
    int y;
    int lenX;
    int lenY;
    RGB color;
    bool scrollable;
    int size;
    char* data;
};

TextObject createTextObject(int id, int x, int y, int lenX, int lenY, RGB color, bool scrollable, int size, char* data){
    TextObject newText = malloc(sizeof(*newText));
    if(!newText){
        return NULL;
    }
    newText->id = id;
    newText->x = x;
    newText->y = y;
    newText->lenX = lenX;
    newText->lenY = lenY;
    newText->color = copyRGB(color);
    newText->scrollable = scrollable;
    newText->size = size;
    newText->data = malloc(sizeof(char)*(strlen(data)+1));
    if(!newText->data){
        free(newText);
        return NULL;
    }
    strcpy(newText->data, data);
    return newText;
}

TextObject copyTextObject(TextObject textObject){
    TextObject newText = createTextObject(textObject->id, textObject->x, textObject->y, textObject->lenX, textObject->lenY, textObject->color, textObject->scrollable, textObject->size, textObject->data);
    if(!newText){
        return NULL;
    }
    return newText;
}

void destroyTextObject(TextObject textObject){
    destroyRGB(textObject->color);
    free(textObject->data);
    free(textObject);
}

int getTextID(TextObject textObject){
    if(!textObject){
        return -1;
    }
    return textObject->id;
}

int getTextX(TextObject textObject){
    if(!textObject){
        return -1;
    }
    return textObject->x;
}

int getTextY(TextObject textObject){
    if(!textObject){
        return -1;
    }
    return textObject->y;
}

int getTextLenX(TextObject textObject){
    if(!textObject){
        return -1;
    }
    return textObject->lenX;
}

int getTextLenY(TextObject textObject){
    if(!textObject){
        return -1;
    }
    return textObject->lenY;
}

/* *************************** PictureObject **************************** */
struct picObject_t{
    int id;
    int x;
    int y;
    int lenX;
    int lenY;
    RGB color;
    Picture_Type type;
    void* data;         // If type = existing then interpret data as enum value
                        // Otherwise, interpret as 2D array of RGB
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
    char *ptr = strtok(data, ",;");
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

PicObject createPicObject(int id, int x, int y, int lenX, int lenY, RGB color, Picture_Type type, char* data){
    PicObject newPic = malloc(sizeof(*newPic));
    if(!newPic){
        return NULL;
    }
    newPic->id = id;
    newPic->x = x;
    newPic->y = y;
    newPic->lenX = lenX;
    newPic->lenY = lenY;
    if(type == EXISTING){
        newPic->color = copyRGB(color);
        if(!newPic->color){
            free(newPic);
            return NULL;
        }
    }
    newPic->type = type;
    if(type == EXISTING){
        newPic->data = (void*)convertPicStrToEnumExisting(data);
    } else {                //type = NEW
        newPic->data = (void*)convertPicStrToRGBArray(data, lenX, lenY);
    }
}

PicObject copyPicObject(PicObject picObject){
    PicObject newPic = createPicObject(picObject->id, picObject->x, picObject->y, picObject->lenX, picObject->lenY, picObject->color,picObject->type, picObject->data);
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
    if(picObject->type == NEW){
        RGB** arr = picObject->data;
        for (int i = 0; i < picObject->lenY ; ++i) {
            for (int j = 0; j < picObject->lenX; ++j) {
                destroyRGB(arr[i][j]);
            }
            free(arr[i]);
        }
        free(arr);
    }
    free(picObject);
    return;
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