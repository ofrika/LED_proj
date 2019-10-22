#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bmp.h"

typedef unsigned char byte;

int resize(int factor, const char *in, const char *out);
void printToBmp(const char* filename, int w, int h, byte* redData, byte* greenData, byte* blueData);
byte* getRgbData(const char* filename, int w, int h);
byte* enlargeImage(int orgLenX, int orgLenY, int finalLenX, int finalLenY, byte* rdata, byte* gdata, byte* bdata);
