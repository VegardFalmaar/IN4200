#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

bool compare_arrays (const double *arr_1, const double *arr_2, const int N)
{
  bool equal = true;
  for (int i=0; i<N; i++)
    if (arr_1[i] != arr_2[i]) {
      equal = false;
      break;
    }
  return equal;
}

int main ()
{
  int i, N = 100;
  double s = 0.;
  double *a = (double*)malloc (N*sizeof(double));
  double *b = (double*)malloc (N*sizeof(double));
  for (i=0; i<N; i++) {
    a[i] = 10.0+2*i;
    b[i] = 20.0+sin(0.1*i);
  }
  for (i=0; i<N-1; i++) {
    s += a[i];
    a[i+1] = cos(b[i]);
  }
  printf("%f \n", s);

  double *a2 = (double*)malloc (N*sizeof(double));
  double *b2 = (double*)malloc (N*sizeof(double));

  a2[0] = 10.0;
  s = 0.0;

  #pragma omp parallel default(shared)
  {
  #pragma omp for
  for (i=0; i<N; i++)
    b2[i] = 20.0 + sin(0.1*i);

  #pragma omp for reduction(+: s)
  for (i=0; i<N-1; i++) {
    s += a[i];
    a2[i+1] = cos(b2[i]);
  }
  }

  printf("%f \n", s);
  bool equal1 = compare_arrays(a, a2, N);
  bool equal2 = compare_arrays(b, b2, N);
  if (equal1 && equal2)
    printf("Success\n");

  free(a); free(b); free(a2); free(b2);

  return 0;
}
