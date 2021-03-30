#include <iostream>

void read_graph_from_file2 (const char *filename, int *N, int **row_ptr, int **col_idx)
{
  int N_edges, from_node_id, to_node_id, illegal_values;
  bool valid_entry;

  // open file with initial conditions
  FILE *input_file = fopen(filename, "r");
  // read past the first two lines,
  // literal # followed by an arbitrary number of letters not #
  (void) (fscanf(input_file, "# %*[^#]") + 1);  // ignore the return value of fscanf
  (void) (fscanf(input_file, "# %*[^#]") + 1);
  // read the number of nodes and edges
  (void) (fscanf(input_file, "# Nodes: %d Edges: %d ", N, &N_edges) + 1);
  // read past line 4
  (void) (fscanf(input_file, "# FromNodeId ToNodeId") + 1);

  // array to hold the raw data from the input file
  int from_array[N_edges];
  int to_array[N_edges];
  int node_links[*N];
  for (int i=0; i<*N; i++)
    node_links[i] = 0;

  illegal_values = 0;
  for (int i=0; i<N_edges; i++) {
    (void) (fscanf(input_file, "%d %d", &from_node_id, &to_node_id) + 1);
    valid_entry =
      (from_node_id < *N)
      && (to_node_id < *N)
      && (from_node_id >= 0)
      && (to_node_id >= 0)
      && (from_node_id != to_node_id);
    if (valid_entry) {
      node_links[from_node_id] += 1;
      node_links[to_node_id] += 1;
      from_array[i-illegal_values] = from_node_id;
      to_array[i-illegal_values] = to_node_id;
    } else {
      illegal_values+= 1;
    }
  }
  // close the input file
  fclose(input_file);
  std::cout << illegal_values << " illegal values excluded" << std::endl;

  N_edges -= illegal_values;

  // allocate memory for row_ptr, and fill it with the appropriate values
  *row_ptr = new int[*N+1];
  (*row_ptr)[0] = 0;
  for (int i=1; i<*N+1; i++)
    (*row_ptr)[i] = (*row_ptr)[i-1] + node_links[i-1];

  // allocate memory for col_idx, and fill it with zeros
  *col_idx = new int[2*N_edges];
  for (int i=0; i<2*N_edges; i++)
    (*col_idx)[i] = 0;

  int from, to, col;
  for (int i=0; i<N_edges; i++) {
    from = from_array[i];
    to = to_array[i];

    // insert the value showing that "from" is connected to "to"
    col = (*row_ptr)[from+1] - 1;
    while ((*col_idx)[col] > to)
      col -= 1;
    for (int j=(*row_ptr)[from]; j<col; j++)
      (*col_idx)[j] = (*col_idx)[j+1];
    (*col_idx)[col] = to;

    // insert the value showing that "to" is connected to "from"
    col = (*row_ptr)[to+1] - 1;
    while ((*col_idx)[col] > from)
      col -= 1;
    for (int j=(*row_ptr)[to]; j<col; j++)
      (*col_idx)[j] = (*col_idx)[j+1];
    (*col_idx)[col] = from;
  }
}
