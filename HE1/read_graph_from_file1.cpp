#include <iostream>

void read_graph_from_file1 (const char *filename, int *N, char ***table2D)
{
  int N_edges, from_node_id, to_node_id, illegal_values;
  bool valid_entry;

  // open file with initial conditions
  FILE *input_file = fopen(filename, "r");
  // read past the first two lines
  // literal # followed by an arbitrary number of letters not #
  (void) (fscanf(input_file, "# %*[^#]") + 1);
  (void) (fscanf(input_file, "# %*[^#]") + 1);
  // read the number of nodes and edges
  (void) (fscanf(input_file, "# Nodes: %d Edges: %d ", N, &N_edges) + 1);
  // read past line 4
  (void) (fscanf(input_file, "# FromNodeId ToNodeId") + 1);

  // allocate contiguous memory for the 2D matrix and fill it with zeros
  char *contig_array = new char[(*N)*(*N)];
  for (int i=0; i<(*N)*(*N); i++)
    contig_array[i] = 0;
  *table2D = new char*[*N];
  for (int i=0; i<*N; i++)
    (*table2D)[i] = &contig_array[i*(*N)];

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
      (*table2D)[from_node_id][to_node_id] = 1;
      (*table2D)[to_node_id][from_node_id] = 1;
    } else
      illegal_values += 1;
  }
  std::cout << illegal_values << " illegal values excluded" << std::endl;
  // close the input file
  fclose(input_file);
}
