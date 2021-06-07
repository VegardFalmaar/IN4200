#include <iostream>
#include <omp.h>

using namespace std;

void func (int N, int *arr)
{
  int pass, smallIndex, smallVal, mySmallIndex, mySmallVal, j, temp;
  omp_set_num_threads(4);
  int *smallestValues, *smallestIndices;
  #pragma omp parallel
  {
  smallestValues = new int[omp_get_num_threads()];
  smallestIndices = new int[omp_get_num_threads()];
  for (pass = 0; pass < N-1; pass++) {
    smallIndex = pass;
    mySmallIndex = smallIndex;
    smallVal = arr[mySmallIndex];
    mySmallVal = smallVal;
    #pragma omp for firstprivate(smallIndex, mySmallVal) reduction(min: smallVal)
    for (j = pass+1; j < N; j++) {
      if (arr[j] < mySmallVal) {
        mySmallIndex = j;
        mySmallVal = arr[j];
        smallVal = mySmallVal;
      }
    }
    smallestValues[omp_get_thread_num()] = mySmallVal;
    smallestIndices[omp_get_thread_num()] = smallIndex;

    #pragma omp single
    {
    for (j=0; j<omp_get_num_threads(); j++)
      if (smallestValues[j] == smallVal)
        smallIndex = smallestIndices[j];

    temp = arr[pass];
    arr[pass] = arr[smallIndex];
    arr[smallIndex] = temp;
    }
  }
  }
  delete smallestValues;
  delete smallestIndices;
}

int main ()
{
  const int N = 20;
  int *arr = new int[N];
  for (int i=0; i<N; i++)
    arr[i] = 20-i%4;

  func(N, arr);
  for (int i=0; i<N; i++)
    cout << " " << arr[i];
  cout << endl;

  delete arr;
}
