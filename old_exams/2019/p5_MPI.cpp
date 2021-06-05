#include <cstddef>
#include <iostream>
#include <string.h>
#include <mpi.h>
#include <string>

using namespace std;

int my_start (const int my_rank, const int num_procs, const int len_t)
{
  return (my_rank*len_t)/num_procs;
}

int my_size (
  const int my_rank, const int num_procs, const int len_t, const int len_p
) {
  const int start =  my_start(my_rank, num_procs, len_t);
  const int stop = my_start(my_rank+1, num_procs, len_t) + (len_p-1)*(my_rank < num_procs-1);
  return stop - start;
}

int MPI_count_occurence (char *text_string, char *pattern)
{
  int my_rank, num_procs;
  int len_p=0, len_t=0;
  int i, matches=0, total_matches=0;

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  int transfer_count_arr[num_procs], displacements_arr[num_procs];

  if (my_rank == 0) {
    len_p = strlen(pattern);
    len_t = strlen(text_string);
    for (i=0; i<num_procs; i++) {
      displacements_arr[i] = my_start(i, num_procs, len_t);
      transfer_count_arr[i] = my_size(i, num_procs, len_t, len_p);
    }
  }

  // share the lengths with the other processes
  MPI_Bcast(&len_t, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&len_p, 1, MPI_INT, 0, MPI_COMM_WORLD);

  const int my_len_t = my_size(my_rank, num_procs, len_t, len_p);

  if (my_rank > 0) {
    text_string = new char[my_len_t];
    pattern = new char[len_p];
  }

  // share the input
  MPI_Bcast(pattern, len_p, MPI_CHAR, 0, MPI_COMM_WORLD);
  MPI_Scatterv(
    text_string, transfer_count_arr, displacements_arr, MPI_CHAR,
    text_string, my_len_t, MPI_CHAR, 0, MPI_COMM_WORLD
  );

  for (i=0; i<=my_len_t-len_p; i++)
    if (strncmp(&(text_string[i]), pattern, len_p) == 0)
      matches++;

  if (my_rank > 0) {
    delete text_string; text_string = NULL;
    delete pattern; pattern = NULL;
  }

  MPI_Reduce(&matches, &total_matches, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return total_matches;
}

int main (int nargs, char **args)
{
  char *text_string=NULL, *pattern=NULL;
  int my_rank, matches;
  string s1, s2;

  MPI_Init(&nargs, &args);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (my_rank == 0) {
    text_string = new char[30];
    pattern = new char[5];
    s1 = "ATTTGCGCAGACCTAAGCA";
    s2 = "GCA";
    strcpy(text_string, s1.c_str());
    strcpy(pattern, s2.c_str());
  }
  matches = MPI_count_occurence(text_string, pattern);
  if (my_rank == 0)
    cout << matches << " should equal 2" << endl;

  // /*
  if (my_rank == 0) {
    s1 = "AAABCDEFGAAGEREAANMT";
    s2 = "AA";
    strcpy(text_string, s1.c_str());
    strcpy(pattern, s2.c_str());
  }
  matches = MPI_count_occurence(text_string, pattern);
  if (my_rank == 0)
    cout << matches << " should equal 4" << endl;

  if (my_rank == 0) {
    s1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    s2 = "BTTT";
    strcpy(text_string, s1.c_str());
    strcpy(pattern, s2.c_str());
  }
  matches = MPI_count_occurence(text_string, pattern);
  if (my_rank == 0)
    cout << matches << " should equal 0" << endl;
  // */

  if (my_rank == 0) {
    delete text_string; delete pattern;
  }

  MPI_Finalize();

  return 0;
}
