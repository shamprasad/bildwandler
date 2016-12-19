#ifndef GD_H
#define GD_H 1


#include <stdio.h>

/* This can't be changed, it's part of the GIF specification. */

#define gdMaxColors 256

/* Image type. See functions below; you will not need to change
	the elements directly. Use the provided macros to
	access sx, sy, the color table, and colorsTotal for 
	read-only purposes. */

typedef struct gdImageStruct {
	unsigned char ** pixels;
	int sx;
	int sy;
	int colorsTotal;
	int red[gdMaxColors];
	int green[gdMaxColors];
	int blue[gdMaxColors]; 
	int open[gdMaxColors];
	int transparent;
	int *polyInts;
	int polyAllocated;
	struct gdImageStruct *brush;
	struct gdImageStruct *tile;	
	int brushColorMap[gdMaxColors];
	int tileColorMap[gdMaxColors];
	int styleLength;
	int stylePos;
	int *style;
	int interlace;
} gdImage;

typedef gdImage * gdImagePtr;


gdImagePtr gdImageCreate(int sx, int sy);
gdImagePtr gdImageCreateFromGif(FILE *fd);
void gdImageDestroy(gdImagePtr im);
void gdImageSetPixel(gdImagePtr im, int x, int y, int color);
int gdImageGetPixel(gdImagePtr im, int x, int y);

int gdImageBoundsSafe(gdImagePtr im, int x, int y);

int gdImageColorAllocate(gdImagePtr im, int r, int g, int b);
int gdImageColorClosest(gdImagePtr im, int r, int g, int b);
int gdImageColorExact(gdImagePtr im, int r, int g, int b);
void gdImageColorDeallocate(gdImagePtr im, int color);
void gdImageColorTransparent(gdImagePtr im, int color);
void gdImageGif(gdImagePtr im, FILE *out);
void gdImageGd(gdImagePtr im, FILE *out);

/* Macros to access information about images. READ ONLY. Changing
	these values will NOT have the desired result. */
#define gdImageSX(im) ((im)->sx)
#define gdImageSY(im) ((im)->sy)
#define gdImageColorsTotal(im) ((im)->colorsTotal)
#define gdImageRed(im, c) ((im)->red[(c)])
#define gdImageGreen(im, c) ((im)->green[(c)])
#define gdImageBlue(im, c) ((im)->blue[(c)])
#define gdImageGetTransparent(im) ((im)->transparent)
#define gdImageGetInterlaced(im) ((im)->interlace)
#endif
