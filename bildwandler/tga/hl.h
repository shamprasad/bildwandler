/* definitions and macros for HF-Lab     January 1996  John P. Beale */

#define VERSION_NUMBER "v0.90Beta 26JUL1997 "

#define GAUFAC 4.0              /* sigmas out along gaussian */

#define FALSE 0
#define TRUE 1

#define D double
#define U unsigned int
#define PTYPE float   /* data-type of heightfield (pixel) values */ 
#define Boolean int

#undef tolower          /* annoying detail for ANSI-compatibility */
			   
typedef struct {        /* Heightfield structure type */
   unsigned char *r;
unsigned char *g;
unsigned char *b;          /* 2-D array of values */
   U xsize;
   U ysize;             /* x- and y-dimensions of array */
   PTYPE min;
   PTYPE max;         /* max and min values in array */
   int c;             /* TRUE if matrix is complex, FALSE if real */
   char name[15];     /* name of HF */
 } hfield;

typedef struct Point3 {
 double x, y, z;                   /* three-dimensional point */
} PT3;

int h_newr(hfield *hfp, int xs, int ys, char *name); /* create real HF */


#define El(vv, xq, yq)  vv[((yq) * xsize) + xq]  /* elmt of array */

