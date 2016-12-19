#include <stdio.h> 
#include <stdlib.h>
#include "bmp_io.h"

int main ( long argc, char **argv );

/******************************************************************************/

int main ( long argc, char **argv ) {

/******************************************************************************/

  int result;

  printf ( "\n" );
  printf ( "BMP_IO_PRB\n" );
  printf ( "  Simple tests of BMP_IO.\n" );

  result = bmp_write_test ( "test.bmp" );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "BMP_WRITE_TEST failed.\n" );
    return EXIT_FAILURE;
  }

  printf ( "\n" );
  printf ( "BMP_WRITE_TEST passed.\n" );

  result = bmp_read_test ( "test.bmp" );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "BMP_READ_TEST failed.\n" );
    return EXIT_FAILURE;
  }

  printf ( "\n" );
  printf ( "BMP_READ_TEST passed.\n" );

  printf ( "\n" );
  printf ( "BMP_IO_PRB:\n" );
  printf ( "  Normal end of execution.\n" );

  return EXIT_SUCCESS;
}
