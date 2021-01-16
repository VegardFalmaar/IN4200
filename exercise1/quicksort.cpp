#include <iostream>
using namespace std;

void swap(int *a, int *b)
{
  int t=*a; *a=*b; *b=t;
}
void sort(int arr[], int beg, int end)
{
  if (end > beg + 1) {
    int piv = arr[beg], l = beg + 1, r = end;
    while (l < r) {
      if (arr[l] <= piv)
        l++;
      else
        swap(&arr[l], &arr[--r]);
    }
    swap(&arr[--l], &arr[beg]);
    sort(arr, beg, l);
    sort(arr, r, end);
  }
}

int main () {
  int arr[10] = { 3, 5, 1, 7, 2, 8, 3, 5, 7, 4 };

  for (const int &e : arr) cout << e << " ";
  cout << endl;

  sort(arr, 0, 10);
  for (const int &e : arr) cout << e << " ";
  cout << endl;

  return 0;
}
