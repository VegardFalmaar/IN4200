#include <iostream>
#include <omp.h>

using namespace std;

void func (int N, int *arr)
{
  int pass, smallIndex, smallVal, mySmallIndex, mySmallVal, j, temp;
  omp_set_num_threads(4);
  #pragma omp parallel
  {
  for (pass = 0; pass < N-1; pass++) {
    smallIndex = pass;
    smallVal = arr[smallIndex];
    mySmallIndex = smallIndex;
    mySmallVal = smallVal;
    #pragma omp for firstprivate(mySmallIndex, mySmallVal) nowait
    for (j = pass+1; j < N; j++) {
      if (arr[j] < mySmallVal) {
        mySmallVal = arr[j];
        mySmallIndex = j;
      }
    }
    #pragma omp critical
    {
    if (mySmallVal < smallVal) {
      smallVal = mySmallVal;
      smallIndex = mySmallIndex;
    }
    }
    #pragma omp barrier
    #pragma omp single
    {
    temp = arr[pass];
    arr[pass] = arr[smallIndex];
    arr[smallIndex] = temp;
    }
    #pragma omp barrier
  }
  }
}

int main ()
{
  const int N = 30;
  int *arr = new int[N];
  for (int i=0; i<N; i++)
    arr[i] = 20-i%5 + i/2;
  for (int i=0; i<N; i++)
    cout << " " << arr[i];
  cout << endl;

  func(N, arr);
  for (int i=0; i<N; i++)
    cout << " " << arr[i];
  cout << endl;

  delete arr;
}
