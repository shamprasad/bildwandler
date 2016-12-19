/* $Id: ppm2tiff.c,v 1.16 2010-04-10 19:22:34 bfriesen Exp $ */

/*
 * Copyright (c) 1991-1997 Sam Leffler
 * Copyright (c) 1991-1997 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Sam Leffler and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Sam Leffler and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 * IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

#include "tif_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#ifdef HAVE_IO_H
# include <io.h>
#endif

#ifdef NEED_LIBPORT
# include "libport.h"
#endif

#include "tiffio.h"

#define	streq(a,b)	(strcmp(a,b) == 0)
#define	strneq(a,b,n)	(strncmp(a,b,n) == 0)

static	uint16 compression = COMPRESSION_NONE;
static	uint16 predictor = 0;
static	int quality = 75;	/* JPEG quality */
static	int jpegcolormode = JPEGCOLORMODE_RGB;
static  uint32 g3opts;

static	int processCompressOptions(char*);

static void
BadPPM(char* file)
{
	fprintf(stderr, "%s: Not a PPM file.\n", file);
	exit(-2);
}

int
ppm2tiff(char *infile,char *s2)
{
	uint16 photometric = 0;
	uint32 rowsperstrip = (uint32) -1;
	double resolution = -1;
	unsigned char *buf = NULL;
	tsize_t linebytes = 0;
	uint16 spp = 1;
	uint16 bpp = 8;
	TIFF *out;
	FILE *in;
	unsigned int w, h, prec, row;
	//char *infile;
	int c;
	

	
		in = fopen(infile, "rb");
		if (in == NULL) {
			fprintf(stderr, "%s: Can not open.\n", infile);
			return (-1);}
		
	if (fgetc(in) != 'P')
		BadPPM(infile);
	switch (fgetc(in)) {
		case '4':			/* it's a PBM file */
			bpp = 1;
			spp = 1;
			photometric = PHOTOMETRIC_MINISWHITE;
			break;
		case '5':			/* it's a PGM file */
			bpp = 8;
			spp = 1;
			photometric = PHOTOMETRIC_MINISBLACK;
			break;
		case '6':			/* it's a PPM file */
			bpp = 8;
			spp = 3;
			photometric = PHOTOMETRIC_RGB;
			if (compression == COMPRESSION_JPEG &&
			    jpegcolormode == JPEGCOLORMODE_RGB)
				photometric = PHOTOMETRIC_YCBCR;
			break;
		default:
			BadPPM(infile);
	}

	/* Parse header */
	while(1) {
		if (feof(in))
			BadPPM(infile);
		c = fgetc(in);
		/* Skip whitespaces (blanks, TABs, CRs, LFs) */
		if (strchr(" \t\r\n", c))
			continue;

		/* Check for comment line */
		if (c == '#') {
			do {
			    c = fgetc(in);
			} while(!(strchr("\r\n", c) || feof(in)));
			continue;
		}

		ungetc(c, in);
		break;
	}
	switch (bpp) {
	case 1:
		if (fscanf(in, " %u %u", &w, &h) != 2)
			BadPPM(infile);
		if (fgetc(in) != '\n')
			BadPPM(infile);
		break;
	case 8:
		if (fscanf(in, " %u %u %u", &w, &h, &prec) != 3)
			BadPPM(infile);
		if (fgetc(in) != '\n' || prec != 255)
			BadPPM(infile);
		break;
	}
	out = TIFFOpen(s2, "w");
	if (out == NULL)
		return (-4);
	TIFFSetField(out, TIFFTAG_IMAGEWIDTH, (uint32) w);
	TIFFSetField(out, TIFFTAG_IMAGELENGTH, (uint32) h);
	TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, spp);
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, bpp);
	TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, photometric);
	TIFFSetField(out, TIFFTAG_COMPRESSION, compression);
	switch (compression) {
	case COMPRESSION_JPEG:
		TIFFSetField(out, TIFFTAG_JPEGQUALITY, quality);
		TIFFSetField(out, TIFFTAG_JPEGCOLORMODE, jpegcolormode);
		break;
	case COMPRESSION_LZW:
	case COMPRESSION_DEFLATE:
		if (predictor != 0)
			TIFFSetField(out, TIFFTAG_PREDICTOR, predictor);
		break;
        case COMPRESSION_CCITTFAX3:
		TIFFSetField(out, TIFFTAG_GROUP3OPTIONS, g3opts);
		break;
	}
	switch (bpp) {
		case 1:
			linebytes = (spp * w + (8 - 1)) / 8;
			if (rowsperstrip == (uint32) -1) {
				TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, h);
			} else {
				TIFFSetField(out, TIFFTAG_ROWSPERSTRIP,
				    TIFFDefaultStripSize(out, rowsperstrip));
			}
			break;
		case 8:
			linebytes = spp * w;
			TIFFSetField(out, TIFFTAG_ROWSPERSTRIP,
			    TIFFDefaultStripSize(out, rowsperstrip));
			break;
	}
	if (TIFFScanlineSize(out) > linebytes)
		buf = (unsigned char *)_TIFFmalloc(linebytes);
	else
		buf = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(out));
	if (resolution > 0) {
		TIFFSetField(out, TIFFTAG_XRESOLUTION, resolution);
		TIFFSetField(out, TIFFTAG_YRESOLUTION, resolution);
		TIFFSetField(out, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
	}
	for (row = 0; row < h; row++) {
		if (fread(buf, linebytes, 1, in) != 1) {
			fprintf(stderr, "%s: scanline %lu: Read error.\n",
			    infile, (unsigned long) row);
			break;
		}
		if (TIFFWriteScanline(out, buf, row, 0) < 0)
			break;
	}
	(void) TIFFClose(out);
	if (buf)
		_TIFFfree(buf);
	return (0);
}
int tifftoppm(char *s1,char *s2)
{
int width,height,i,x,y;
uint32 npixels,* raster;
unsigned char *buffer;
TIFF *tif=TIFFOpen(s1, "r");
FILE * fp;
fp = fopen (s2, "wb");
if (!fp) 
    {
fprintf (stderr, "Unable to open file '%s'\n", s2);  
exit (1);      
}
TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);           // uint32 width;
TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height); 
npixels=width*height;
buffer = (unsigned char *)malloc(npixels*3);
if(!buffer){printf("error allcating memory\n"); return 0;}
raster=(uint32 *) _TIFFmalloc(npixels *sizeof(uint32));
if(!raster){printf("error allcating memory\n"); return 0;}
TIFFReadRGBAImage(tif, width, height, raster, 0);
for(y = 0; y < height; y++)
{
for(x = 0; x < width; x++)
  {
i = width*(height - y -1) + x;
buffer[i*3 + 0] = TIFFGetR(raster[width*y+x]);//(unsigned char)((raster[i] && 0xff000000) >> 24);
buffer[i*3 + 1] = TIFFGetG(raster[width*y+x]);//(unsigned char)((raster[i] && 0x00ff0000) >> 16);
buffer[i*3 + 2] = TIFFGetB(raster[width*y+x]);//(unsigned char)((raster[i] && 0x0000ff00) >> 8);
}
}
fprintf (fp, "P6\n");
fprintf (fp, "# Created by %s\n", "ShamPrasad");
fprintf (fp, "%d %d\n", width, height);
fprintf (fp, "%d\n", 255);
fwrite(buffer,1,npixels*3,fp);
fclose (fp);
_TIFFfree(raster);
free(buffer);
TIFFClose(tif);
}
