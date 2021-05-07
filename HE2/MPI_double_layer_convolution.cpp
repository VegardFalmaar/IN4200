#include <cstddef>    // NULL
#include <stdexcept>
#include <mpi.h>
#include <iostream>

int alloc2D (float ***A, const int M, const int N)
{
  *A = new float*[M];
  (*A)[0] = new float[M*N];
  for (int i=1; i<M; i++)
    (*A)[i] = &((*A)[0][i*N]);
  return 0;
}

int calculate_overlap (
  const int rank, const int numprocs,
  const int K1, const int K2, const int direction
) {
  int overlap;
  if (direction == 0) {
    // above
    overlap = (K1-1)/2 + (K2-1)/2;
    overlap *= (rank > 0);
  } else if (direction == 1) {
    // below
    overlap = K1/2 + K2/2;
    overlap *= (rank < numprocs-1);
  } else
    throw std::invalid_argument( "direction must be 0 or 1" );

  return overlap;
}

int calculate_my_number_of_input_rows (
  const int my_rank, const int num_rows, const int numprocs,
  const int K1, const int K2
) {
  int i_start, i_stop, my_M, overlap_above, overlap_below, input_M;
  i_start = (my_rank*num_rows)/numprocs;
  i_stop = ((my_rank + 1)*num_rows)/numprocs;
  my_M = i_stop - i_start;
  overlap_above = calculate_overlap(my_rank, numprocs, K1, K2, 0);
  overlap_below = calculate_overlap(my_rank, numprocs, K1, K2, 1);
  input_M = my_M + overlap_above + overlap_below;
  return input_M;
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
  int input_M, transf_cnt_int;
  float **intermediate_layer;

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

  int transf_cnt_arr[numprocs], displacement[numprocs];

  // use enough input overlap to allow both kernels to be applied
  // without intermediate communication
  input_M = calculate_my_number_of_input_rows(my_rank, M, numprocs, K1, K2);

  // allocate input and intermediate arrays
  if (my_rank > 0)
    alloc2D(&input, input_M, N);
  alloc2D(&intermediate_layer, input_M-K1+1, N-K1+1);

  // scatter the input
  if (my_rank == 0) {
    int overlap_above, overlap_below, rows, current=0;
    for (int i=0; i<numprocs; i++) {
      rows = calculate_my_number_of_input_rows(i, M, numprocs, K1, K2);
      transf_cnt_arr[i] = rows*N;
      overlap_above = calculate_overlap(i, numprocs, K1, K2, 0);
      overlap_below = calculate_overlap(i, numprocs, K1, K2, 1);
      displacement[i] = (current - overlap_above)*N;
      current += rows - overlap_above - overlap_below;
    }
    transf_cnt_arr[0] = displacement[0] = 0;
  }
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
    int output_M = input_M - K1 - K2 + 2;
    int output_N = N - K1 - K2 + 2;
    for (int i=0; i<output_M; i++)
      output[i] = &(output[0][i*output_N]);
  }
  single_layer_convolution(input_M-K1+1, N-K1+1, intermediate_layer, K2, kernel2, output);

  // gather the results
  if (my_rank == 0) {
    int rows;
    for (int i=0; i<numprocs; i++) {
      rows = calculate_my_number_of_input_rows(i, M, numprocs, K1, K2);
      transf_cnt_arr[i] = (rows - K1 - K2 + 2)*(N - K1 - K2 + 2);
    }
    displacement[0] = 0;
    for (int i=1; i<numprocs; i++)
      displacement[i] = displacement[i-1] + transf_cnt_arr[i-1];
    transf_cnt_arr[0] = 0;
  }
  transf_cnt_int = (input_M - K1 - K2 + 2)*(N - K1 - K2 + 2)*(my_rank > 0);

  // the data is sent from "input" as "input" was reused for the output of the
  // second convolution
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
