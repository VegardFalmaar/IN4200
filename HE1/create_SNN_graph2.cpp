#include <iostream>

using namespace std;

void create_SNN_graph2 (int N, int *row_ptr, int *col_idx, int **SNN_val)
{
  int start, end, to, from_idx, to_idx, matches;

  // allocate memory for SSN_val, length 2*N_edges (same as col_idx)
  int SNN_val_length = row_ptr[N];
  *SNN_val = new int[SNN_val_length];
  for (int i=0; i<SNN_val_length; i++)
    (*SNN_val)[i] = 0;

  /*
  for (int i=0; i<N+1; i++)
    cout << row_ptr[i] << " ";
  cout << endl;

  for (int i=0; i<row_ptr[N]; i++)
    cout << col_idx[i] << " ";
  cout << endl;

  for (int i=0; i<SNN_val_length; i++)
    cout << (*SNN_val)[i] << " ";
  cout << endl;
  */

  int added[N];
  for (int i=0; i<N; i++)
    added[i] = 0;

  for (int i=0; i<N; i++) {
    start = row_ptr[i];
    end = row_ptr[i+1];
    while (col_idx[start] < i)
      start += 1;
    for (int j=start; j<end; j++) {
      to = col_idx[j];
      from_idx = row_ptr[i];
      to_idx = row_ptr[to];

      matches = 0;

      while ((from_idx < end) && (to_idx < row_ptr[to+1])) {
        if (col_idx[from_idx] > col_idx[to_idx])
          to_idx += 1;
        else if (col_idx[from_idx] < col_idx[to_idx])
          from_idx += 1;
        else {
          matches += 1;
          from_idx += 1;
          to_idx += 1;
        }
      }
      // record the number of matches
      (*SNN_val)[j] = matches;
      (*SNN_val)[row_ptr[to] + added[to]] = matches;
      added[to] += 1;
    }
  }
  // for (int i=0; i<SNN_val_length; i++)
    // cout << (*SNN_val)[i] << " ";
  // cout << endl;
}
