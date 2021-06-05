void compare_split (
  int m, int *my_sublist, int my_MPI_rank, int other_MPI_rank
) {
  int *other_sublist = new int[m];
  int *merged_list = new int[2*m];
  int i, i_start;
  if (my_MPI_rank < other_MPI_rank) {
    MPI_Recv(
      other_sublist, m, MPI_INT, other_MPI_rank,
      tag, MPI_COMM_WORLD, &status
    );
    MPI_Send(my_sublist, m, MPI_INT, other_MPI_rank, tag, MPI_COMM_WORLD);
  }
  else {
    MPI_Send(my_sublist, m, MPI_INT, other_MPI_rank, tag, MPI_COMM_WORLD);
    MPI_Recv(
      other_sublist, m, MPI_INT, other_MPI_rank,
      tag, MPI_COMM_WORLD, &status
    );
  }
  merge(m, my_sublist, other_sublist, merged_list);

  i_start = (my_MPI_rank > other_MPI_rank)*m;
  for (i=0; i<m, i++)
    my_sublist[i] = merged_list[i+i_start];

  delete other_sublist;
  delete merged_list;
}
