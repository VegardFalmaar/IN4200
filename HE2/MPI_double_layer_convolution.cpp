#include <cstddef>    // NULL
#include <iostream>
#include <stdexcept>
#include <mpi.h>

int alloc2D (float ***A, const int M, const int N)
{
  *A = new float*[M];
  (*A)[0] = new float[M*N];
  for (int i=1; i<M; i++)
    (*A)[i] = &((*A)[0][i*N]);
  return 0;
}

int calculate_my_number_of_rows (
  const int my_rank, const int num_rows, const int numprocs
) {
  int i_start, i_stop, my_M;
  i_start = (my_rank*num_rows)/numprocs;
  i_stop = ((my_rank + 1)*num_rows)/numprocs;
  my_M = i_stop - i_start;
  return my_M;
}

int calculate_overlap (
  const int rank, const int numprocs,
  const int K1, const int K2, const int direction
) {
  int overlap;
  if (direction == 0) {
    // above
    overlap = K1/2 + K1%2 + K2/2 + K2%2 - 2;
    overlap *= (rank > 0);
  } else if (direction == 1) {
    // below
    overlap = K1/2 + K2/2;
    overlap *= (rank < numprocs-1);
  } else
    throw std::invalid_argument( "direction must be 0 or 1" );

  return overlap;
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
  int my_M, input_M, receive_count;
  float *receive_input;
  float **intermediate;

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

  int send_counts[numprocs], displacement[numprocs];

  my_M = calculate_my_number_of_rows(my_rank, M, numprocs);

  {   // scope some temporary variables
    int overlap_above = calculate_overlap(my_rank, numprocs, K1, K2, 0);
    int overlap_below = calculate_overlap(my_rank, numprocs, K1, K2, 1);
    input_M = my_M + overlap_above + overlap_below;
    int interm_M = input_M - K1 + 1;
    int interm_N = N - K1 + 1;
    alloc2D(&intermediate, interm_M, interm_N);
  }

  // allocate input and output arrays on the other processes. Use enough
  // overlap to allow both kernels to be applied without communication
  if (my_rank > 0) {
    alloc2D(&input, input_M, N);
    receive_input = input[0];
    int output_M = input_M - K1 - K2 + 2;
    int output_N = N - K1 - K2 + 2;
    alloc2D(&output, output_M, output_N);
  }

  // scatter the input
  if (my_rank == 0) {
    int overlap_above, overlap_below, rows, current=0;
    for (int i=0; i<numprocs; i++) {
      rows = calculate_my_number_of_rows(i, M, numprocs);
      overlap_above = calculate_overlap(i, numprocs, K1, K2, 0);
      overlap_below = calculate_overlap(i, numprocs, K1, K2, 1);
      send_counts[i] = (rows + overlap_above + overlap_below)*N;
      displacement[i] = (current - overlap_above)*N;
      current += rows;
    }
    send_counts[0] = displacement[0] = 0;
  }
  receive_count = input_M*N;
  MPI_Scatterv (
    *input, send_counts, displacement, MPI_FLOAT,
    receive_input, receive_count, MPI_FLOAT, 0, MPI_COMM_WORLD
  );

  /*
  std::cout << "  Rank " << my_rank << ": " << my_M << " " << receive_count << " -";
  for (int i=0; i<receive_count; i++)
    std::cout << " " << (*input)[i];
  std::cout << std::endl << std::endl;
  // */

  // do the convolution baby
  single_layer_convolution(input_M, N, input, K1, kernel1, intermediate);
  single_layer_convolution(input_M-K1+1, N-K1+1, intermediate, K2, kernel2, output);

  // gather the results
  if (my_rank == 0) {
    int overlap_above, overlap_below, rows;
    for (int i=0; i<numprocs; i++) {
      rows = calculate_my_number_of_rows(i, M, numprocs);
      overlap_above = calculate_overlap(i, numprocs, K1, K2, 0);
      overlap_below = calculate_overlap(i, numprocs, K1, K2, 1);
      send_counts[i] =
        (rows + overlap_above + overlap_below - K1 - K2 + 2)
        *(N - K1 - K2 + 2);
    }
    for (int i=1; i<numprocs; i++)
      displacement[i] = displacement[i-1] + send_counts[i-1];
    send_counts[0] = displacement[0] = 0;

    // /*
    for (int i=0; i<numprocs; i++)
      std::cout << " " << send_counts[i];
    std::cout << std::endl;
    for (int i=0; i<numprocs; i++)
      std::cout << " " << displacement[i];
    std::cout << std::endl;
    // */
  }
  receive_count = (input_M - K1 - K2 + 2)*(N - K1 - K2 + 2)*(my_rank > 0);
  std::cout << " " << my_rank << ": " << receive_count << std::endl;
  MPI_Gatherv (
    *output,
    receive_count,
    MPI_FLOAT,
    *output,
    send_counts,
    displacement,
    MPI_FLOAT,
    0,
    MPI_COMM_WORLD
  );

  if (my_rank > 0) {
    delete[] *input;
    delete[] input;
    delete[] *output;
    delete[] output;
    input = output = NULL;
  }
}

