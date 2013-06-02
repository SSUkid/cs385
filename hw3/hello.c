#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long thread_count;

void * hello(void* rank);

int main(int argc, char* argv[]){
  long thread_id;
  pthread_t* thread_handles;

  thread_count = strtol(argv[1],NULL,10);
  thread_handles = malloc(thread_count * sizeof(pthread_t));

  for(thread_id =0; thread_id< thread_count; thread_id++){
    pthread_create(&thread_handles[thread_id], NULL, hello,(void*) thread_id);
  }

  printf("hello from the main thread\n");

  for(thread_id = 0; thread_id < thread_count; thread_id++)
    pthread_join(thread_handles[thread_id],NULL);

  free(thread_handles);
  return 0;
}

void* hello(void* rank){
  long my_rank = (long) rank;
  printf("hi from thread %ld of %ld\n", my_rank, thread_count);
  return NULL;
}
