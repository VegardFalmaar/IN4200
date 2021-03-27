#include <iostream>

void update_indices (
    int *N, int N_edges, int **row_ptr, int **col_idx, int from, int to
) {
  int col;
  col = (*row_ptr)[from];
  for (int j=from+1; j<*N; j++)
    (*row_ptr)[j] += 1;
  while (((*col_idx)[col] < to) && (col < (*row_ptr)[from+1] - 1))
    col += 1;
  for (int j=2*N_edges-1; j>col; j--)
    (*col_idx)[j] = (*col_idx)[j-1];
  (*col_idx)[col] = to;
}

void print_arrays(int *N, int N_edges, int **row_ptr, int **col_idx)
{
  for (int i=0; i<*N+1; i++)
    std::cout << " " << (*row_ptr)[i];
  std::cout << std::endl;
  for (int i=0; i<2*N_edges; i++)
    std::cout << " " << (*col_idx)[i];
  std::cout << std::endl;
}

void read_graph_from_file2 (char *filename, int *N, int **row_ptr, int **col_idx)
{
  int N_edges, from_node_id, to_node_id;
  bool valid_entry;

  // open file with initial conditions
  FILE *input_file = fopen(filename, "r");
  // read past the first two lines,
  // literal # followed by an arbitrary number of letters not #
  fscanf(input_file, "# %*[^#]");
  fscanf(input_file, "# %*[^#]");
  // read the number of nodes and edges
  fscanf(input_file, "# Nodes: %d Edges: %d ", N, &N_edges);
  // read past line 4
  fscanf(input_file, "# FromNodeId ToNodeId");

  // allocate contiguous memory for the 2D matrix and fill it with zeros
  *row_ptr = new int[*N+1];
  *col_idx = new int[2*N_edges];
  for (int i=0; i<*N; i++)
    (*row_ptr)[i] = 0;
  (*row_ptr)[*N] = 2*N_edges;
  for (int i=0; i<2*N_edges; i++)
    (*col_idx)[i] = 0;

  // print the arrays for testing
  print_arrays(N, N_edges, row_ptr, col_idx);

  for (int i=0; i<N_edges; i++) {
    fscanf(input_file, "%d %d", &from_node_id, &to_node_id);
    valid_entry =
      (from_node_id < *N)
      && (to_node_id < *N)
      && (from_node_id != to_node_id);
    if (valid_entry) {
      // update_indices(N, N_edges, row_ptr, col_idx, from_node_id, to_node_id);
      // update_indices(N, N_edges, row_ptr, col_idx, to_node_id, from_node_id);
    } else
      std::cout << "Illegal value excluded: "
        << from_node_id << " " << to_node_id << std::endl;
  }

  // testing update_indices
  update_indices(N, N_edges, row_ptr, col_idx, 1, 2);
  print_arrays(N, N_edges, row_ptr, col_idx);
  update_indices(N, N_edges, row_ptr, col_idx, 2, 1);
  print_arrays(N, N_edges, row_ptr, col_idx);
  update_indices(N, N_edges, row_ptr, col_idx, 3, 1);
  print_arrays(N, N_edges, row_ptr, col_idx);
  update_indices(N, N_edges, row_ptr, col_idx, 1, 3);
  print_arrays(N, N_edges, row_ptr, col_idx);

  // print the arrays filled with values
  print_arrays(N, N_edges, row_ptr, col_idx);

  // close the input file
  fclose(input_file);
}

int main () {
  char fname[] = "data/simple.txt";
  // char fname[] = "data/simple_with_invalid.txt";

  int *row_ptr, *col_idx;
  int N;
  read_graph_from_file2(fname, &N, &row_ptr, &col_idx);

  // free the memory
  delete [] row_ptr;
  delete [] col_idx;

  return 0;
}
