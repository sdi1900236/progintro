#include<stdio.h>
#include<stdlib.h>

int main ( int argc, char **argv )
{
  FILE * dna1file, * dna2file;
  unsigned char dna1[100000], dna2[100000];
  int i = 0, j, k = 0, maxlength = 0, start = 0, file1size, file2size;

  /* Check if enough arguments are given to the program */
  if ( argc < 3 )
  {
    printf("Too few agrguments\n");
    exit ( 1 );
  }

  /* Open dna file1 for reading */
  if ( ( dna1file = fopen ( argv[1], "r" ) ) == NULL )
  {
    printf("Error opening file %s for dna1 reading\n", argv[1] );
    exit ( 1 );
  }
  /* Open dna file2 for reading */
  if ( ( dna2file = fopen ( argv[2], "r" ) ) == NULL )
  {
    printf("Error opening file %s for dna2 reading\n", argv[2] );
    exit ( 1 );
  }

  /* Read dna1 file data into array dna1[] */
  while ( fscanf( dna1file ,"%c", dna1 + i ) != -1 )
  /* Check that each symbol belongs to the known 4-letter alphabet */
    if ( ( dna1[i]  == 'A' ) || ( dna1[i]  == 'G' )  || ( dna1[i]  == 'T' ) || ( dna1[i]  == 'C' ) ) i++;

  /* Determine size of file dna1[] */
  file1size = i - 1;

  /* Close dna1 file */
  fclose ( dna1file );

  /* Read dna2 file data into array dna2[] */
  i = 0;
  while ( fscanf( dna2file ,"%c", dna2 + i ) != -1 )
  /* Check thateach symbol belongs to the known 4-letter alphabet */
    if ( ( dna2[i]  == 'A' ) || ( dna2[i]  == 'G' )  || ( dna2[i]  == 'T' ) || ( dna2[i]  == 'C' ) ) i++;

  /* Determine size of file dna2 */
  file2size = i - 1;

  /* Close dna2 file */
  fclose ( dna2file );

  /* DNA match loop, check dna1[i + k] against dna2[j + k] for each i in the dna1 domain and each j in the dna2 domain.
  k is the offset index in the matched part */
  for ( i = 0; i < file1size; i++ )
    for ( j = 0; j < file2size - maxlength; j++ )
    {
      /* Check maximum common part (k sweep) for given  i, j */
      for ( k = 0; ( k < file2size - j ) && ( i + k < file1size ) && ( dna1[i + k] == dna2[j + k] ); k++ );

      /* If matched part of i, j has bigger length than previously found maximum common part (maxlength) then replace maxlength with k and change starting point to i */
      if ( k > maxlength )
      {
        maxlength = k;
	start = i;
      }
    }

  /* Print maximum matched part */
  for ( i = start; i < start + maxlength; i++ )
    printf("%c", dna1[i] );
  printf("\n");

  exit ( 0 );
}
