#include <stdlib.h> // malloc and free.
#include <stdio.h>  // printf

int alloc2D_d(double ***A, int n, int m)
{
    *A = malloc(n * sizeof *A);
    (*A)[0] = malloc(n*m * sizeof (*A)[0]);
    if (!(*A)[0] || !*A){
        // Allocation failed.
        printf("Allocation failed\n");
        return 1;
    }

    for (size_t i = 1; i < n; i++) {
        (*A)[i] = &((*A)[0][i*m]);
    }
    return 0;
}

int zeros2D_d(double ***A, int m, int n)
{
  alloc2D_d(A, m, n);
  for (size_t i=0; i<m*n; i++)
    (*A)[0][i] = 0.0;
  return 0;
}

int alloc_full_i(int **A, const int n, const int val)
{
  *A = malloc(n*sizeof(int));
  for (size_t i=0; i<n; i++)
    (*A)[i] = val;
  return 0;
}

// Free pointers allocated in alloc2D.
int free2D(double **A)
{
    free(A[0]);
    free(A);
    return 0;
}

// Print matrix values.
int printmat_d(double **A, int n, int m)
{
    for (size_t i = 0; i < n; ++i){
        printf("| ");
        for (size_t j = 0; j < m; ++j){
            printf("%6.3lf ", A[i][j]);
        }
        printf("|\n");
    }
    return 0;
}


int printvec_d(double *y, int N)
{
    printf("(");
    for (size_t i = 0; i < N-1; i++) {
        printf("%4.3lf, ", y[i]);
    }
    printf("%3.3lf", y[N-1]);
    printf(")\n");
    return 0;
}

int printvec_i(int *y, int N)
{
    printf("(");
    for (size_t i = 0; i < N-1; i++) {
        printf("%4d, ", y[i]);
    }
    printf("%4d", y[N-1]);
    printf(")\n");
    return 0;
}
