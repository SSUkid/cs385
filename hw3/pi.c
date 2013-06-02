//Cory Bekker
//Steve kyle
/* initial comment: my stradegy for implementing the reduction phase was to let the threads
do the work in any order that they want. I didnt want to try to spread it across threads 
evenly because I didnt want each thread to wait for each other, I just wanted the thread to 
do the task as fast as it could, even if it was the case that thread 1 or 0 did most of the work.
I mean the reason that thread did most of the work in the first place was because it was 
fastest right? well then why wait for the slower thread when we could blow through it again
with the faster thread? as a result the threads distribute the work load in a random matter.
There are probably way better ways, but I don't know them as this is our first time using pthreads */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int num_threads;
static long long num_throws;
static long long throw = 0;
static long long int numberInCircle = 0;
sem_t lock;
pthread_mutex_t mutex;

void* monte_carlo(void* rank);

int main(int argc, char* argv[]){
  
  int root = 0;
  pthread_t* thread_handles;
  long thread_id = 0;

  if(argc != 3){
    printf("Usage: [num threads] [num throws]\n");
    exit(1);
  }

  num_threads = strtol(argv[1], NULL, 10); 
  num_throws = strtol(argv[2], NULL, 10);
  thread_handles = malloc(num_threads * sizeof(pthread_t*));
  printf("number of threads: %d\n", num_threads);
  printf("number of throws: %d\n", num_throws);

  for(thread_id =0; thread_id < num_threads; thread_id++){
    pthread_create(&thread_handles[thread_id],NULL,monte_carlo,(void*)thread_id);
  }
  for(thread_id =0; thread_id < num_threads; thread_id++){
    pthread_join(thread_handles[thread_id], NULL);
  }
  free(thread_handles);
  
  return 0;
}
/****FUNCTION****/
void* monte_carlo(void* rank){
 
  double x,y,distance;

  pthread_mutex_init(&mutex,NULL);
  
  printf("thread id: %d has reached critical section\n", rank);
  while(throw < num_throws){ 
    x = (rand()-RAND_MAX/2.0)/(RAND_MAX/2.0);
    y = (rand()-RAND_MAX/2.0)/(RAND_MAX/2.0);
    distance = x*x + y*y;
    pthread_mutex_lock(&mutex);//
    if(distance <= 1)
      numberInCircle++;
    printf("number in cirlce increased to %d by id %d\n", numberInCircle, rank);
    throw++;
    pthread_mutex_unlock(&mutex);//
  }
  
  if(rank == 0)
    printf("Pi Equals: %f\n", (4.0 * (double)numberInCircle)/(double)num_throws);

  return NULL;
}
