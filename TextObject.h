#ifndef MIXEDLIST_TEXTOBJECT_H
#define MIXEDLIST_TEXTOBJECT_H

#include "RGB.h"
#include <stdbool.h>

typedef struct textObject_t *TextObject;

TextObject createTextObject(int id, int x, int y, int lenX, int lenY, RGB color, bool scrollable, int* data, int data_len);
TextObject copyTextObject(TextObject textObject);
void destroyTextObject(TextObject textObject);
int getTextID(TextObject textObject);
int getTextX(TextObject textObject);
int getTextY(TextObject textObject);
int getTextLenX(TextObject textObject);
int getTextLenY(TextObject textObject);
RGB getTextColor(TextObject textObject);
int* getTextData(TextObject textObject);
int getTextLen(TextObject textObject);
bool isTextScrollable(TextObject textObject);
int updateTextData(TextObject textObject, int* new_data, int new_data_size);
int updateTextRGB(TextObject textObject, RGB newColor);

#endif