int main (int nargs, char **args)
{
  int M=0, N=0, K1=0, K2=0, my_rank, numprocs;
  float **input=NULL, **output=NULL, **kernel1=NULL, **kernel2=NULL;

  std::cout << std::endl;

  if (nargs != 5) {
    std::cout << "Please include M, N, K1, K2 as arguments" << std::endl;
    exit(1);
  }

  MPI_Init (&nargs, &args);
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

  if (my_rank == 0) {
    M = atoi(args[1]);
    N = atoi(args[2]);
    K1 = atoi(args[3]);
    K2 = atoi(args[4]);

    alloc2D(&input, M, N);
    int N_out_rows = M - K1 - K2 + 2;
    int N_out_cols = N - K1 - K2 + 2;
    alloc2D(&output, N_out_rows, N_out_cols);
    alloc2D(&kernel1, K1, K1);
    alloc2D(&kernel2, K2, K2);

    // fill the input array with arbitrary values
    for (int i=0; i<M; i++) {
      for (int j=0; j<N; j++)
        input[i][j] = i*N + j;
      /*
      for (int j=0; j<N/2; j++)
        input[i][j] = 10.0;
      for (int j=N/2; j<N; j++)
        input[i][j] = 0.0;
      */
    }

    // fill the kernel arrays with arbitrary values
    for (int i=0; i<K1; i++)
      for (int j=0; j<K1; j++)
        kernel1[i][j] = 2.0*i;
    for (int i=0; i<K2; i++)
      for (int j=0; j<K2; j++)
        kernel2[i][j] = j%3;
  }

  // process 0 broadcasts values of M, N, K to all the other processes
  int sizes[] = {M, N, K1, K2};
  MPI_Bcast (sizes, 4, MPI_FLOAT, 0, MPI_COMM_WORLD);
  M = sizes[0];
  N = sizes[1];
  K1 = sizes[2];
  K2 = sizes[3];

  if (my_rank > 0) {
    alloc2D(&kernel1, K1, K1);
    alloc2D(&kernel2, K2, K2);
  }

  // process 0 broadcasts the content of kernel to all the other processes
  MPI_Bcast (*kernel1, K1*K1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Bcast (*kernel2, K2*K2, MPI_FLOAT, 0, MPI_COMM_WORLD);

  std::cout << "Rank " << my_rank << "/" << numprocs << ": "
    << M << " " << N << " " << K1 << " " << K2
    << " -";
  for (int i=0; i<K1*K1; i++)
    std::cout << " " << (*kernel1)[i];
  std::cout << " -";
  for (int i=0; i<K2*K2; i++)
    std::cout << " " << (*kernel2)[i];
  std::cout << std::endl;

  if (my_rank == 0)
    std::cout << "Computing ... ";
  // parallel computation of a double-layer convolution
  MPI_double_layer_convolution (M, N, input, K1, kernel1, K2, kernel2, output);
  if (my_rank == 0)
    std::cout << "Done" << std::endl;

  // compare the output of parallel code to sequential code
  if (my_rank == 0) {
    float **verify_output, **intermediate;
    int errors = 0;

    alloc2D(&intermediate, M-K1+1, N-K1+1);
    int N_out_rows = M - K1 - K2 + 2;
    int N_out_cols = N - K1 - K2 + 2;
    alloc2D(&verify_output, N_out_rows, N_out_cols);
    single_layer_convolution(M, N, input, K1, kernel1, intermediate);
    single_layer_convolution(M-K1+1, N-K1+1, intermediate, K2, kernel2, verify_output);

    for (int i=0; i<N_out_rows; i++)
      for (int j=0; j<N_out_cols; j++)
        if (output[i][j] != verify_output[i][j])
          errors++;
    std::cout << "Errors: " << errors << std::endl;

    /*
    std::cout << "Sequential:";
    for (int i=0; i<N_out_rows; i++) {
      for (int j=0; j<N_out_cols; j++)
        std::cout << " " << verify_output[i][j];
      std::cout << " -";
    }
    std::cout << std::endl;
    std::cout << "Parallel:  ";
    for (int i=0; i<N_out_rows; i++) {
      for (int j=0; j<N_out_cols; j++)
        std::cout << " " << output[i][j];
      std::cout << " -";
    }
    std::cout << std::endl;
    // */
    delete[] *verify_output; delete[] verify_output;
    delete[] *intermediate; delete[] intermediate;
  }

  if (my_rank == 0) {
    delete[] *input; delete[] input;
    delete[] *output; delete[] output;
  }
  delete[] *kernel1; delete[] kernel1;
  delete[] *kernel2; delete[] kernel2;

  MPI_Finalize();

  return 0;
  }
