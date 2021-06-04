#include <omp.h>
#include <iostream>
#include <chrono>
#include "module.cpp"

void sweep_serial (
  const int N, const double *const *table1, const int n,
  const double *const *mask, double **table2
) {
  int i,j,ii,jj;
  double temp;
  for (i=0; i<=N-n; i++)
    for (j=0; j<=N-n; j++) {
      temp = 0.0;
      for (ii=0; ii<n; ii++)
        for (jj=0; jj<n; jj++)
          temp += table1[i+ii][j+jj]*mask[ii][jj];
      table2[i][j] = temp;
  }
}

void sweep_parallel (
  const int N, const double *const *table1, const int n,
  const double *const *mask, double **table2
) {
  omp_set_num_threads(omp_get_max_threads());
  #pragma omp parallel
  {
  int i,j,ii,jj;
  double temp;

  #pragma omp master
  std::cout << "Running with "
    << omp_get_num_threads() << " threads" << std::endl;

  #pragma omp for collapse(2)
  for (i=0; i<=N-n; i++)
    for (j=0; j<=N-n; j++) {
      temp = 0.0;
      for (ii=0; ii<n; ii++)
        for (jj=0; jj<n; jj++)
          temp += table1[i+ii][j+jj]*mask[ii][jj];
      table2[i][j] = temp;
    }
  }
}

int main ()
{
  std::chrono::high_resolution_clock::time_point start, stop;
  const int N = 5E3;
  const int n = 10;

  double **table_1, **table_2_s, **table_2_p, **mask;
  alloc2d(&table_1, N, N);
  alloc2d(&table_2_s, N-n+1, N-n+1);
  alloc2d(&table_2_p, N-n+1, N-n+1);
  alloc2d(&mask, n, n);
  fillWithValues(table_1, N, N);
  fillWithValues(mask, n, n);

  start = std::chrono::high_resolution_clock::now();
  sweep_serial(N, table_1, n, mask, table_2_s);
  stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "Duration of serial: "
      << duration.count() << " milliseconds" << std::endl;

  start = std::chrono::high_resolution_clock::now();
  sweep_parallel(N, table_1, n, mask, table_2_p);
  stop = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "Duration of parallel: "
      << duration.count() << " milliseconds" << std::endl;

  bool equal = compare_arrays(*table_2_s, *table_2_p, N-n+1);
  if (equal)
    std::cout << "Success" << std::endl;
  else
    std::cout << "Failed" << std::endl;

  delete[] *table_1; delete[] table_1;
  delete[] *table_2_s; delete[] table_2_s;
  delete[] *table_2_p; delete[] table_2_p;
  delete[] *mask; delete[] mask;
}
