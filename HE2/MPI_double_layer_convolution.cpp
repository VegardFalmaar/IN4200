void MPI_double_layer_convolution (
  int M, int N, float **input,
  int K1, float **kernel1,
  int K2, float **kernel2,
  float **output
) {
  int my_rank, numprocs;
  int my_M, my_N, i_start, i_stop, j_start, j_stop;
  float **my_input, **my_output, *contig_array;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

  // first layer of convolution
  my_M = M/numprocs;
  my_N = N/numprocs;
  i_start = my_rank*my_M;
  i_stop = (my_rank+1)*my_M;
  j_start = my_rank*my_N;
  j_stop = (my_rank+1)*my_N;

  my_input = new float*[my_M];
  contig_array = new float[my_M*my_N];
  for (int i=0; i<my_M; i++) {
    my_input[i] = &(contig_array[i*my_N]);
    for (int j=0; j<my_N; j++)
      my_input[i][j] = 
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
