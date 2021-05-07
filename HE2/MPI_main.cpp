#include <iostream>
#include <chrono>
#include "MPI_double_layer_convolution.cpp"

int main (int nargs, char **args)
{
  int M=0, N=0, K1=0, K2=0, my_rank, numprocs;
  float **input=NULL, **output=NULL, **kernel1=NULL, **kernel2=NULL;
  // for timing the code
  std::chrono::high_resolution_clock::time_point start, stop;

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
      for (int j=0; j<N/2; j++)
        input[i][j] = 10.0;
      for (int j=N/2; j<N; j++)
        input[i][j] = j%((i%10)+5);
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
  MPI_Bcast (sizes, 4, MPI_INT, 0, MPI_COMM_WORLD);

  if (my_rank > 0) {
    M = sizes[0]; N = sizes[1]; K1 = sizes[2]; K2 = sizes[3];
    alloc2D(&kernel1, K1, K1);
    alloc2D(&kernel2, K2, K2);
  }

  // process 0 broadcasts the content of kernel to all the other processes
  MPI_Bcast (*kernel1, K1*K1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Bcast (*kernel2, K2*K2, MPI_FLOAT, 0, MPI_COMM_WORLD);

  // parallel computation of a double-layer convolution
  if (my_rank == 0) {
    std::cout << std::endl << "Computing ... ";
    start = std::chrono::high_resolution_clock::now();
  }
  MPI_double_layer_convolution (M, N, input, K1, kernel1, K2, kernel2, output);
  if (my_rank == 0) {
    stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Done" << std::endl;
    std::cout << "Duration of MPI function: "
      << duration.count() << " milliseconds" << std::endl;
  }

  // compare the output of parallel code to sequential code
  if (my_rank == 0) {
    float **verify_output, **intermediate;
    int errors = 0;

    alloc2D(&intermediate, M-K1+1, N-K1+1);
    int N_out_rows = M - K1 - K2 + 2;
    int N_out_cols = N - K1 - K2 + 2;
    alloc2D(&verify_output, N_out_rows, N_out_cols);

    start = std::chrono::high_resolution_clock::now();
    single_layer_convolution(M, N, input, K1, kernel1, intermediate);
    single_layer_convolution(M-K1+1, N-K1+1, intermediate, K2, kernel2, verify_output);
    stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Duration of sequential function: "
      << duration.count() << " milliseconds" << std::endl;

    for (int i=0; i<N_out_rows; i++)
      for (int j=0; j<N_out_cols; j++)
        if (output[i][j] != verify_output[i][j])
          errors++;
    std::cout << "Errors: " << errors << std::endl;

    delete[] *verify_output; delete[] verify_output;
    delete[] *intermediate; delete[] intermediate;
  }

  // check that input and output pointers are NULL
  if (my_rank > 0) {
    bool pointers_are_null = ( (input == NULL) && (output == NULL) );
    if ( !pointers_are_null )
      std::cout << "Bad pointers on rank " << my_rank << std::endl;
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
