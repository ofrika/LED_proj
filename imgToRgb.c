#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "resize.c"

typedef unsigned char byte;

void printToBmp(const char* filename, int w, int h, byte* redData, byte* greenData, byte* blueData){
	FILE *f;
	unsigned char *img = NULL;
	int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int

	img = (unsigned char *)malloc(3*w*h);
	memset(img,0,3*w*h);

	for(int i=0; i<w; i++){
		for(int j=0; j<h; j++){
			int x=i, y=(h-1)-j;
			byte r = *(redData+j*w+i);
			byte g = *(greenData+j*w+i);
			byte b = *(blueData+j*w+i);
			img[(x+y*w)*3+2] = r;
			img[(x+y*w)*3+1] = g;
			img[(x+y*w)*3+0] = b;
		}
	}

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
	unsigned char bmppad[3] = {0,0,0};

	bmpfileheader[ 2] = (unsigned char)(filesize    );
	bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);

	bmpinfoheader[ 4] = (unsigned char)(       w    );
	bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
	bmpinfoheader[ 6] = (unsigned char)(       w>>16);
	bmpinfoheader[ 7] = (unsigned char)(       w>>24);
	bmpinfoheader[ 8] = (unsigned char)(       h    );
	bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
	bmpinfoheader[10] = (unsigned char)(       h>>16);
	bmpinfoheader[11] = (unsigned char)(       h>>24);

	f = fopen(filename,"wb");
	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);
	for(int i=0; i<h; i++)
	{
		fwrite(img+(w*(h-i-1)*3),3,w,f);
		fwrite(bmppad,1,(4-(w*3)%4)%4,f);
	}

	free(img);
	fclose(f);
}


byte* getRgbData(const char* filename, int w, int h) {
    FILE* f = fopen(filename, "rb");
    if(!f){
        printf("haaa?\n");
    }
    byte info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];


    int size = 3 * width * height;
    byte* data = malloc(sizeof(byte)*size); // allocate 3 bytes per pixel
    fread(data, sizeof(byte), size, f); // read the rest of the data at once
    fclose(f);

    for(int i = 0; i < size; i += 3)
    {
        unsigned char tmp = data[i];
        data[i] = data[i+2];
        data[i+2] = tmp;
    }
    int triSize = 3 * w * h;
    byte* triData = malloc(sizeof(byte)*triSize); // allocate 3 bytes per pixel

    for (int i = 0; i < triSize; ++i) {
        triData[i] = 0;
    }
    int j = triSize-1;
    for (int i = 0 ; i < size ; i+=3) {
        if(((i/3)%width) < w){
            triData[j-2] = data[i];
            triData[j-1] = data[i+1];
            triData[j] = data[i+2];
            j-=3;
        }
    }
    for(int i=0; i<triSize; i+=3){
        printf("%d %d %d\n",(int)triData[i], (int)triData[i+1], (int)triData[i+2]);
    }
    printf("%d\n",j);
    free(data);
    return triData;
    // triData is organized up->down, left->right
}


byte* enlargeImage(int factor, byte* rdata, byte* gdata, byte* bdata, int lenX, int lenY){
	printToBmp("orgImg.bmp",lenX, lenY, rdata,gdata,bdata);
	resize(factor,"orgImg.bmp","resizedImg.bmp");
	return getRgbData("resizedImg.bmp", lenX, lenY);
}
