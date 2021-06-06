#include <iostream>

using namespace std;

void merge (int m, const int *sorted_sublist1, const int *sorted_sublist2, int *merged_list)
{
  int i,j,k;
  i = 0;
  j = 0;
  k = 0;
  while (i<m && j<m) {
    if (sorted_sublist1[i] <=sorted_sublist2[j]) {
      merged_list[k] = sorted_sublist1[i];
      i++;
    }
    else {
      merged_list[k] = sorted_sublist2[j];
      j++;
    }
    k++;
  }
  if (i < m)
    for (i=i; i<m; i++, k++)
      merged_list[k] = sorted_sublist1[i];
  else
    for (j=j; j<m; j++, k++)
      merged_list[k] = sorted_sublist2[j];
}

int main ()
{
  const int m = 5;
  int *sub1, *sub2, *merged;
  int i;
  sub1 = new int[m];
  sub2 = new int[m];
  merged = new int[2*m];

  for (i=0; i<m; i++) {
    sub1[i] = 1;
    cout << " " << sub1[i];
  }
  cout << endl;

  for (i=0; i<m; i++) {
    sub2[i] = 2*(i-1);
    cout << " " << sub2[i];
  }
  cout << endl;

  merge(m, sub1, sub2, merged);
  for (i=0; i<2*m; i++) {
    cout << " " << merged[i];
  }
  cout << endl;

  delete sub1;
  delete sub2;
  delete merged;

  return 0;
}
