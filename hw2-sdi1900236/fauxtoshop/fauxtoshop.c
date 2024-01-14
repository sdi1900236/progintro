#include<stdio.h>
#include<stdlib.h>

int main ()
{
  unsigned char ch, header[100], **pixel;
  int i, j;
  int padding = 0, file_size = 0, width = 0, length = 0, picture_size = 0;

  /* Read header */
  /*  Initialize header[10] to a high value for the loop to work, then read header[10] from the input */
  header[10] = 100;
  for ( i = 0; i < header[10]; i++ )
    scanf("%c", header + i );

  /* Check header validity */
  if ( ( header[0] != 'B' ) || ( header[1] != 'M' ) )
  {
    printf("Error:not a BMP file\n");
    exit ( 1 );
  }
  if ( header[28] != 24 )
  {
    printf("Error:not a color BMP file\n");
    exit ( 1 );
  }
  if ( header[10] < 54 )
  {
    printf("Error: header of BMP file contains < 54 bytes\n");
    exit ( 1 );
  }
 
  /* Read width */
  for ( i = 18; i < 22; i++ )
    width += header[i] << ( ( i - 18 ) * 8 );

  /* Read length */
  for ( i = 22; i < 26; i++ )
    length += header[i] << ( ( i - 22 ) * 8 ); 

  /* Allocate memory for pixels */
  pixel = malloc ( length * sizeof ( unsigned char * ) );
    for ( i = 0; i < length; i++ )
      pixel[i] = malloc ( 3 * width * sizeof ( unsigned char ) );

  /* Read picture size */
  for ( i = 34; i < 38; i++ )
    picture_size += header[i] << ( ( i - 34 ) * 8 );

  /* Specify padding based on the value of width */
  padding = ( 4 - ( ( 3 * width ) % 4 ) ) % 4;

  /* Read data */
  for ( i = 0; i < length; i++ )
  {
    int k;
    for ( j = 0; j < width * 3; j++ )
      scanf("%c", &(pixel[i][j]) );

    /* Read any padding zeros */
    for ( k = 0; k < padding; k++ )
      scanf("%c", &ch );
  }

  /* Check if the image size corresponds its width and length*/
  if ( ( 3 * width + padding ) * length != picture_size )
  {
    printf("Error: BMP file does not have the width/length or padding size\n");
    exit ( 1 );
  }

  /* Read filesize */
  for ( i = 2; i < 6; i++ )
    file_size += header[i] << ( ( i - 2 ) * 8 );

  /* Check if file size is correct */
  if ( ( file_size != ( 3 * width + padding ) * length + header[10] ) )
  {
    printf("Error: BMP file does not have the file size\n");
    exit ( 1 );
  }


  /* Swap width/length */
  for ( i = 18; i < 22; i++ )
  {
    unsigned char ch;

    ch = header[i];
    header[i] = header[i + 4];
    header[i + 4] = ch;
  }

  /* Specify padding based on the value of length, which is now width */
  padding = ( 4 - ( ( 3 * length ) % 4 ) ) % 4;

  /* Change picture size */
  picture_size = ( 3 * length + padding ) * width;
  for ( i = 34; i < 38; i++ )
    header[i] = ( picture_size >> 8 * ( i - 34 ) ) & 255;

  /* Change file size */
  file_size = picture_size + header[10];
  for ( i = 2; i < 6; i++ )
    header[i] = ( file_size >> 8 * ( i - 2 ) ) & 255;

  /* Write header */
  for ( i = 0; i < header[10]; i++ )
    putchar ( header[i] );

  ch = 0;
  /* Write data */
  for ( i = 0; i < width; i += 1 )
  {
    int k;

    for ( j = 0; j < length; j++ )
    {
      /* Writing red blue green (3 bytes) in [j][width-i] format */
      putchar( pixel[j][3 * ( width - i - 1 )] );
      putchar( pixel[j][3 * ( width - i - 1 ) + 1] );
      putchar( pixel[j][3 * ( width - i - 1 ) + 2] );
    }

    /* Add padding if any */
    for ( k = 0; k < padding; k++ )
      putchar ( ch );
  }

  /* Free allocated memory for pixels */
  for ( i = 0; i < length; i++ )
    free ( pixel[i] );
  free ( pixel );

  exit(0);
}
