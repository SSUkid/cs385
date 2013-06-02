//cory bekker
//steve kyle

/*
 * pthreads program to increment every element of an array
 * Command-line args: number of threads, array size
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

int DISTRIBUTION = 1;  // 0 for block, 1 for cyclic
long int num_threads;
int* arr;
long int arr_size;
static int index_count = 0;//- holds current index position of the array
static int difference = 0; //hold the position in the array that didnt divide envenly
pthread_mutex_t mutex; 
pthread_mutex_t mutex2; // used for the part of the array that doesnt divide evenly into pthread
sem_t sem_mutex;

void InitArray(int* arr, long long length);  // Initialize array with random ints
void* Increment(void* rank); 
void* Increment_cyclic(void* rank);

int main(int argc, char* argv[]) {
  int correct = 1;
  long long i;
  int* arr_copy;
  
  /* YOUR DECLARATIONS GO HERE */
  /* -------------------------- */
  long thread_id = 0;
  pthread_t* thread_handles;
  int block_size;

  if (argc != 3) {
    printf("Usage: inc.out [num threads] [array size]\n");
    exit(1);
  }
  printf("Distribution is %d (0=block, 1=cyclic)\n", DISTRIBUTION);
  
  num_threads = strtol(argv[1], NULL, 10);
  thread_handles = malloc(num_threads * sizeof(pthread_t*));

  arr_size = strtoll(argv[2], NULL, 10);
  arr      = (int *) malloc(arr_size * sizeof (int));    
  arr_copy = (int *) malloc(arr_size * sizeof (int));
  InitArray(arr, arr_size); 
  for (i = 0; i < arr_size; i++) { //makes arr_copy
    arr_copy[i] = arr[i];
  }
  
  printf("array size: %d\n", arr_size);
  printf("number of threads: %d\n", num_threads);
  
  if(DISTRIBUTION == 0){
    for(thread_id = 0; thread_id < num_threads; thread_id++){
      pthread_create(&thread_handles[thread_id], NULL, Increment, (void*)thread_id);    
    }
  }
  /*DISTRIBUTION = 1*/
  else{
    for(thread_id = 0; thread_id < num_threads; thread_id++){
      pthread_create(&thread_handles[thread_id], NULL, Increment_cyclic, (void*)thread_id);
    }  
  }
  /*JOIN THREADS*/
  for(thread_id =0; thread_id < num_threads; thread_id++){
    pthread_join(thread_handles[thread_id], NULL);
  }
  free(thread_handles);
  
  /* START OF CHECKER CODE */
  for (i = 0; i < arr_size; i++) {
    if (arr[i] != arr_copy[i] + 1) {
      printf("Error at index %lld: supposed to have %d, but has %d\n", i, arr_copy[i]+1, arr[i]);
      correct = 0;
    }
  } 
  if (correct) printf("Correct!\n");

  return 0;
}
/****FUNCTIONS****/
void* Increment_cyclic(void* rank){
  int itr = 0;
  long int thread = 0;
 
  pthread_mutex_init(&mutex, NULL);

  while(index_count < arr_size){ 
    rank = 0;
    for(rank; rank < num_threads; rank++){
      pthread_mutex_lock(&mutex);
      //thread = arr_size % index_count; //make sure indexCount != 0 "floating point excep."
      printf("index %d changed to %d by thread id %d\n", index_count, arr[index_count], rank);
      if(index_count < arr_size){
	arr[index_count++]++;
      }
      pthread_mutex_unlock(&mutex);
    }
  }
  return NULL;
}

void* Increment(void* rank) {
  int block_size;
  int i = 0;

  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&mutex2, NULL);

  block_size = arr_size/num_threads;
  
  /*if the array doesn't divide evenly amongst threads, rank 0 will make "difference".
    Only one thread does this otherwise each thread could update
    "difference" when it is already being used... which is bad mmmm k   */
  if(rank == 0){
    difference = arr_size % num_threads;
    difference = arr_size - difference;//postions in array that doesn't divide evenly
  }

  printf("block size: %d for rank: %d\n", block_size, rank);

  for(i; i < block_size; i++){//incr. the portion THAT DID divide evenly between threads
    pthread_mutex_lock(&mutex);//locked for crtical section
    printf("index %d changed to %d by thread id %d\n", index_count, arr[index_count], rank);   
    arr[index_count++]++; 
    pthread_mutex_unlock(&mutex);//unlocked
  }
  
  //this chunk of code slows it down, but i wanted the uneven index to be distributed across threads
  if(difference != 0){ //incr. the portion of the array THAT DIDN'T divide evenly between threads
    if(difference < arr_size){
      pthread_mutex_lock(&mutex);
      printf("index %d changed to %d by thread id %d\n", difference, arr[difference], rank);
      arr[difference++]++;
      pthread_mutex_unlock(&mutex);
    }
  }
  return NULL; 
}

void InitArray(int* arr, long long length) {
  long long i;
  
  // Initialize array with random ints
  srand(time(NULL));
  for (i=0; i < length; i++){
    arr[i] = rand();
  }
}
