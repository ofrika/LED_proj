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

char* getTextData(TextObject textObject){
    if(!textObject){
        return NULL;
    }
    return textObject->data;
}

int updateTextData(TextObject textObject, char* newData){
    if(!textObject){
        return -1;
    }
    free(textObject->data);
    textObject->data = malloc(sizeof(char)*(strlen(newData)+1));
    if(!textObject->data){
        return -1;
    }
    strcpy(textObject->data, newData);
    return 0;
}
