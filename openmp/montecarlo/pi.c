//Cory Bekker
//Steve kyle
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

int main(int argc, char* argv[]){
  
  int num_threads;
  long long num_throws = 0;
  long long throw = 0;
  long long int numberInCircle = 0;

  if(argc != 3){
    printf("Usage: [num threads] [num throws]\n");
    exit(1);
  }

  num_threads = strtol(argv[1], NULL, 10); 
  num_throws = strtol(argv[2], NULL, 10);
  printf("number of threads: %d\n", num_threads);
  printf("number of throws: %d\n", num_throws);
  omp_set_num_threads(num_threads);//set the number of threads


  double x,y,distance;

#pragma omp parallel for shared(numberInCircle)
  for(throw = 0; throw < num_throws; throw++){
    x = (rand()-RAND_MAX/2.0)/(RAND_MAX/2.0);
    y = (rand()-RAND_MAX/2.0)/(RAND_MAX/2.0);
    distance = x*x + y*y;
    if(distance <= 1)
      numberInCircle++;
  } 
  
  printf("Pi Equals: %f\n", (4.0 * (double)numberInCircle)/(double)num_throws);

  return 0;
}
