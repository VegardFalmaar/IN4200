#include "read_graph_from_file1.cpp"
#include "read_graph_from_file2.cpp"
#include "create_SNN_graph1.cpp"
#include "create_SNN_graph2.cpp"
#include "check_node.cpp"
#include <iostream>

using namespace std;

int test_read_graph_from_file1 (const char *fname);
int test_read_graph_from_file2 (const char *fname);
int test_create_SNN_graph1 (const char *fname);
int test_create_SNN_graph2 (const char *fname);
int test_check_node (const char *fname);

int main ()
{
  int exit_status;
  bool all_passed = true;

  char fname[] = "data/simple_with_invalid.txt";

  exit_status = test_read_graph_from_file1(fname);
  cout << "test_read_graph_from_file1() .... ";
  if (exit_status == 0)
    cout << "Passed" << endl;
  else {
    cout << "Failed" << endl;
    all_passed = false;
  }

  exit_status = test_read_graph_from_file2(fname);
  cout << "test_read_graph_from_file2() .... ";
  if (exit_status == 0)
    cout << "Passed" << endl;
  else {
    cout << "Failed" << endl;
    all_passed = false;
  }

  exit_status = test_create_SNN_graph1(fname);
  cout << "test_create_SNN_graph1() ........ ";
  if (exit_status == 0)
    cout << "Passed" << endl;
  else {
    cout << "Failed" << endl;
    all_passed = false;
  }

  exit_status = test_create_SNN_graph2(fname);
  cout << "test_create_SNN_graph2() ........ ";
  if (exit_status == 0)
    cout << "Passed" << endl;
  else {
    cout << "Failed" << endl;
    all_passed = false;
  }

  exit_status = test_check_node(fname);
  cout << "test_check_node() ............... ";
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

int test_read_graph_from_file1 (const char *fname)
{
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
  delete [] table2D;

  if (passed)
    return 0;
  else
    return 1;
}

int test_read_graph_from_file2 (const char *fname)
{
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

int test_create_SNN_graph1 (const char *fname)
{
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
  delete [] table2D[0];     // deallocate contiguous block of memory
  delete [] table2D;
  delete [] SNN_table[0];
  delete [] SNN_table;

  if (test_passed)
    return 0;
  else
    return 1;
}

int test_create_SNN_graph2 (const char *fname)
{
  int *row_ptr, *col_idx, *SNN_val;
  int N;
  bool test_passed = true;

  read_graph_from_file2(fname, &N, &row_ptr, &col_idx);
  create_SNN_graph2(N, row_ptr, col_idx, &SNN_val);

  int expected[] = {2, 2, 2, 2, 2, 2, 2, 2, 3, 1, 2, 2, 3, 1, 1, 1};
  for (int i=0; i<row_ptr[N]; i++)
    if (SNN_val[i] != expected[i])
      test_passed = false;

  // free the memory
  delete [] row_ptr;
  delete [] col_idx;
  delete [] SNN_val;

  if (test_passed)
    return 0;
  else
    return 1;
}

int test_check_node (const char *fname)
{
  int *row_ptr, *col_idx, *SNN_val;
  int N, node_idx, tau;
  bool test_passed = true;

  read_graph_from_file2(fname, &N, &row_ptr, &col_idx);
  create_SNN_graph2(N, row_ptr, col_idx, &SNN_val);

  char *cluster = new char[N];
  char expected[N];

  node_idx = 1; tau = 1;
  check_node_copy (node_idx, tau, N, row_ptr, col_idx, SNN_val, cluster);
  expected[0] = 1;
  expected[1] = 1;
  expected[2] = 1;
  expected[3] = 1;
  expected[4] = 1;
  for (int i=0; i<N; i++)
    if (cluster[i] != expected[i])
      test_passed = false;

  node_idx = 3; tau = 2;
  check_node_copy (node_idx, tau, N, row_ptr, col_idx, SNN_val, cluster);
  expected[0] = 1;
  expected[1] = 1;
  expected[2] = 1;
  expected[3] = 1;
  expected[4] = 0;
  for (int i=0; i<N; i++)
    if (cluster[i] != expected[i])
      test_passed = false;

  node_idx = 2; tau = 3;
  check_node_copy (node_idx, tau, N, row_ptr, col_idx, SNN_val, cluster);
  expected[0] = 0;
  expected[1] = 0;
  expected[2] = 1;
  expected[3] = 1;
  expected[4] = 0;
  for (int i=0; i<N; i++)
    if (cluster[i] != expected[i])
      test_passed = false;

  node_idx = 4; tau = 1;
  check_node_copy (node_idx, tau, N, row_ptr, col_idx, SNN_val, cluster);
  expected[0] = 1;
  expected[1] = 1;
  expected[2] = 1;
  expected[3] = 1;
  expected[4] = 1;
  for (int i=0; i<N; i++)
    if (cluster[i] != expected[i])
      test_passed = false;

  node_idx = 4; tau = 2;
  check_node_copy (node_idx, tau, N, row_ptr, col_idx, SNN_val, cluster);
  expected[0] = 0;
  expected[1] = 0;
  expected[2] = 0;
  expected[3] = 0;
  expected[4] = 0;
  for (int i=0; i<N; i++)
    if (cluster[i] != expected[i])
      test_passed = false;

  node_idx = 0; tau = 3;
  check_node_copy (node_idx, tau, N, row_ptr, col_idx, SNN_val, cluster);
  expected[0] = 0;
  expected[1] = 0;
  expected[2] = 0;
  expected[3] = 0;
  expected[4] = 0;
  for (int i=0; i<N; i++)
    if (cluster[i] != expected[i])
      test_passed = false;

  // free the memory
  delete [] row_ptr;
  delete [] col_idx;
  delete [] SNN_val;
  delete [] cluster;

  if (test_passed)
    return 0;
  else
    return 1;
}
