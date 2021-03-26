#include <iostream>
#include <fstream>
#include <string>

void read_graph_from_file1 (const char* filename, int* N, char*** table2D)
{
  int N_nodes, from_node_id, to_node_id;
  bool valid_entry;

  // open file with initial conditions
  FILE *input_file = fopen(filename, "r");
  // read past the first two lines
  // literal # followed by an arbitrary number of letters not #
  fscanf(input_file, "# %*[^#]");
  fscanf(input_file, "# %*[^#]");
  // read the number of nodes and edges
  fscanf(input_file, "# Nodes: %d Edges: %d ", &N_nodes, N);
  // read past line 4
  fscanf(input_file, "# FromNodeId ToNodeId");

  for (int i=0; i<*N; i++) {
    fscanf(input_file, "%d %d", &from_node_id, &to_node_id);
    valid_entry =
      (from_node_id < N_nodes)
      && (to_node_id < N_nodes)
      && (from_node_id != to_node_id);
    if (valid_entry) {
      std::cout << from_node_id
        << " -> " << to_node_id << std::endl;
    } else
      std::cout << "Illegal value excluded: "
        << from_node_id << " " << to_node_id << std::endl;
  }
  // close the input file
  fclose(input_file);
}

int main () {
  // char fname[] = "data/simple.txt";
  char fname[] = "data/simple_with_invalid.txt";
  // std::cout << "String: " << fname << std::endl;

  char** table2D;
  int N;
  N = 10;
  // std::cout << N << std::endl;
  read_graph_from_file1(fname, &N, &table2D);
  // read_graph_from_file2(fname, &N, &table2D);
  // std::cout << N << std::endl;

  return 0;
}
