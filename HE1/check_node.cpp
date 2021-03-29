#include <iostream>

using namespace std;

void add_to_cluster(int node_idx, int tau, int *row_ptr, int *col_idx,int *SNN_val, char *cluster)
{
  cluster[node_idx] = 1;
  int start = row_ptr[node_idx];
  int end = row_ptr[node_idx+1];
  for (int i=start; i<end; i++)
    if ((SNN_val[i] >= tau) && !(cluster[col_idx[i]]))
      add_to_cluster(col_idx[i], tau, row_ptr, col_idx, SNN_val, cluster);
}

void check_node (int node_idx, int tau, int N, int *row_ptr, int *col_idx,int *SNN_val)
{
  if ((node_idx < 0) || (node_idx > N-1)) {
    std::cout << "Invalid value of node_idx: " << node_idx
      << std::endl << "Aborting" << std::endl;
    return;
  }
  int start, end;
  char *cluster = new char[N];
  for (int i=0; i<N; i++)
    cluster[i] = 0;

  start = row_ptr[node_idx];
  end = row_ptr[node_idx+1];
  for (int i=start; i<end; i++)
    if ((SNN_val[i] >= tau) && !(cluster[col_idx[i]]))
      add_to_cluster(col_idx[i], tau, row_ptr, col_idx, SNN_val, cluster);

  for (int i=0; i<N; i++)
    if (cluster[i])
      std::cout << i << std::endl;

  delete [] cluster;
}
