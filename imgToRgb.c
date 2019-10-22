#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bmp.h"
#include "imgToRgb.h"
#include "xil_io.h"
#include "RGB.h"

/**
 * resize.c part of bmp.h by Iliyan Stankov
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Copies a BMP piece by piece, just because.
 */

int resize(int factor, const char *in, const char *out){
    // remember filenames
    const char *infile = in;
    const char *outfile = out;

    //get the factor
    int n = factor;
    if (n < 1 || n > 100)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof (BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof (BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0. If not, quit
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // change the header files

    int oldWidth = bi.biWidth; //save old width
    bi.biWidth *= n; // the new width

    int oldHeight = bi.biHeight; // save old height
    bi.biHeight *= n; // the new height

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof (RGBTRIPLE)) % 4) % 4;
    int oldPadding = (4 - (oldWidth * sizeof (RGBTRIPLE)) % 4) % 4;

    bi.biSizeImage = abs(bi.biHeight) * ((bi.biWidth * sizeof (RGBTRIPLE)) + padding); // determine the new size of the image
    bf.bfSize = bi.biSizeImage + sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER); // determine the new file size.

    RGBTRIPLE scan[bi.biWidth];

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof (BITMAPFILEHEADER), 1, outptr);


    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof (BITMAPINFOHEADER), 1, outptr);


    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(oldHeight); i < biHeight; i++)
    {
        int cursor = 0;

        // iterate over pixels in scanline
        for (int j = 0; j < oldWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof (RGBTRIPLE), 1, inptr);

            for (int k = 0; k < n; k++)
            {
                scan[cursor].rgbtBlue = triple.rgbtBlue;
                scan[cursor].rgbtGreen = triple.rgbtGreen;
                scan[cursor].rgbtRed = triple.rgbtRed;
                cursor++;
            }
        }
        // now I have read the entire line
        for (int l = 0; l < n; l++)
        {
            // write out the new resized scanline
            for (cursor = 0; cursor < bi.biWidth; cursor++)
            {
                // write RGB triple to outfile
                fwrite(&scan[cursor], sizeof (RGBTRIPLE), 1, outptr);
            }

            // write padding to outfile
            for (int k = 0; k < padding; k++)
                fputc(0x00, outptr);
        }

        // skip over padding, if any
        fseek(inptr, oldPadding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}

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

	f = fopen(filename,"w+");
    if (f == NULL)
    {
        printf("Could not write to %s.\n", filename);
        return ;
    }
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

byte* enlargeImage(int orgLenX, int orgLenY, int finalLenX, int finalLenY, byte* rdata, byte* gdata, byte* bdata){

	    int triSize = 3 * finalLenX * finalLenY;
	    byte* finalData = malloc(sizeof(byte)*triSize); // allocate 3 bytes per pixel

	    for (int i = 0; i < triSize; ++i) {
	        finalData[i] = 0;
	    }

	    int p = 0;
	    byte* intermData =  malloc(sizeof(byte)*3*orgLenY*finalLenX);

	    // iterate over infile's scanlines
	    for (int i = 0; i < orgLenY; i++)
	    {
	        // iterate over pixels in scanline
	        for (int j = 0; j < orgLenX; j++)
	        {
	            for (int k = 0; k < finalLenX/orgLenX; k++)
	            {
	            	*(intermData+p) = *(rdata+i*orgLenX+j);
	            	p++;
	            	*(intermData+p) = *(gdata+i*orgLenX+j);
	            	p++;
	            	*(intermData+p) = *(bdata+i*orgLenX+j);
	            	p++;
	            }
	        }
	        // now I have read the entire line
	    }

        for (int j = 0; j < finalLenX; j++){
        	for (int i = 0; i < orgLenY; i++){
	            for (int k = 0; k < finalLenY/orgLenY; k++){
	            	*(finalData+3*k*finalLenX+3*i*finalLenY/orgLenY*finalLenX+3*j) = *(intermData+3*i*finalLenX+3*j);
	            	*(finalData+3*k*finalLenX+3*i*finalLenY/orgLenY*finalLenX+3*j+1) = *(intermData+3*i*finalLenX+3*j+1);
	            	*(finalData+3*k*finalLenX+3*i*finalLenY/orgLenY*finalLenX+3*j+2) = *(intermData+3*i*finalLenX+3*j+2);
	            }
	        }
	    }

        free(intermData);
        return finalData;

}
