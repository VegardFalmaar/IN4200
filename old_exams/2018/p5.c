#include <omp.h>
#include <stdio.h>

int main ()
{
  int total_sum = 0;
  int i;
  omp_set_num_threads(4);
  #pragma omp parallel default(shared) reduction(+:total_sum)
  {
    int my_id = omp_get_thread_num();
    int my_sum = 0;
    #pragma omp for schedule(static,10)
    for (i=1; i<=100; i++)
      my_sum += i;
    printf("From thread No.%d: my_sum=%d\n", my_id, my_sum);
    total_sum += my_sum;
  }
  printf("Total sum=%d\n",total_sum);

  return 0;
}
