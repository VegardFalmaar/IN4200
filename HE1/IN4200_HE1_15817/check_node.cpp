#include <iostream>

void add_to_cluster(
    const int node_idx,
    const int tau,
    const int *row_ptr,
    const int *col_idx,
    const int *SNN_val,
    char *cluster
) {
  cluster[node_idx] = 1;
  const int start = row_ptr[node_idx];
  const int end = row_ptr[node_idx+1];
  for (int i=start; i<end; i++)
    if ((!cluster[col_idx[i]]) && (SNN_val[i] >= tau))
      add_to_cluster(col_idx[i], tau, row_ptr, col_idx, SNN_val, cluster);
}

void check_node (
    const int node_idx,
    const int tau,
    const int N,
    const int *row_ptr,
    const int *col_idx,
    const int *SNN_val
) {
  if ((node_idx < 0) || (node_idx > N-1)) {
    std::cout << "Invalid value of node_idx: " << node_idx
      << std::endl << "Aborting" << std::endl;
    return;
  }
  bool can_form_cluster = false;
  char *cluster = new char[N];
  for (int i=0; i<N; i++)
    cluster[i] = 0;

  const int start = row_ptr[node_idx];
  const int end = row_ptr[node_idx+1];
  for (int i=start; i<end; i++)
    if ((SNN_val[i] >= tau) && (!cluster[col_idx[i]])) {
      add_to_cluster(col_idx[i], tau, row_ptr, col_idx, SNN_val, cluster);
      cluster[node_idx] = 1;
      can_form_cluster = true;
    }

  for (int i=0; i<N; i++)
    if (cluster[i])
      std::cout << i << std::endl;

  if (!can_form_cluster)
      std::cout << "Node " << node_idx
        << " cannot from a cluster with tau = " << tau << std::endl;

  delete [] cluster;
}

void check_node_copy (
    const int node_idx,
    const int tau,
    const int N,
    const int *row_ptr,
    const int *col_idx,
    const int *SNN_val,
    char *cluster
) {
  for (int i=0; i<N; i++)
    cluster[i] = 0;

  const int start = row_ptr[node_idx];
  const int end = row_ptr[node_idx+1];
  for (int i=start; i<end; i++)
    if ((SNN_val[i] >= tau) && (!cluster[col_idx[i]])) {
      add_to_cluster(col_idx[i], tau, row_ptr, col_idx, SNN_val, cluster);
      cluster[node_idx] = 1;
    }
}
