#ifndef SRC_TEXTTORGB_H_
#define SRC_TEXTTORGB_H_


typedef unsigned char byte;

byte* rgbData(const char* filename, int w, int h);
void text2Image(int NumOfChars, const char* tmp, int final_height);
byte* convert_text_to_rgb(int NumTextChars, int width, int height);

#endif
