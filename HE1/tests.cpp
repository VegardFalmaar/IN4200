#include "read_graph_from_file1.cpp"
#include "read_graph_from_file2.cpp"
#include "create_SNN_graph1.cpp"
#include "create_SNN_graph2.cpp"
#include <iostream>

using namespace std;

int test_read_graph_from_file1 ();
int test_read_graph_from_file2 ();
int test_create_SNN_graph1 ();
int test_create_SNN_graph2 ();

int main ()
{
  int exit_status;
  bool all_passed = true;

  exit_status = test_read_graph_from_file1();
  cout << "test_read_graph_from_file1() .... ";
  if (exit_status == 0)
    cout << "Passed" << endl;
  else {
    cout << "Failed" << endl;
    all_passed = false;
  }

  exit_status = test_read_graph_from_file2();
  cout << "test_read_graph_from_file2() .... ";
  if (exit_status == 0)
    cout << "Passed" << endl;
  else {
    cout << "Failed" << endl;
    all_passed = false;
  }

  exit_status = test_create_SNN_graph1();
  cout << "test_create_SNN_graph1() ........ ";
  if (exit_status == 0)
    cout << "Passed" << endl;
  else {
    cout << "Failed" << endl;
    all_passed = false;
  }

  cout << endl;

  if (all_passed)
    return 0;
  else
    return 1;
}

int test_read_graph_from_file1 ()
{
  char fname[] = "data/simple_with_invalid.txt";

  char **table2D;
  int N;
  bool passed = true;

  read_graph_from_file1(fname, &N, &table2D);
  char expected[] = {0,1,1,1,0, 1,0,1,1,0, 1,1,0,1,1, 1,1,1,0,1, 0,0,1,1,0};
  if (N != 5)
    passed = false;
  for (int i=0; i<N*N; i++)
    if (table2D[0][i] != expected[i])
      passed = false;

  // free the memory
  delete [] table2D[0];   // deallocate contiguous block of memory
  delete [] table2D;        // deallocate the pointer to the array

  if (passed)
    return 0;
  else
    return 1;
}

int test_read_graph_from_file2 ()
{
  char fname[] = "data/simple_with_invalid.txt";

  int *row_ptr, *col_idx;
  int N;
  bool passed = true;

  read_graph_from_file2(fname, &N, &row_ptr, &col_idx);
  int expected_row[] = {0, 3, 6, 10, 14, 16};
  int expected_col[] = {1, 2, 3, 0, 2, 3, 0, 1, 3, 4, 0, 1, 2, 4, 2, 3};
  if (N != 5)
    passed = false;
  for (int i=0; i<N+1; i++)
    if (row_ptr[i] != expected_row[i])
      passed = false;
  for (int i=0; i<16; i++)
    if (col_idx[i] != expected_col[i])
      passed = false;

  // free the memory
  delete [] row_ptr;
  delete [] col_idx;

  if (passed)
    return 0;
  else
    return 1;
}

int test_create_SNN_graph1 ()
{
  char fname[] = "data/simple_with_invalid.txt";

  int **SNN_table;
  char **table2D;
  int N;
  bool test_passed = true;

  read_graph_from_file1(fname, &N, &table2D);
  create_SNN_graph1(N, table2D, &SNN_table);

  int expected[] = {0,2,2,2,0, 2,0,2,2,0, 2,2,0,3,1, 2,2,3,0,1, 0,0,1,1,0};
  for (int i=0; i<N*N; i++)
    if (SNN_table[0][i] != expected[i])
      test_passed = false;

  // free the memory
  delete [] table2D[0];   // deallocate contiguous block of memory
  delete [] table2D;        // deallocate the pointer to the array
  delete [] SNN_table[0];   // deallocate contiguous block of memory
  delete [] SNN_table;        // deallocate the pointer to the array

  if (test_passed)
    return 0;
  else
    return 1;
}
