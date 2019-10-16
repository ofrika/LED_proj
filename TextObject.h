#ifndef MIXEDLIST_TEXTOBJECT_H
#define MIXEDLIST_TEXTOBJECT_H

#include <stdbool.h>
#include "RGB.h"

typedef struct textObject_t *TextObject;

TextObject createTextObject(int id, int x, int y, int lenX, int lenY, RGB color, bool scrollable, int size, char* data);
TextObject copyTextObject(TextObject textObject);
void destroyTextObject(TextObject textObject);
int getTextID(TextObject textObject);
int getTextX(TextObject textObject);
int getTextY(TextObject textObject);
int getTextLenX(TextObject textObject);
int getTextLenY(TextObject textObject);
int updateTextData(TextObject textObject, char* newData);

#endif
