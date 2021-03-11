#include <iostream>

using namespace std;

void print (int **arr, int, int);

int main () {
  uint32_t m = 10, n = 5;

  int **arr = new int*[m];
  for (int i = 0; i < m; i++)
    arr[i] = new int[n];

  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
      arr[i][j] = 0;

  print(arr, m, n);

  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      arr[i][j] = 1;

  print(arr, m, n);

  for (int i = 0; i < m; i++)
    delete [] arr[i];
  delete [] arr;

  return 0;
}

void print (int **arr, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++)
      cout << arr[i][j] << " ";
    cout << endl;
  }
}
