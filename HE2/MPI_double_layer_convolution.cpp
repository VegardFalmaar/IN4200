#include <cstddef>    // NULL
#include <stdexcept>
#include <mpi.h>

int set2dRowPointers (float **A, const int M, const int N)
{
  for (int i=1; i<M; i++)
    A[i] = &(A[0][i*N]);
  return 0;
}

int alloc2d (float ***A, const int M, const int N)
{
  *A = new float*[M];
  (*A)[0] = new float[M*N];
  set2dRowPointers(*A, M, N);
  return 0;
}

int myNumInputRows (
  const int my_rank, const int num_rows, const int numprocs,
  const int overlap_above, const int overlap_below
) {
  int i_start, i_stop, my_M, input_M;
  i_start = (my_rank*num_rows)/numprocs;
  i_stop = ((my_rank + 1)*num_rows)/numprocs;
  my_M = i_stop - i_start;
  input_M = my_M
            + overlap_above*(my_rank>0)
            + overlap_below*(my_rank<numprocs-1);
  return input_M;
}

int calculateTransferCountAndDisplacement (
  int transf_cnt_arr[], int displacement[],
  const int overlap_above, const int overlap_below,
  const int M, const int N, const int numprocs
) {
  for (int i=0; i<numprocs; i++)
    transf_cnt_arr[i] = N*myNumInputRows(i, M, numprocs, overlap_above, overlap_below);
  displacement[0] = 0;
  for (int i=1; i<numprocs; i++) {
    displacement[i] = displacement[i-1]
                      + transf_cnt_arr[i-1]
                      - (overlap_above + overlap_below)*N;
  }
  transf_cnt_arr[0] = 0;
  return 0;
}

void single_layer_convolution (
  const int M, const int N, const float * const *input,
  const int K, const float * const *kernel,
  float **output
) {
  int i,j,ii,jj;
  double temp;
  for (i=0; i<=M-K; i++)
    for (j=0; j<=N-K; j++) {
      temp = 0.0;
      for (ii=0; ii<K; ii++)
        for (jj=0; jj<K; jj++)
          temp += input[i+ii][j+jj]*kernel[ii][jj];
      output[i][j] = temp;
    }
}

void MPI_double_layer_convolution (
  const int M, const int N, float **input,
  const int K1, const float * const *kernel1,
  const int K2, const float * const *kernel2,
  float **output
) {
  int my_rank, numprocs;
  int transf_cnt_int;
  float **intermediate_layer;

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

  int transf_cnt_arr[numprocs], displacement[numprocs];

  // use enough input overlap to allow both kernels to be applied
  // without intermediate communication
  const int overlap_above = (K1-1)/2 + (K2-1)/2;
  const int overlap_below = K1/2 + K2/2;
  const int input_M = myNumInputRows(my_rank, M, numprocs, overlap_above, overlap_below);
  const int output_M = input_M - K1 - K2 + 2;
  const int output_N = N - K1 - K2 + 2;

  // allocate input and intermediate arrays
  if (my_rank > 0)
    alloc2d(&input, input_M, N);
  alloc2d(&intermediate_layer, input_M-K1+1, N-K1+1);

  // scatter the input
  if (my_rank == 0)
    calculateTransferCountAndDisplacement(
      transf_cnt_arr, displacement,
      overlap_above, overlap_below,
      M, N, numprocs
    );

  transf_cnt_int = input_M*N*(my_rank > 0);
  MPI_Scatterv (
    *input, transf_cnt_arr, displacement, MPI_FLOAT,
    *input, transf_cnt_int, MPI_FLOAT, 0, MPI_COMM_WORLD
  );

  // do the convolution
  single_layer_convolution(input_M, N, input, K1, kernel1, intermediate_layer);
  // processes other than 0 reuse input array for output from the second layer
  if (my_rank > 0) {
    output = input; input = NULL;
    set2dRowPointers(output, output_M, output_N);
  }
  single_layer_convolution(input_M-K1+1, N-K1+1, intermediate_layer, K2, kernel2, output);

  // gather the results
  if (my_rank == 0) {
    int rows;
    for (int i=0; i<numprocs; i++) {
      rows = myNumInputRows(i, M, numprocs, overlap_above, overlap_below);
      transf_cnt_arr[i] = (rows - K1 - K2 + 2)*(N - K1 - K2 + 2);
    }
    displacement[0] = 0;
    for (int i=1; i<numprocs; i++)
      displacement[i] = displacement[i-1] + transf_cnt_arr[i-1];
    transf_cnt_arr[0] = 0;
  }
  transf_cnt_int = output_M*output_N*(my_rank > 0);

  MPI_Gatherv (
    *output, transf_cnt_int, MPI_FLOAT,
    *output, transf_cnt_arr, displacement,
    MPI_FLOAT, 0, MPI_COMM_WORLD
  );

  delete[] *intermediate_layer; delete[] intermediate_layer;
  if (my_rank > 0) {
    delete[] *output; delete[] output;
    output = NULL;
  }
}
