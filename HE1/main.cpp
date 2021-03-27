#include <chrono>
#include "read_graph_from_file1.cpp"
#include "create_SNN_graph1.cpp"

int main ()
{
  char fname[] = "data/facebook_combined.txt";
  int **SNN_table;
  char **table2D;
  int N;

  read_graph_from_file1(fname, &N, &table2D);

  // time the code
  auto start = std::chrono::high_resolution_clock::now();
  create_SNN_graph1(N, table2D, &SNN_table);
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "Duration: " << duration.count() << " milliseconds" << std::endl;

  // for (int i=0; i<N*N; i++)
    // std::cout << SNN_table[0][i] << std::endl;

  // free the memory
  delete [] table2D[0];   // deallocate contiguous block of memory
  delete [] table2D;        // deallocate the pointer to the array
  delete [] SNN_table[0];   // deallocate contiguous block of memory
  delete [] SNN_table;        // deallocate the pointer to the array
}
