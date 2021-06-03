
int set2dRowPointers (double **A, const int M, const int N)
{
  for (int i=1; i<M; i++)
    A[i] = &(A[0][i*N]);
  return 0;
}

int alloc2d (double ***A, const int M, const int N)
{
  *A = new double*[M];
  (*A)[0] = new double[M*N];
  set2dRowPointers(*A, M, N);
  return 0;
}

bool compare_arrays (const char *arr_1, const char *arr_2, const int N)
{
  bool equal = true;
  for (int i=0; i<N; i++)
    if (arr_1[i] != arr_2[i]) {
      equal = false;
      break;
    }
  return equal;
}

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

int fillWithValues (double **arr, const int M, const int N)
{
  for (int i=0; i<M; i++)
    for (int j=0; j<N; j++)
      arr[i][j] = (i%3)*(j+1)+1;
  return 0;
}
