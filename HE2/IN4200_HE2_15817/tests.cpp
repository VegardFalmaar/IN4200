#include <iostream>
#include "MPI_double_layer_convolution.cpp"

using namespace std;

class Test_single_layer_convolution
{
private:
  int M=8, N=8, K1=3, K2;
  float *contig_array;
  bool test_passed;
  float **input, **kernel_1, **kernel_2, **output;

public:
  Test_single_layer_convolution ()
  {
    // set up the common input for the tests
    input = new float*[M];
    contig_array = new float[M*N];
    for (int i=0; i<M; i++) {
      input[i] = &contig_array[i*N];
      for (int j=0; j<4; j++)
        input[i][j] = 10.0;
      for (int j=4; j<N; j++)
        input[i][j] = 0.0;
    }

    kernel_1 = new float*[K1];
    contig_array = new float[K1*K1];
    for (int i=0; i<K1; i++) {
      kernel_1[i] = &contig_array[i*K1];
      kernel_1[i][0] = 1.0;
      kernel_1[i][1] = 0.0;
      kernel_1[i][2] = -1.0;
    }
  }

  ~Test_single_layer_convolution ()
  {
    delete[] *input; delete[] input;
    delete[] *kernel_1; delete[] kernel_1;
  }

  int test_single_layer_convolution_first_layer ()
  {
    // compute the example in Figure 1 in the task sheet
    K2=1;
    test_passed = true;

    // the second layer is 1x1 2D-array with element 1.0,
    // s.t. it does not change the output
    kernel_2 = new float*[K2];
    contig_array = new float[K2*K2] { 1.0 };
    kernel_2[0] = &contig_array[0];

    int N_rows = M-K1-K2+2;
    int N_cols = N-K1-K2+2;
    output = new float*[N_rows];
    contig_array = new float[N_rows*N_cols];
    for (int i=0; i<N_rows*N_cols; i++)
      contig_array[i] = 0.0;
    for (int i=0; i<N_rows; i++)
      output[i] = &contig_array[i*N_cols];

    float expected[N_rows][N_cols];
    for (int i=0; i<N_rows; i++) {
      expected[i][0] = 0.0;
      expected[i][1] = 0.0;
      expected[i][2] = 30.0;
      expected[i][3] = 30.0;
      expected[i][4] = 0.0;
      expected[i][5] = 0.0;
    }

    // call the function and compare the results
    single_layer_convolution(M, N, input, K1, kernel_1, output);
    for (int i=0; i<N_rows; i++) {
      for (int j=0; j<N_cols; j++)
        if (output[i][j] != expected[i][j])
          test_passed = false;
    }

    delete[] *kernel_2; delete[] kernel_2;
    delete[] *output; delete[] output;

    if (test_passed)
      return 0;
    else
      return 1;
  }
};

int main ()
{
  int out;
  bool tests_passed = true;

  Test_single_layer_convolution tester;

  cout << "Running test_single_layer_convolution_first_layer() ... ";
  out = tester.test_single_layer_convolution_first_layer();
  if (out == 0)
    cout << "passed" << endl;
  else {
    cout << "failed" << endl;
    tests_passed = false;
  }

  if (tests_passed)
    return 0;
  else
    return 1;
}
