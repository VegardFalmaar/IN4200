#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>

int set2dRowPointers (double **A, const int M, const int N)
{
  for (int i=1; i<M; i++)
    A[i] = &(A[0][i*N]);
  return 0;
}

int alloc2d (double ***A, const int M, const int N)
{
  *A = malloc(M*sizeof(double *));
  (*A)[0] = malloc(M*N*sizeof(double));
  set2dRowPointers(*A, M, N);
  return 0;
}

int free2d (double **A)
{
  free(A[0]);
  free(A);
  return 0;
}

bool compare2d (double **A, double **B, const int M, const int N)
{
  bool is_equal = true;
  const double eps = 1e-6;

  for (int i=0; i<M*N; i++)
    if (abs(A[0][i] - B[0][i]) > eps){
      printf(" %d  %f   %f\n", i, A[0][i], B[0][i]);
      is_equal = false;
      break;
    }
  return is_equal;
}

void sweep_serial (int N, double **table1, int n, double **mask, double **table2)
{
  int i, j, ii, jj;
  double tmp;

  for (i=0; i<=N-n; i++)
    for (j=0; j<=N-n; j++){
      tmp = 0.0;
      for (ii=0; ii<n; ii++)
        for (jj=0; jj<n; jj++)
          tmp += table1[i+ii][j+jj]*mask[ii][jj];
      table2[i][j] = tmp;
    }
}

void sweep_parallel (int N, double **table1, int n, double **mask, double **table2)
{
  int i, j, ii, jj;
  double tmp;

  omp_set_num_threads(8);

  #pragma omp parallel for private(j, ii, jj, tmp)
  for (i=0; i<=N-n; i++)
    for (j=0; j<=N-n; j++){
      tmp = 0.0;
      for (ii=0; ii<n; ii++)
        for (jj=0; jj<n; jj++)
          tmp += table1[i+ii][j+jj]*mask[ii][jj];
      table2[i][j] = tmp;
    }
}

void print2d (double **A, const int M, const int N)
{
  for (int i=0; i<M; i++){
    for (int j=0; j<N; j++)
      printf(" %10.6f", A[i][j]);
    printf("\n");
  }
}

int main ()
{
  int i, j;
  const int N = 20000;
  const int n = 8;

  double **table1; alloc2d(&table1, N, N);
  #pragma omp parallel for
  for (i=0; i<N*N; i++)
    table1[0][i] = sin(i);

  // double **table2_s; alloc2d(&table2_s, N-n+1, N-n+1);
  // for (i=0; i<(N-n+1); i++)
    // table2_s[0][i] = 0.0;
  double **table2_p; alloc2d(&table2_p, N-n+1, N-n+1);
  for (i=0; i<(N-n+1); i++)
    table2_p[0][i] = 0.0;

  double **mask; alloc2d(&mask, n, n);
  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      mask[i][j] = i + j;

  const double start = omp_get_wtime();
  // sweep_serial(N, table1, n, mask, table2_s);
  sweep_parallel(N, table1, n, mask, table2_p);
  const double stop = omp_get_wtime();
  printf("Elapsed time: %f seconds\n", stop - start);
  /*

  const bool is_equal = compare2d(table2_s, table2_p, N-n+1, N-n+1);
  if (is_equal)
    printf("Equality!\n");
  else
    printf("No quality!\n");
  // */

  free2d(table1);
  // free2d(table2_s);
  free2d(table2_p);
  free2d(mask);

  return 0;
}
