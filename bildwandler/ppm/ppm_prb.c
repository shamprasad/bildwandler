#include<stdio.h> 
#include<stdlib.h>
#include "ppm_io.h"

int 
main (int argc, char *argv[]) 
{
  
 
 
PPMImage * image;
  
 
if (argc != 3)
    
 
printf ("usage: ./a.out <inputfile> <outputfile>\n");
  
 
  else
    
 
    {
      
 
image = readPPM (argv[1]);
      
 
 
changeColorPPM (image);
      
 
 
writePPM (argv[2], image);
      
 
 

      
 
 
//getchar ();
    }

 
}


