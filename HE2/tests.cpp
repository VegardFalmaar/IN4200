// #include "MPI_double_layer_convolution.cpp"
// #include "MPI_main.cpp"

int test_MPI_double_layer_convolution_first_layer ()
{
  // compute the example in Figure 1 in the task sheet
  int M=8, N=8, K1=3, K2=1;
  float *contig_array;

  float **input = new float*[M];
  contig_array = new float[M*N];
  for (int i=0; i<M; i++) {
    input[i] = &contig_array[i*N];
    for (int j=0; j<4; j++)
      input[i][j] = 10.0;
    for (int j=4; j<N; j++)
      input[i][j] = 0.0;
  }

  float **kernel_1 = new float*[K1];
  contig_array = new float[K1*K1];
  for (int i=0; i<K1; i++) {
    kernel_1[i] = &contig_array[i*K1];
    for (int j=0; j<K1; j++) {
      kernel_1[i][0] = 1.0;
      kernel_1[i][1] = 0.0;
      kernel_1[i][2] = -1.0;
    }
  }

  // the second layer is 1x1 2D-array with element 1.0,
  // s.t. it does not change the output
  float **kernel_2 = new float*[K2];
  contig_array = new float[K2*K2];
  for (int i=0; i<K2; i++) {  // redundant, but allows for easy generalization
    kernel_2[i] = &contig_array[i*K2];
    kernel_2[0][0] = 1.0;
  }

  delete[] *input;
  delete[] input;
  delete[] *kernel_1;
  delete[] kernel_1;
  delete[] *kernel_2;
  delete[] kernel_2;

  return 0;
}

int main ()
{
  test_MPI_double_layer_convolution_first_layer();

  return 0;
}
