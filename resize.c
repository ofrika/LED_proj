/**
 * resize.c part of bmp.h by Iliyan Stankov
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int resize(int factor, const char *in, const char *out)
{
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