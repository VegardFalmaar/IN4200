void compare_split (
  const int m, int *my_sublist, const int my_MPI_rank, const int other_MPI_rank
) {
  int *other_sublist = new int[m];
  int *merged_list = new int[2*m];
  int i, i_start;
  const int tag = 0;  // arbitrary in this case
  MPI_Status recv_status;

  if (my_MPI_rank < other_MPI_rank) {
    MPI_Recv(
      other_sublist, m, MPI_INT, other_MPI_rank,
      tag, MPI_COMM_WORLD, &recv_status
    );
    MPI_Send(my_sublist, m, MPI_INT, other_MPI_rank, tag, MPI_COMM_WORLD);
  } else {
    MPI_Send(my_sublist, m, MPI_INT, other_MPI_rank, tag, MPI_COMM_WORLD);
    MPI_Recv(
      other_sublist, m, MPI_INT, other_MPI_rank,
      tag, MPI_COMM_WORLD, &recv_status
    );
  }
  merge(m, my_sublist, other_sublist, merged_list);

  i_start = (my_MPI_rank > other_MPI_rank)*m;
  for (i=0; i<m, i++)
    my_sublist[i] = merged_list[i+i_start];

  delete other_sublist;
  delete merged_list;
}

void odd_even_block_parallel (const int m, int *sublist)
{
  int i, my_id, num_procs;

  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  const bool odd_id = (my_id%2 == 1);

  for (i=1; i<=num_procs; i++) {
    if (i%2 == 1) {
      if (odd_id)
        compare_split(m, sublist, my_id, my_id+1);
      else
        compare_split(m, sublist, my_id, my_id-1);
    } else {
      if (odd_id)
        compare_split(m, sublist, my_id, my_id-1);
      else
        compare_split(m, sublist, my_id, my_id+1);
    }
  }
}
