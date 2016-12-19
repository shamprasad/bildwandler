#define CREATOR "yespee_4444@hotmail.com" 
#define RGB_COMPONENT_COLOR 255 

typedef struct 
{
  


unsigned char red, green, blue;
 


} PPMPixel;

 
typedef struct 
{
  


unsigned char gray;
 


} PPMPixelbw;

 
 
typedef struct 
{
  


int x, y;
   


PPMPixel * data;
 


} PPMImage;

PPMImage *
readPPM (const char *filename);
void 
writePPM (const char *filename, PPMImage * img);
void 
resizePPM (PPMImage * img,int xnew, int ynew);
