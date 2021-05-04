#include <cstddef>
#include <iostream>

/*
void MPI_double_layer_convolution (
  const int M, const int N, float **input,
  const int K1, const float **kernel1,
  const int K2, const float **kernel2,
  float **output
) {
  int my_rank, numprocs;
  int my_M, i_start, i_stop;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

  i_start = (my_rank*M)/numprocs;
  i_stop = ((my_rank + 1)*M)/numprocs;
  my_M = i_stop - i_start;

  if (my_rank == 0) {
    // scatter
  } else {
    float *contig_array;
    input = new float*[my_M];
    contig_array = new float[my_M*N];
    for (int i=0; i<my_M; i++)
      my_input[i] = &(contig_array[i*N]);
    // gather
  }
}

void single_layer_convolution (
  int M, int N, float **input,
  int K, float **kernel,
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
*/

int main (int nargs, char **args)
{
  int M=0, N=0, K1=0, K2=0, my_rank, num_procs;
  float **input=NULL, **output=NULL, **kernel1=NULL, **kernel2=NULL;

  if (nargs != 5) {
    std::cout << "Please include M, N, K1, K2 as arguments" << std::endl;
    exit(1);
  }

  // MPI_Init (&nargs, &args);
  // MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);

  if (my_rank==0) {
    M = atoi(args[1]);
    N = atoi(args[2]);
    K1 = atoi(args[3]);
    K2 = atoi(args[4]);

    // allocate contiguous input array
    input = new float*[M];
    float *contig_array = new float[M*N];
    for (int i=0; i<M; i++)
      input[i] = &(contig_array[i*N]);

    // allocate contiguous output array
    int N_out_rows = M - K1 - K2 + 2;
    int N_out_cols = N - K1 - K2 + 2;
    output = new float*[N_out_rows];
    contig_array = new float[N_out_rows*N_out_cols];
    for (int i=0; i<N_out_rows; i++)
      output[i] = &(contig_array[i*N_out_rows]);

    // allocate the convolutional kernels
    kernel1 = new float*[K1];
    contig_array = new float[K1*K1];
    for (int i=0; i<K1; i++)
      kernel1[i] = &(contig_array[i*K1]);
    kernel2 = new float*[K2];
    contig_array = new float[K2*K2];
    for (int i=0; i<K2; i++)
      kernel2[i] = &(contig_array[i*K2]);

    // fill 2D array ’input’ with some values
    // fill kernel with some values
    // ....
  }
  // process 0 broadcasts values of M, N, K to all the other processes
  // ...
  if (my_rank>0) {
    // allocated the convolutional kernel with K rows and K columns
    // ...
  }
  // process 0 broadcasts the content of kernel to all the other processes
  // ...

  // parallel computation of a double-layer convolution
  // MPI_double_layer_convolution (M, N, input, K, kernel, output);

  if (my_rank==0) {
    // For example, compare the content of array ’output’ with that is
    // produced by the sequential function single_layer_convolution
    // ...
  }
  // MPI_Finalize();

  delete[] *input;
  delete[] input;

  return 0;
  }
