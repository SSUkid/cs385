/* initial comment:

   1)What happens if you specify a number of threads less than the default?
   - nothing unusual happend. The program ran with the number of threads.

   2)What happens if you specify a number of threads less than the default?
   - Once again nothing unusual happened. The program ran with the 32 threads
     just fine.

   3)What happens if you specify a number of threads less than the default?
   -  When I tried to run with 2000 threads I recieved an error which is..
     "libgomp: Thread creation failed: Resource temporarily unavailable"
     From the given error it looks like there are not enough resources
     to run 2000 threads.
*/


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void Hello(void); 

int main(int argc, char* argv[]) {  
  
  int thread_count = strtol(argv[1], NULL, 10);
  
#pragma omp parallel num_threads(thread_count)
  Hello();
  
  return 0;
} 
//----------------------------------------------------------
void Hello(void) {  
  
  int my_rank = omp_get_thread_num();
  int thread_count = omp_get_num_threads();
  
  printf("Hello from thread %d of %d\n", my_rank, thread_count);
  
} 
