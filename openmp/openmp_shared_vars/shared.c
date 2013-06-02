//cory bekker
//steve kyles

/*notes: when the variables are declared outsie of the 
  pragma the program runs much slower because every 
  thread uses each variable*/

#include <omp.h>
#include <stdio.h>
#define NUM_THREADS 4

int main(){

  int i,k,j;
  omp_set_num_threads(NUM_THREADS);

#pragma omp parallel for //private(k,j)
  for(i=0; i<20000; i++){
    for(j=0; j<20000;j++)
      for(k=0;k<20000;k++);
    printf("thread id: %i (%i, %i, %i)\n", omp_get_thread_num(),i,j,k);
  }

  return 0;
}
