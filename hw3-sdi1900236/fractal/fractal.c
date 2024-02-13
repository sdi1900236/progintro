#include<stdio.h>
#include<stdlib.h>

#include "complexlib.h"

int NewtonRaphson ( complex * z0, float * A, float * Aderiv, int N, complex * root );
void polynomial ( float * A, int N, complex * x, complex * result );

int main ( int argc, char **argv )
{
  int i, N;
  FILE * file;
  float *A, * Aderiv, STEP, MIN_REAL, MIN_IMAG, MAX_REAL, MAX_IMAG;
  complex z0;

  /* Make sure that a filename has been given */
  if ( argc < 2 )
    exit(1);
  else if ( ( file = fopen ( argv[1], "r" ) ) == NULL )
    exit(1);

  /* Read N from file */
  fscanf ( file, "%d", &N );

  /* Allocate memory for array of polynomial coefficients */
  A = malloc ( ( N + 1 ) * sizeof ( float ) );

  /* Allocate memory for array of derivative polynomial coefficients */
  Aderiv = malloc ( N * sizeof ( float ) );

  /* Read polynomial coefficients from file */
  for ( i = 0; i <= N; i++ )
    fscanf ( file, "%e", A + i );

  /* Read initial point scan limits & step from file */
  fscanf ( file, "%f %f %f %f %f", &(MIN_REAL), &(MIN_IMAG), &(MAX_REAL), &(MAX_IMAG), &STEP );

  /* Close file */
  fclose ( file );

  /* Calculate derivative polynomial coefficients */
  for ( i = 0; i < N; i++ )
    Aderiv[i] = A[i+1] * ( i + 1 );

  /* Loop real part of initial point of search */
  for ( z0.real = MIN_REAL; z0.real <= MAX_REAL; z0.real += STEP )
  {
    /* Loop imaginary part of initial point of search */
    for ( z0.imag = MIN_IMAG; z0.imag <= MAX_IMAG; z0.imag += STEP )
    {
      complex root;
      int result = NewtonRaphson ( &z0, A, Aderiv, N, &root );

      /* If results = -1 then our method diverged, so we print nan */
      if ( result == -1 ) printf("nan ");
      /* If results = 0 then our method did not converge in 1000 runs, so we print incomplete */
      else if ( result == 0 ) printf("incomplete ");
      else
      /* If we are here then our method converged */
      {
        /* Add + sign if the real part of the solution is positive */
        if ( root.real >= 0.0 ) printf("+");
	printf("%.2f", root.real );
        /* Add + sign if the imaginary part of the solution is positive */
        if ( root.imag >= 0.0 ) printf("+");
	printf("%.2fi ", root.imag );
      }
    }
    printf("\n");
  }

  /* Free allocated memory */
  free ( A );
  free ( Aderiv );
  exit(0);
}

void polynomial ( float * A, int N, complex * x, complex * result )
{
  int i;
  complex XpowN;

  /* result is the return complex number, so it is initialized to 0 and we add to it An*x^n in each iteration */
  result->real = 0.0;
  result->imag = 0.0;

  /* XpowN is the x^i, so it is initialized to 1 and multiplied by x in each iteration */
  XpowN.real = 1.0;
  XpowN.imag = 0.0;

  /* Polynomial loop, for each iteration we add the term Ai*x^i */
  for ( i = 0; i <= N; i++ )
  {
    /* this will be the term Ai*x^i */
    complex multiplier;

    /* We give it the value of Ai*x^i */
    multiplier.real = A[i] * XpowN.real;
    multiplier.imag = A[i] * XpowN.imag;

    /* Add Ai*x^i to the sum */
    add ( result, &multiplier, result );

    /* Generate x^(i+1) for next run (no need to do this for i=N) */
    if ( i < N ) multiply ( x, &XpowN, &XpowN );
  }
}

int NewtonRaphson ( complex * z0, float * A, float * Aderiv, int N, complex * root )
{
  complex Xn, numerator, denominator, delta;
  float error_squared;
  int i;

  /* Xn is the current value of Newton-Raphson method, so it's initialized to the initial value z0 */
  Xn.real = z0->real;
  Xn.imag = z0->imag;

  /* Newton-Raphson loop */
  for ( i = 0; i < 1000; i++ )
  {
    /* Calculate denominator, i.e. the value of the derivative polynomial for input Xn */
    polynomial ( Aderiv, N - 1, &Xn, &denominator );
    /* If this value is zero, we cannot devide, so we need to return -1, meaning nan */
    if ( magnitude_squared ( &denominator ) < 1e-100 )
      return -1;

    /* Calculate numerator, i.e. the value of the polynomial for input Xn */
    polynomial ( A, N, &Xn, &numerator );
    
    /* Divide numerator / denominator to produce the value of delta which is to be subtracted from the current value of Xn */
    divide ( &numerator, &denominator, &delta );

    /* Calculate the square of the magnitude of delta */
    error_squared = magnitude_squared ( &delta );

    /* If the square of the magnitude of delta is < 1e-12 (i.e. mag(delta),1e-6) then our method has converged, so exit the function and return the root */
    if ( error_squared < 1e-12 )
    {
      /* Set the last value of Xn to the return variable root */
      root->real = Xn.real;
      root->imag = Xn.imag;
      return 1;
    }
    
    /* If we are here, then we may iterate again, so substract the delta from Xn and produce Xn for the next run. No need to do this if this is the last run (i=999) */
    if ( i < 1000 ) subtract ( &Xn, &delta, &Xn );
  }

  return 0;
}
