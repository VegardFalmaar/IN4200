#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main (int nargs, char **args)
{
  int rank, size, len, i, *arr_out, *arr_in;
  int send_to, recv_from;
  MPI_Status status;

  MPI_Init (&nargs, &args);
  MPI_Comm_size (MPI_COMM_WORLD, &size);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);

  if (rank==0)
    len = 100;
  MPI_Bcast (&len, 1, MPI_INT, 0, MPI_COMM_WORLD);

  arr_out = (int*)malloc(len*sizeof(int));
  arr_in = (int*)malloc(len*sizeof(int));

  for (i=0; i<len; i++)
     arr_out[i] = rank*len + i;

  recv_from = (rank+1)%size;
  send_to = (rank-1+size)%size;

  MPI_Sendrecv(arr_out, len, MPI_INT, send_to, 0,
    arr_in, len, MPI_INT, recv_from, 0,
    MPI_COMM_WORLD, &status
  );

  printf("On rank <%d>, first values received=%d %d\n", rank, arr_in[0], arr_in[1]);

  free(arr_in);
  free(arr_out);

  MPI_Finalize ();

  return 0;
}
