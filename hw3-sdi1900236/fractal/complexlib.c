#include<stdio.h>
#include<stdlib.h>

#include "complexlib.h"

/* Adds two complex numbers */
void add ( complex * a, complex * b, complex * result )
{
  result->real = a->real + b->real;
  result->imag = a->imag + b->imag;
}

/* Subtracts two complex numbers */
void subtract ( complex * a, complex * b, complex * result )
{
  result->real = a->real - b->real;
  result->imag = a->imag - b->imag;
}

/* Multiplies two complex numbers */
void multiply ( complex * a, complex * b, complex * result )
{
  complex buffer;

  buffer.real = a->real * b->real - a->imag * b->imag;
  buffer.imag = a->real * b->imag + a->imag * b->real;
  result->real = buffer.real;
  result->imag = buffer.imag;
}

/* Divides two complex numbers */
int divide ( complex * a, complex * b, complex * result )
{
  double mag = magnitude_squared ( b );
  
  if ( mag > 0.0 )
  {
    result->real = ( a->real * b->real + a->imag * b->imag ) / mag;
    result->imag = ( a->imag * b->real - a->real * b->imag ) / mag;
    return 1;
  }
  else return 0;
}

/* Returns the square of the magnitude of a complex number */
double magnitude_squared ( complex * a )
{
  return a->real * a->real + a->imag * a->imag;
}
