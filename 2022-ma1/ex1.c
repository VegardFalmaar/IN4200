#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "utils.h"

void read_graph_from_file (char *filename, int *N, int **row_ptr, int **col_idx, double **val);
void PageRank_iterations (int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores);
void top_n_webpages (int N, double *scores, int n);

int main(int argc, char const *argv[]) {
  int N=0, *row_ptr, *col_idx;
  double *val;
  read_graph_from_file("simple-webgraph.txt", &N, &row_ptr, &col_idx, &val);
  free(col_idx);
  return 0;
}

/*
This function reads a text file that contains a web graph, so that the
corresponding hyperlink matrix is built up in the CRS forma
*/
void read_graph_from_file (
    char *filename, int *N, int **row_ptr, int **col_idx, double **val
) {
  int edges = 0;
  FILE *fp = fopen(filename, "r");
  double **hyper_mat;

  if (fp == NULL){
    printf("Error: could not open file %s", filename);
    exit(0);
  }
  // read past the two leading lines of text
  fscanf(fp, "# %*[^#]");
  fscanf(fp, "# %*[^#]");

  fscanf(fp, "# Nodes: %d Edges: %d " , N, &edges);

  // read past the fourth line
  fscanf(fp, "# FromNodeId    ToNodeId");

  int *fromID = malloc(edges*sizeof(int));
  int *toID = malloc(edges*sizeof(int));
  int *num_out_links = malloc((*N)*sizeof(int));

  zeros2D_d(&hyper_mat, *N, *N);
  zeros_i(col_idx, *N + 1);

  for (size_t i=0; i<edges; i++) {
    fscanf(fp, "%d %d", &(fromID[i]), &(toID[i]));
    num_out_links[fromID[i]]++;
    (*col_idx)[toID[i] + 1]++;
  }

  for (size_t i=0; i<*N; i++)
    (*col_idx)[i+1] += (*col_idx)[i];

  for (size_t i=0; i<edges; i++)
    hyper_mat[toID[i]][fromID[i]] = 1.0 / (double) num_out_links[fromID[i]];

  printmat_d(hyper_mat, *N, *N);
  printvec_i(num_out_links, *N);
  printvec_i(*col_idx, *N + 1);

  free2D(hyper_mat);
  free(num_out_links);
  free(fromID);
  free(toID);

  // close the file
  fclose(fp);
}
