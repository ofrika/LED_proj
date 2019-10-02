#ifndef MIXEDLIST_RGB_H
#define MIXEDLIST_RGB_H

typedef unsigned char byte;

typedef struct rgb_t *RGB;

RGB createRGB(byte r, byte g, byte b);
RGB copyRGB(RGB rgb);
void destroyRGB(RGB rgb);
byte getR(RGB rgb);
byte getG(RGB rgb);
byte getB(RGB rgb);


#endif //MIXEDLIST_RGB_H
