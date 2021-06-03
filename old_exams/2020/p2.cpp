#include <omp.h>
#include <iostream>
#include <cmath>
#include <chrono>
#include "module.cpp"

int serial (char *array, const int N)
{
  int i, j, sqrt_N;

  array[0] = array[1] = 0;
  array[2] = 1;
  for (i=3; i<N; i++)
    array[i] = i%2;

  sqrt_N = (int) (sqrt(N));
  for (i=3; i<=sqrt_N; i+=2) {
    if (array[i]) {
      for (j=i*i; j<N; j+=2*i)
        array[j] = 0;
    }
  }
  return 0;
}

bool oddIsPrime(const int odd_num)
{
  bool prime = true;
  for (int i=3; i*i<=odd_num; i+=2)
    if (odd_num%i == 0) {
      prime = false;
      break;
    }
  return prime;
}

int parallel (char *array, const int N)
{
  int i, j, sqrt_N;

  array[0] = array[1] = 0;
  array[2] = 1;
  sqrt_N = (int) (sqrt(N));

  omp_set_num_threads(4);
  #pragma omp parallel private(i, j)
  {
  #pragma omp master
  std::cout << "Running with "
    << omp_get_num_threads() << " threads" << std::endl;
  #pragma omp for
  for (i=3; i<N; i++)
    array[i] = i%2;

  #pragma omp for schedule(static, 1)
  for (i=3; i<=sqrt_N; i+=2)
    // if (array[i])
    if (oddIsPrime(i))
      for (j=i*i; j<N; j+=2*i)
        array[j] = 0;
  }
  return 0;
}

int print_array (const char *arr, const int N)
{
  for (int i=0; i<N; i++)
    printf(" %d", arr[i]);
  std::cout << std::endl;
  return 0;
}

int main ()
{
  std::chrono::high_resolution_clock::time_point start, stop;
  const int N = 3E8;
  // const int N = 21;
  char *arr_serial = new char[N];
  char *arr_parallel = new char[N];

  start = std::chrono::high_resolution_clock::now();
  serial(arr_serial, N);
  stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "Duration of serial: "
      << duration.count() << " milliseconds" << std::endl;

  start = std::chrono::high_resolution_clock::now();
  parallel(arr_parallel, N);
  stop = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "Duration of parallel: "
      << duration.count() << " milliseconds" << std::endl;

  // print_array(arr_serial, N);
  // print_array(arr_parallel, N);

  bool success = compare_arrays(arr_serial, arr_parallel, N);
  if (success)
    std::cout << "Success" << std::endl;
  else
    std::cout << "Failed" << std::endl;

  delete arr_serial;
  delete arr_parallel;

  return 0;
}
