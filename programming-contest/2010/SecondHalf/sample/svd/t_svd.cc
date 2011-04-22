#include "svd.h"
#include "d-mat2d.h"

#include <stdio.h>

int main( int argc, char *argv[] )
{
  const int m=4, n=3;
  double x[m*n] = {
    1.23000, -1.00000,  2.34000,
    1.89000,  2.00000,  3.23000,
    3.46000,  0.00000,  7.68000,
    5.69000, -2.00000,  9.02000
  };
  double **A = mat_clone_vec(x, m, n);
  
  double **U = mat_alloc(m,n);
  double **S = mat_alloc(1,n);
  double **V = mat_alloc(n,n);
  
  // call the core function
  mat_svd(A, m, n, U, S[0], V);
  
  printf( "showing singular values..\n" );
  mat_print(S,1,n, "%8.5lf ");
  
  printf( "showing matrix a:\n" );
  mat_print(A,m,n, "%8.5lf ");
  
  printf( "showing matrix u:\n" );
  mat_print(U,m,n, "%8.5lf ");
  
  printf( "showing matrix v:\n" );
  mat_print(V,n,n, "%8.5lf ");
  
  double **Vt = mat_transpose(V, n, n);
  printf( "showing matrix vt:\n" );
  mat_print(Vt,n,n, "%8.5lf ");
  
  double **S2 = mat_clone_diag(S[0], n);
  printf( "showing matrix S:\n" );
  mat_print(S2,n,n, "%8.5lf ");
  
  mat_mul(U, m, n, S2, n, n, U);
  mat_mul(U, m, n, Vt, n, n, U);
  mat_free(Vt);
  mat_free(S2);

  printf( "showing matrix ak:\n" );
  mat_print(U,m,n, "%8.5lf ");
  
  mat_free(A);
  mat_free(U);
  mat_free(S);
  mat_free(V);
  return 0;
}
