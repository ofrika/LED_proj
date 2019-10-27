//
// Created by Anabusy on 9/2/2019.
//

#include "TextObject.h"
#include <stdlib.h>
#include <string.h>

struct textObject_t{
    int id;
    int x;
    int y;
    int lenX;
    int lenY;
    RGB color;
    bool scrollable;
    int data_len;
    int* data;
};


TextObject createTextObject(int id, int x, int y, int lenX, int lenY, RGB color, bool scrollable, int* data, int data_len){
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
    newText->data_len = data_len;
    newText->data = malloc(sizeof(int)*data_len);
    if(!newText->data){
        free(newText);
        return NULL;
    }
    for(int i=0; i<data_len; i++){
    	newText->data[i] = data[i];
    }
    return newText;
}

TextObject copyTextObject(TextObject textObject){
    TextObject newText = createTextObject(textObject->id, textObject->x, textObject->y, textObject->lenX, textObject->lenY, textObject->color, textObject->scrollable, textObject->data, textObject->data_len);
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

RGB getTextColor(TextObject textObject){
    if(!textObject){
        return NULL;
    }
    return textObject->color;
}

int* getTextData(TextObject textObject){
    if(!textObject){
        return NULL;
    }
    return textObject->data;
}
int getTextLen(TextObject textObject){
    if(!textObject){
        return -1;
    }
    return textObject->data_len;
}

int updateTextData(TextObject textObject, int* new_data, int new_data_size){
    if(!textObject){
        return -1;
    }
    free(textObject->data);
    textObject->data = malloc(sizeof(int)*new_data_size);
    if(!textObject->data){
        return -1;
    }
    textObject->data_len = new_data_size;
    for(int i=0; i<new_data_size; i++){
    	textObject->data[i] = new_data[i];
    }
    return 0;
}
