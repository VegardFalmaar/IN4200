#include <iostream>

void print_matrix(char*** table2D, int* N)
{
  for (int i=0; i<*N; i++) {
    for (int j=0; j<*N; j++)
      printf(" %d", (*table2D)[i][j]);
    std::cout << std::endl;
  }
}

void read_graph_from_file1 (const char* filename, int* N, char*** table2D)
{
  int N_edges, from_node_id, to_node_id;
  bool valid_entry;

  // open file with initial conditions
  FILE *input_file = fopen(filename, "r");
  // read past the first two lines
  // literal # followed by an arbitrary number of letters not #
  fscanf(input_file, "# %*[^#]");
  fscanf(input_file, "# %*[^#]");
  // read the number of nodes and edges
  fscanf(input_file, "# Nodes: %d Edges: %d ", N, &N_edges);
  // read past line 4
  fscanf(input_file, "# FromNodeId ToNodeId");

  // allocate contiguous memory for the 2D matrix and fill it with zeros
  char* contig_array = new char[(*N)*(*N)];
  for (int i=0; i<(*N)*(*N); i++)
    contig_array[i] = 0;
  *table2D = new char*[*N];
  for (int i=0; i<*N; i++)
    (*table2D)[i] = &contig_array[i*(*N)];

  // print the matrix for testing
  print_matrix(table2D, N);

  for (int i=0; i<N_edges; i++) {
    fscanf(input_file, "%d %d", &from_node_id, &to_node_id);
    valid_entry =
      (from_node_id < *N)
      && (to_node_id < *N)
      && (from_node_id != to_node_id);
    if (valid_entry) {
      (*table2D)[from_node_id][to_node_id] = 1;
      (*table2D)[to_node_id][from_node_id] = 1;
    } else
      std::cout << "Illegal value excluded: "
        << from_node_id << " " << to_node_id << std::endl;
  }

  // print the 2D arrat filled with values
  std::cout << std::endl;
  print_matrix(table2D, N);
  std::cout << std::endl;

  // close the input file
  fclose(input_file);
}

int main () {
  char fname[] = "data/simple.txt";
  // char fname[] = "data/simple_with_invalid.txt";
  // std::cout << "String: " << fname << std::endl;

  char** table2D;
  int N;
  N = 10;
  // std::cout << N << std::endl;
  read_graph_from_file1(fname, &N, &table2D);
  // read_graph_from_file2(fname, &N, &table2D);
  // std::cout << N << std::endl;

  // free the memory
  delete [] (table2D)[0];   // deallocate contiguous block of memory
  delete [] table2D;        // deallocate the pointer to the array

  return 0;
}
