#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "utils.h"

void read_graph_from_file (char *, int *, int **, int **, double **);
void PageRank_iterations (int, int *, int *, double *, double, double, double *);
void top_n_webpages (int, double *, int );

int main(int argc, char const *argv[]) {
  int N=0, *row_ptr, *col_idx;
  double *val;
  // read_graph_from_file("simple-webgraph.txt", &N, &row_ptr, &col_idx, &val);
  read_graph_from_file("simple-unsorted-invalid.txt", &N, &row_ptr, &col_idx, &val);
  free(row_ptr);
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
  size_t i, j, k;
  int edges = 0, invalid_entries = 0, fromID, toID, idx_start, idx_stop;
  bool entry_is_valid;

  FILE *fp = fopen(filename, "r");

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

  int *fromID_a = malloc(edges*sizeof(int));
  int *toID_a = malloc(edges*sizeof(int));
  int *num_out_links;
  alloc_full_i(&num_out_links, *N, 0);
  alloc_full_i(row_ptr, *N + 1, 0);

  for (i=0; i<edges; i++) {
    fscanf(fp, "%d %d", &fromID, &toID);
    entry_is_valid = (fromID != toID);
    if (entry_is_valid) {
      fromID_a[i - invalid_entries] = fromID;
      toID_a[i - invalid_entries] = toID;
      num_out_links[fromID]++;
      (*row_ptr)[toID + 1]++;
    } else
      invalid_entries++;
  }
  printf("Excluding %d invalid entries\n", invalid_entries);
  edges -= invalid_entries;
  printvec_i(num_out_links, *N);

  for (i=0; i<*N; i++)
    (*row_ptr)[i+1] += (*row_ptr)[i];

  alloc_full_i(col_idx, edges, -1);
  *val = malloc(edges*sizeof(double));

  for (i=0; i<edges; i++) {
    fromID = fromID_a[i];
    toID = toID_a[i];
    idx_start = (*row_ptr)[toID];
    idx_stop = (*row_ptr)[toID + 1];
    for (j=idx_start; j<idx_stop; j++) {
      if ((*col_idx)[j] == -1)
        break;
      if (fromID < (*col_idx)[j]) {
        for (k=idx_stop-1; k>j; k--) {
          (*col_idx)[k] = (*col_idx)[k-1];
          (*val)[k] = (*val)[k-1];
        }
        break;
      }
    }
    (*col_idx)[j] = fromID;
    (*val)[j] = 1.0 / (double) num_out_links[fromID];
  }

  // printvec_i(num_out_links, *N);
  printvec_i(*row_ptr, *N + 1);
  printvec_i(*col_idx, edges);
  printvec_d(*val, edges);

  free(num_out_links);
  free(fromID_a);
  free(toID_a);

  // close the file
  fclose(fp);
}
