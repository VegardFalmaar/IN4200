void create_SNN_graph1 (int N, char **table2D, int ***SNN_table)
{
  int dot_product;
  int *contig_array = new int[N*N];
  for (int i=0; i<N*N; i++)
    contig_array[i] = 0;
  *SNN_table = new int*[N];
  for (int i=0; i<N; i++)
    (*SNN_table)[i] = &contig_array[i*N];

  for (int i=0; i<N; i++) {
    for (int j=i+1; j<N; j++) {
      dot_product = 0;
      for (int k=0; k<N; k++)
        dot_product += table2D[i][k]*table2D[j][k];
      dot_product *= table2D[i][j];
      (*SNN_table)[i][j] = dot_product;
      (*SNN_table)[j][i] = dot_product;
    }
  }
}