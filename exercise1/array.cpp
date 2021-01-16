#include <iostream>
using namespace std;

int main () {
  const int n = 100;
  int array[n];
  for (int &e : array) e = 0;

  // fill in the random values in the array

  // find the min and max values
  int max = array[0];
  int min = max;
  for (const int &num : array) {
    if (num > max) max = num;
    else if (num < min) min = num;
  }

  // output the results
  cout << "min value: " << min << endl;
  cout << "max value: " << max << endl;

  return 0;
}
