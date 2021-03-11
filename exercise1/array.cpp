#include <iostream>
using namespace std;

void print_array (array_view<int> a)
{
  for (auto &e : a)
    cout << e << endl;
}

int main () {
  const int n = 10;
  int array[n];
  for (int &e : array) e = 0;

  print_array(array_view<int>{array});

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
