#ifdef _OPENMP
  #include <omp.h>
#endif
#include <iostream>

void create_SNN_graph2 (int N, int *row_ptr, int *col_idx, int **SNN_val)
{
  // allocate memory for SSN_val, length 2*N_edges (same as col_idx)
  int SNN_val_length = row_ptr[N];
  *SNN_val = new int[SNN_val_length];
  for (int i=0; i<SNN_val_length; i++)
    (*SNN_val)[i] = 0;

  int mirror_idx;

  #ifdef _OPENMP
    // set the number of parallel threads to the highest number available
    omp_set_num_threads(omp_get_max_threads());
    #pragma omp parallel for schedule(static, 1) if (N > 100)
  #endif
  for (int i=0; i<N-1; i++) {
    int start, end, to, from_idx, to_idx, matches;
    start = row_ptr[i];
    end = row_ptr[i+1];
    while ((col_idx[start] < i) && (start < end))
      start++;
    for (int j=start; j<end; j++) {
      from_idx = row_ptr[i];
      to = col_idx[j];
      to_idx = row_ptr[to];

      mirror_idx = 0;
      matches = 0;

      while ((from_idx < end) && (to_idx < row_ptr[to+1])) {
        if (i == col_idx[to_idx]) {
          // std::cout << j << " " << to_idx << std::endl;
          mirror_idx = to_idx;
          to_idx++;
        } else if (col_idx[from_idx] > col_idx[to_idx])
          to_idx++;
        else if (col_idx[from_idx] < col_idx[to_idx])
          from_idx++;
        else {
          matches++;
          from_idx++;
          to_idx++;
        }
      }
      // record the number of matches
      (*SNN_val)[j] = matches;
      (*SNN_val)[mirror_idx] = matches;
    }
  }
}
