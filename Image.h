//
// Created by Anabusy on 9/29/2019.
//

typedef struct image_t *Image;

Image createImage(int id, int width, int height, RGB** colorArr);
void destroyImage(Image img);
int getImageId(Image img);
int getImageWidth(Image img);
int getImageHeight(Image img);
RGB** getImageRGB(Image img);