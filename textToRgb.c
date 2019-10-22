/* Created By: Justin Meiners (2013) */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include "textToRgb.h"

#include "cmunrm.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_WRITE_NO_STDIO
#include "stb_image_write.h" /* http://nothings.org/stb/stb_image_write.h */

#define STB_TRUETYPE_IMPLEMENTATION 
#include "stb_truetype.h" /* http://nothings.org/stb/stb_truetype.h */


byte* rgbData(const char* filename, int w, int h) {
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

void invert_image(unsigned char* buffer, int w, int h)
{
    int N = w * h;
    for (int i = 0; i < N; ++i){
		buffer[i] = 255 - buffer[i];
	}
}

void render_text(stbtt_fontinfo* font_info,
                 unsigned char* bitmap,
                 int* w,
                 float scale,
                 const char* text,
                 size_t text_size)
                
{
    int ascent, descent, line_gap;
    stbtt_GetFontVMetrics(font_info, &ascent, &descent, &line_gap);
    
    ascent *= scale;
    descent *= scale;

    int x = 0;
    for (int i = 0; i < text_size; ++i)
    {
        if (iscntrl(text[i])) continue;

        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(font_info, text[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
        
        int y = ascent + c_y1;
        
        /* render character (stride and offset is important here) */
        if (bitmap)
        {
            int offset = x + (y * (*w));
            stbtt_MakeCodepointBitmap(font_info, bitmap + offset, c_x2 - c_x1, c_y2 - c_y1, *w, scale, scale, text[i]);
        }
       
        int ax;
        stbtt_GetCodepointHMetrics(font_info, text[i], &ax, 0);
        x += ax * scale;
        
        int kern = stbtt_GetCodepointKernAdvance(font_info, text[i], text[i + 1]);
        x += kern * scale;
    }

    *w = x;
}

unsigned char* read_font_file(const char* path)
{
    FILE* font_file = fopen("font/cmunrm.ttf", "rb");

    if (!font_file)
    {
        fprintf(stderr, "cannot find font: %s\n", path);
        exit(1);
    }

    /* get length of file */
    fseek(font_file, 0, SEEK_END);
    size_t file_size = ftell(font_file);
    fseek(font_file, 0, SEEK_SET);
    
    /* allocate buffer*/
    unsigned char* font_buffer = malloc(file_size);

    if (!font_buffer)
    {
        fprintf(stderr, "allocation failed. out of memory?");
        exit(2);
    }
   
    /* read into buffer */ 
    fread(font_buffer, file_size, 1, font_file);
    fclose(font_file);
    return font_buffer;
}

void show_usage(int help)
{
    const char* usage = "tex2image --out OUTPUT --height HEIGHT --font TTF --invert\n";

    if (help)
    {
        fputs(usage, stdout);
        fputs("--out\tspecify an output file instead of stdout.\n", stdout);
        fputs("--height\tspecify height of text in pixels.\n", stdout);
        fputs("--font\tprovide an alternative font file in TrueType format. (.ttf)\n", stdout);
        fputs("--invert\tdraw white text on black background.\n", stdout);
        fputs("--help\tshow the help text.\n", stdout);
        exit(0);
    }
    else
    {
        fputs(usage, stderr);
        exit(1);
    }
}

static void image_stdio_write(void *context, void *data, int size)
{
    fwrite(data, 1, size, (FILE*)context);
}

void text2Image(int NumOfChars, const char* tmp, int final_height)
{
    unsigned char* font_buffer = font_cmunrm_ttf;
    int line_height = final_height;
    int invert = 0;
    const char* out_path = NULL;
    const char* font_path = NULL;

	out_path = tmp;

    /* prepare font */
    stbtt_fontinfo font_info;
    if (!stbtt_InitFont(&font_info, font_buffer, 0))
    {
        fprintf(stderr, "failed to initialize font.");
        exit(2);
    }

    /* read stdin into a string.
       we have to do this, because it needs two passes
    */
    size_t text_size = 0;
    size_t text_capacity = 2048;
    char* text = malloc(text_capacity);

    if (!text)
    {
        fprintf(stderr, "allocation failed. out of memory?");
        exit(2);
    }

	int i = 0;
    while (i!=1)
    {
        if (text_size >= text_capacity)
        {
            text_capacity = text_capacity * 3 / 2;
            text = realloc(text, sizeof(char) * text_capacity);

            if (!text)
            {
                fprintf(stderr, "allocation failed. out of memory?");
                exit(2);
            }
        }

        size_t read = fread(text, sizeof(char), NumOfChars, stdin);
        text_size += read;
		i++;
    }

    /* cleanup characters */

    for (int i = 0; i < text_size; ++i)
    {
        if (text[i] == '\n')
        {
            text[i] = ' ';
        }
        else if (text[i] == '\r')
        {
            text[i] = ' ';
        }
    }

    float scale = stbtt_ScaleForPixelHeight(&font_info, line_height);

    /* first pass */
    int image_w;  
    int image_h = line_height;

    render_text(&font_info, NULL, &image_w, scale, text, text_size);

    /* second pass */
    unsigned char* bitmap = malloc(image_w * image_h);
    if (!bitmap)
    {
        fprintf(stderr, "allocation failed. out of memory?");
        exit(2);
    }

    render_text(&font_info, bitmap, &image_w, scale, text, text_size);
	
/*
    if (!invert){
		invert_image(bitmap, image_w, image_h);
    }
*/
  
    /* save out a 1 channel image */

    FILE* out_file = stdout;
    if (out_path)
    {
        out_file = fopen(out_path, "wb");
    }

    stbi_write_bmp_to_func(image_stdio_write, out_file, image_w, image_h, 1, bitmap);

    if (out_file != stdout)
    {
        fclose(out_file);
    }
    
    if (font_buffer != font_cmunrm_ttf)
    {
        free(font_buffer);
    }

    free(text);
    free(bitmap);
    
}

byte* convert_text_to_rgb(int NumTextChars, int width, int height){
	text2Image(NumTextChars,"tmp.bmp",height);
	return rgbData("tmp.bmp",  width, height);
    // triData is organized up->down, left->right
}
