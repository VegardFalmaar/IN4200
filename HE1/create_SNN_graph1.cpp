#ifdef _OPENMP
  #include <omp.h>
#endif

void create_SNN_graph1 (int N, char **table2D, int ***SNN_table)
{
  int dot_product;

  // allocate a continuous array for the 2D SNN matrix and fill it with zeros
  int *contig_array = new int[N*N];
  for (int i=0; i<N*N; i++)
    contig_array[i] = 0;
  *SNN_table = new int*[N];
  for (int i=0; i<N; i++)
    (*SNN_table)[i] = &contig_array[i*N];

  #ifdef _OPENMP
    // set the number of parallel threads to the highest number available
    omp_set_num_threads(omp_get_max_threads());
    #pragma omp parallel for private(dot_product) schedule(static, 1) if (N > 100)
  #endif
  for (int i=0; i<N; i++) {
    for (int j=i+1; j<N; j++) {
      if (table2D[i][j]) {
        dot_product = 0;
        for (int k=0; k<N; k++)
          dot_product += table2D[i][k]*table2D[j][k];
        (*SNN_table)[i][j] = dot_product;
        (*SNN_table)[j][i] = dot_product;
      }
    }
  }
}
