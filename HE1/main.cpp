#include <chrono>
#include <iostream>
#include "read_graph_from_file1.cpp"
#include "read_graph_from_file2.cpp"
#include "create_SNN_graph1.cpp"
#include "create_SNN_graph2.cpp"

using namespace std;

void run_create_SNN_graph1 (char *fname)
{
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
  delete [] table2D[0];     // deallocate contiguous block of memory
  delete [] table2D;        // deallocate the pointer to the array
  delete [] SNN_table[0];   // deallocate contiguous block of memory
  delete [] SNN_table;      // deallocate the pointer to the array
}

void run_create_SNN_graph2 (char *fname)
{
  int *row_ptr, *col_idx, *SNN_val;
  int N;

  read_graph_from_file2(fname, &N, &row_ptr, &col_idx);

  // time the code
  auto start = std::chrono::high_resolution_clock::now();
  create_SNN_graph2(N, row_ptr, col_idx, &SNN_val);
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "Duration: " << duration.count() << " milliseconds" << std::endl;

  // free the memory
  delete [] row_ptr;
  delete [] col_idx;
  delete [] SNN_val;
}

int main ()
{
  char fname[] = "data/facebook_combined.txt";
  // char fname[] = "data/simple_with_invalid.txt";

  run_create_SNN_graph1(fname);
  run_create_SNN_graph2(fname);

  return 0;
}
