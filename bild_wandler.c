#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
# include <unistd.h>
# include <fcntl.h>
#include "tiff/tif_config.h"
#include "ppm/ppm_io.h"
#include "gif1/gd.h"
#include "bmp/bmp_io.h"
#include "tga/hl.h"
void func_53(char *s1, char *s2,int grayscale)
{
char  string[100];
PPMImage *image;
PPMImagebw *imagebw;
sprintf(string,"jpeg-9/./djpeg -dct int -ppm -outfile %s %s",s2,s1);
system(string);
if(grayscale == 1){
image = readPPM(s2);
remove(s2);
imagebw = converttobw(image);
  writePPMbw (s2, imagebw);
}


}
void func_35(char *s1, char *s2,int grayscale)
{
char  string[100];
char buf[3];
PPMImage *image;
PPMImagebw *imagebw;
FILE *fp;
 fp = fopen(s1,"rb");
fgets (buf, sizeof (buf), fp);
fclose(fp);
if(buf[1] == '5')
{
imagebw = readPPMbw(s1);
remove(s1);
image = converttoppm(imagebw);
writePPM (s1, image);
free(image->data);
free(image);
}
if(grayscale == 1 && buf[1] != '5')
{
image = readPPM(s1);
converttobwppm(image);
  writePPM (s1, image);
free(image->data);
free(image);
}
sprintf(string,"jpeg-9/./cjpeg -dct int -outfile %s %s",s2,s1);
system(string);
}
void
func_43 (char *s1, char *s2,int grayscale)
{
  int *barray;
  int *garray;
  int *rarray;
  int *indexb;
  int *indexg;
  int *indexr;
  int result;
  int xsize;
  int ysize, x1, y1, i, y;

  PPMImage *image;
PPMImagebw *imagebw;
  image = (PPMImage *) malloc (sizeof (PPMImage));

  bmp_read (s1, &xsize, &ysize, &rarray, &garray, &barray);
  image->x = xsize;
  image->y = ysize;
  image->data = (PPMPixel *) malloc (image->x * image->y * sizeof (PPMPixel));
  indexr = rarray;
  indexg = garray;
  indexb = barray;

  for (y1 = 0; y1 < image->y; y1++)
    {
      y = ysize - 1 - y1;
      for (x1 = 0; x1 < image->x; x1++)
	{
	  i = y * image->x + x1;

	  image->data[i].red = *indexr;
	  image->data[i].green = *indexg;
	  image->data[i].blue = *indexb;


	  indexr = indexr + 1;
	  indexg = indexg + 1;
	  indexb = indexb + 1;
	}
    }


  if (rarray != NULL)
    {
      free (rarray);
    }

  if (garray != NULL)
    {
      free (garray);
    }

  if (barray != NULL)
    {
      free (barray);
    }
  writePPM (s2, image);
  free (image->data);
  free (image);
if(grayscale == 1){
image = readPPM(s2);
remove(s2);
imagebw = converttobw(image);
  writePPMbw (s2, imagebw);
}
}
int func_32(char *s1, char *s2,int grayscale)
{
char string[100],buf[3];
PPMImage *image;
PPMImagebw *imagebw;
FILE *fp;
 fp = fopen(s1,"rb");
fgets (buf, sizeof (buf), fp);
fclose(fp);
if(buf[1] == '5')
{
imagebw = readPPMbw(s1);
remove(s1);
image = converttoppm(imagebw);
writePPM (s1, image);
free(image->data);
free(image);
}
if(grayscale == 1 && buf[1] != '5')
{
image = readPPM(s1);
converttobwppm(image);
  writePPM (s1, image);
free(image->data);
free(image);
}
sprintf(string,"gif1/./ppmquant 256 %s > ogre",s1);
system(string);
sprintf(string,"gif1/./ppmtogif ogre > %s",s2);
system(string);
remove("ogre");
}
int
func_34 (char *s1, char *s2,int grayscale)
{
  int *barray;
  int *garray;
  int i;
  int i2;
  int *indexb;
  int *indexg;
  int *indexr;
  int j;
  int j2;
  int numbytes;
  int *rarray;
  int result;
  int xsize;
  int ysize;
  PPMImage *image;
PPMImagebw *imagebw;
char buf[3];
FILE *fp;
 fp = fopen(s1,"rb");
fgets (buf, sizeof (buf), fp);
fclose(fp);
if(buf[1] == '5')
{
imagebw = readPPMbw(s1);
remove(s1);
image = converttoppm(imagebw);
writePPM (s1, image);
free(image->data);
free(image);
}
if(grayscale == 1 && buf[1] != '5')
{
image = readPPM(s1);
converttobwppm(image);
  writePPM (s1, image);
free(image->data);
free(image);
}
  image = readPPM (s1);
  xsize = image->x;
  ysize = image->y;
rarray = NULL;
  garray = NULL;
  barray = NULL;
  numbytes = xsize * ysize * sizeof (int);

  rarray = (int *) malloc (numbytes);

  if (rarray == NULL)
    {
      printf ("\n");
      printf ("BMP_WRITE_TEST: Fatal error!\n");
      printf ("  Unable to allocate memory for data.\n");
      return 1;
    }

  garray = (int *) malloc (numbytes);

  if (garray == NULL)
    {
      printf ("\n");
      printf ("BMP_WRITE_TEST: Fatal error!\n");
      printf ("  Unable to allocate memory for data.\n");
      return 1;
    }

  barray = (int *) malloc (numbytes);

  if (barray == NULL)
    {
      printf ("\n");
      printf ("BMP_WRITE_TEST: Fatal error!\n");
      printf ("  Unable to allocate memory for data.\n");
      return 1;
    }
/*
  Set the data.
  Note that BMP files go from "bottom" up, so we'll reverse the
  sense of "J" here to get what we want.
*/
  indexr = rarray;
  indexg = garray;
  indexb = barray;

  for (j2 = 0; j2 < ysize; j2++)
    {
      j = ysize - 1 - j2;
      for (i = 0; i < xsize; i++)
	{
	  i2 = j * xsize + i;
	  *indexr = image->data[i2].red;
	  *indexg = image->data[i2].green;
	  *indexb = image->data[i2].blue;

	  indexr = indexr + 1;
	  indexg = indexg + 1;
	  indexb = indexb + 1;
	}
    }
/*
  Write the data to a file.
*/
  result = bmp_write (s2, xsize, ysize, rarray, garray, barray);
/*
  Free the memory.
*/
  if (rarray != NULL)
    {
      free (rarray);
    }

  if (garray != NULL)
    {
      free (garray);
    }

  if (barray != NULL)
    {
      free (barray);
    }

  if (result != 0)
    {
      printf ("\n");
      printf ("BMP_WRITE_TEST: Fatal error!\n");
      printf ("  BMP_WRITE failed.\n");
      return 1;
    }

  return 0;
}
void func_37(char *s1, char *s2,int grayscale)
{
int iy,ix,j, xsize;
hfield temphf;
char buf[3];
  PPMImage *image;
PPMImagebw *imagebw;
FILE *fp;
 fp = fopen(s1,"rb");
fgets (buf, sizeof (buf), fp);
fclose(fp);
if(buf[1] == '5')
{
imagebw = readPPMbw(s1);
remove(s1);
image = converttoppm(imagebw);
writePPM (s1, image);
free(image->data);
free(image);
}
if(grayscale == 1 && buf[1] != '5')
{
image = readPPM(s1);
converttobwppm(image);
  writePPM (s1, image);
free(image->data);
free(image);
}
  image = readPPM (s1);
xsize = image->x;
h_newr(&temphf,image->x,image->y,s2);
for (iy = 0; iy < image->y; iy++) {
j = iy*image->x;
    for (ix = 0; ix < image->x; ix++) {
El(temphf.r,ix,iy) =  image->data[j+ix].red;
El(temphf.g,ix,iy) =  image->data[j+ix].green;
El(temphf.b,ix,iy) =  image->data[j+ix].blue;
}
}
tga_write(temphf,s2);
free(temphf.r);
free(temphf.g);
free(temphf.b);

}
void func_73(char *s1, char *s2,int grayscale)
{
hfield temphf;
int iy,ix,j,xsize;
  PPMImage *image;
PPMImagebw *imagebw;
image = (PPMImage *) malloc (sizeof (PPMImage));
tga_read(&temphf,s1);
xsize = temphf.xsize;
 image->x = temphf.xsize;
  image->y = temphf.ysize;
  image->data = (PPMPixel *) malloc (image->x * image->y * sizeof (PPMPixel));
for (iy = 0; iy < image->y; iy++) {
j = iy*image->x;
    for (ix = 0; ix < image->x; ix++) {
image->data[j+ix].red = El(temphf.r,ix,iy) ;
image->data[j+ix].green = El(temphf.g,ix,iy) ;
image->data[j+ix].blue = El(temphf.b,ix,iy) ;
}
}
writePPM (s2, image);
  free (image->data);
  free (image);
free(temphf.r);
free(temphf.g);
free(temphf.b);
if(grayscale == 1){
image = readPPM(s2);
remove(s2);
imagebw = converttobw(image);
  writePPMbw (s2, imagebw);
}

}
void
func_23 (char *s1, char *s2,int grayscale)
{
  long int x1;
  long int xs;
  long int y1;
  long int ys;
  int i, c;
  PPMImage *image;
PPMImagebw *imagebw;
  gdImagePtr im;
  FILE *in;
  in = fopen (s1, "rb");
  if (in == NULL)
    {
      printf ("file not found\n");
      exit (0);

    }
  im = gdImageCreateFromGif (in);
  fclose (in);
  image = (PPMImage *) malloc (sizeof (PPMImage));
  image->x = gdImageSX (im);
  image->y = gdImageSY (im);
  image->data = (PPMPixel *) malloc (image->x * image->y * sizeof (PPMPixel));
  for (y1 = 0; y1 < image->y; y1++)
    {
      for (x1 = 0; x1 < image->x; x1++)
	{
	  i = y1 * image->x + x1;
	  c = gdImageGetPixel (im, x1, y1);
	  image->data[i].red = im->red[c];
	  image->data[i].green = im->green[c];
	  image->data[i].blue = im->blue[c];
	}
    }
  writePPM (s2, image);
  free (image->data);
  free (image);
gdImageDestroy(im);
if(grayscale == 1){
image = readPPM(s2);
remove(s2);
imagebw = converttobw(image);
  writePPMbw (s2, imagebw);
}
}
void func_13(char *s1,char *s2,int grayscale)
{
PPMImage *image;
PPMImagebw *imagebw;
tifftoppm(s1,s2);
if(grayscale == 1){
image = readPPM(s2);
remove(s2);
imagebw = converttobw(image);
  writePPMbw (s2, imagebw);
}

}
void
func_31 (char *s1, char *s2,int grayscale)
{
  char buf[3];
PPMImage *image;
PPMImagebw *imagebw;
FILE *fp;
 fp = fopen(s1,"rb");
fgets (buf, sizeof (buf), fp);
fclose(fp);
if(buf[1] == '5')
{
imagebw = readPPMbw(s1);
remove(s1);
image = converttoppm(imagebw);
writePPM (s1, image);
free(image->data);
free(image);
}
if(grayscale == 1 && buf[1] != '5')
{
image = readPPM(s1);
converttobwppm(image);
  writePPM (s1, image);
free(image->data);
free(image);
}
ppm2tiff(s1,s2);


}
const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}
int convid(char *s1,char *s2)
{
int conv_id;
if(!strcmp(get_filename_ext(s1),"tiff")) 
conv_id = 1;
if(!strcmp(get_filename_ext(s1),"tif")) 
conv_id = 1;
if(!strcmp(get_filename_ext(s1),"gif")) 
conv_id = 2;
if(!strcmp(get_filename_ext(s1),"ppm")) 
conv_id = 3;
if(!strcmp(get_filename_ext(s1),"bmp")) 
conv_id = 4;
if(!strcmp(get_filename_ext(s1),"jpg")) 
conv_id = 5;
if(!strcmp(get_filename_ext(s1),"png")) 
conv_id = 6;
if(!strcmp(get_filename_ext(s1),"tga")) 
conv_id = 7;
if(!strcmp(get_filename_ext(s2),"tiff")) 
conv_id = conv_id*10+1;
if(!strcmp(get_filename_ext(s2),"tif")) 
conv_id = conv_id*10+1;
if(!strcmp(get_filename_ext(s2),"gif")) 
conv_id = conv_id*10+2;
if(!strcmp(get_filename_ext(s2),"ppm")) 
conv_id = conv_id*10+3;
if(!strcmp(get_filename_ext(s2),"bmp")) 
conv_id = conv_id*10+4;
if(!strcmp(get_filename_ext(s2),"jpg")) 
conv_id = conv_id*10+5;
if(!strcmp(get_filename_ext(s2),"png")) 
conv_id = conv_id*10+6;
if(!strcmp(get_filename_ext(s2),"tga")) 
conv_id = conv_id*10+7;
return conv_id;
}
int
main (int argc, char *argv[])
{
  int conv_id = 25,tr,grayscale=0;
  if (argc != 4)
    {
      printf ("usage : bildWandler <inputfile> <outputfile> <grayscale>\n");
      exit (0);

    }
  grayscale = atoi(argv[3]);
conv_id = convid(argv[1],argv[2]);
tr = conv_id/10;
if(conv_id % 10 == tr)
{
rename(argv[1],argv[2]);
}
else{
  switch (conv_id)
    {
/**tiff*/
case 12: 
	func_13(argv[1],"temp.ppm",grayscale);	//tiff to ppm
	func_32("temp.ppm",argv[2],grayscale);	//ppm to gif
	remove ("temp.ppm");
	break;
case 21:
      func_23 (argv[1], "temp.ppm",grayscale);	//gif to ppm
      func_31 ("temp.ppm", argv[2],grayscale);	//ppm to tiff
      remove ("temp.ppm");

      break;

case 13: 
func_13(argv[1],argv[2],grayscale);		//tiff to ppm

break;
    case 31:
      func_31 (argv[1], argv[2],grayscale);	//ppm to tiff
      break;
case 14:
      func_13 (argv[1], "temp.ppm",grayscale);	//tiff to ppm
      func_34 ("temp.ppm", argv[2],grayscale);	//ppm to bmp
      remove ("temp.ppm");

      break;
case 41:
      func_43 (argv[1], "temp.ppm",grayscale);	//bmp to ppm
      func_31 ("temp.ppm", argv[2],grayscale);	//ppm to tiff
      remove ("temp.ppm");
      break;
case 15:
      func_13 (argv[1], "temp.ppm",grayscale);	//tiff to ppm
      func_35 ("temp.ppm", argv[2],grayscale);	//ppm to jpeg
      remove ("temp.ppm");

      break;
case 51:
      func_53 (argv[1], "temp.ppm",grayscale);	//jpg to ppm
      func_31 ("temp.ppm", argv[2],grayscale);	//ppm to tiff
      remove ("temp.ppm");

      break;
case 17:
      func_13 (argv[1], "temp.ppm",grayscale);	//tiff to ppm
      func_37 ("temp.ppm", argv[2],grayscale);	//ppm to tga
      remove ("temp.ppm");

      break;
case 71:
      func_73 (argv[1], "temp.ppm",grayscale);	//tga to ppm
      func_31 ("temp.ppm", argv[2],grayscale);	//ppm to tiff
      remove ("temp.ppm");

      break;

/***gif**/
    case 23:
      func_23 (argv[1], argv[2],grayscale);	//gif to ppm
      break;
case 32:
   func_32(argv[1],argv[2],grayscale);		//ppm to gif
break;
case 24:
      func_23 (argv[1], "temp.ppm",grayscale);	//gif to ppm
      func_34 ("temp.ppm", argv[2],grayscale);	//ppm to bmp
      remove ("temp.ppm");
      break;   
	
	case 42: 
	func_43(argv[1],"temp.ppm",grayscale);	//bmp to ppm
	func_32("temp.ppm",argv[2],grayscale);	//ppm to gif
	remove ("temp.ppm");
	break;
case 25:
    func_23(argv[1],"temp.ppm",grayscale);   //gif to ppm
    func_35("temp.ppm",argv[2],grayscale);  // ppm to jpg
    remove("temp.ppm");
break;
case 52: 
	func_53(argv[1],"temp.ppm",grayscale);	//jpg to ppm
	func_32("temp.ppm",argv[2],grayscale);	//ppm to gif
	remove ("temp.ppm");
	break;
case 27:
      func_23 (argv[1], "temp.ppm",grayscale);	//gif to ppm
      func_37 ("temp.ppm", argv[2],grayscale);	//ppm to tga
      remove ("temp.ppm");

      break;
case 72:
      func_73 (argv[1], "temp.ppm",grayscale);	//tga to ppm
      func_32 ("temp.ppm", argv[2],grayscale);	//ppm to gif
      remove ("temp.ppm");

      break;
/***bmp***/
    case 34:
      func_34 (argv[1], argv[2],grayscale);	//ppm to bmp

      break;
    case 43:
      func_43 (argv[1], argv[2],grayscale);	//bmp to ppm

      break;

case 45:
    func_43(argv[1],"temp.ppm",grayscale);   //bmp to ppm
    func_35("temp.ppm",argv[2],grayscale);  // ppm to jpg
    remove("temp.ppm");
break;
    case 54:
    func_53(argv[1],"temp.ppm",grayscale);   //jpg to ppm
    func_34("temp.ppm",argv[2],grayscale);  // ppm to bmp
    remove("temp.ppm");
break;
case 47:
      func_43 (argv[1], "temp.ppm",grayscale);	//bmp to ppm
      func_37 ("temp.ppm", argv[2],grayscale);	//ppm to tga
      remove ("temp.ppm");

      break;
case 74:
      func_73 (argv[1], "temp.ppm",grayscale);	//tga to ppm
      func_34 ("temp.ppm", argv[2],grayscale);	//ppm to bmp
      remove ("temp.ppm");

      break;
	/**jpg**/
 
 
    case 53:
     func_53(argv[1],argv[2],grayscale);     //jpg to ppm
	break;
	case 35:
    func_35(argv[1],argv[2],grayscale);     //ppm to jpg
   break;
	case 57:
      func_53 (argv[1], "temp.ppm",grayscale);	//jpg to ppm
      func_37 ("temp.ppm", argv[2],grayscale);	//ppm to tga
      remove ("temp.ppm");

      break;
case 75:
      func_73 (argv[1], "temp.ppm",grayscale);	//tga to ppm
      func_35 ("temp.ppm", argv[2],grayscale);	//ppm to jpg
      remove ("temp.ppm");

      break;
/***tga***/
case 37:
func_37(argv[1],argv[2],grayscale);
break;
case 73:
func_73(argv[1],argv[2],grayscale);
break;


    default:
      printf ("Some Error\n");
    }
}
}
