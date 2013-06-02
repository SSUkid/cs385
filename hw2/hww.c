#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

int get_input(int rank, int size);

int main(void){

  int rank;
  int size;
  double toss; 
  double distance; 
  double number_in_circle;
  int chunk_size = 0;
  int num_of_throws = 0;
  double pi_estimate;
  double x;
  double y;


  srand(time(NULL)); 

  MPI_Init(NULL, NULL); //argc argv
  MPI_Comm_size(MPI_COMM_WORLD, &size); //gets size 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); //gets rank 

  num_of_throws = get_input(rank,size);
  chunk_size = num_of_throws/size;

  if(rank != 0){
    printf("process %i recieved %i tosses\n", rank, chunk_size);
    for(toss = 0; toss < chunk_size; toss++){
      x = (double)rand()/(double)RAND_MAX;
      y = (double)rand()/(double)RAND_MAX;
      printf("\nx: %f\n", x);
      printf("y: %f\n", y);
      distance = (x*x) + (y*y);
      printf("distance: %f\n",distance);
      printf("rank: %d\n", rank);
      printf("toss: %f\n", toss);

      if (distance <= 1){
        number_in_circle++;
        printf("number_in_circle: %f\n",number_in_circle);
      }
    }
  }

  double total_in_circle;
  MPI_Reduce(&number_in_circle, &total_in_circle, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if(rank == 0){
    printf("total in cirlce: %f\n", total_in_circle);
    pi_estimate = (4 * total_in_circle)/((double)num_of_throws);
    printf("pi_estimate: %f\n", pi_estimate);
    MPI_Barrier(MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return 0;
}
//function------------------------------------------------

int get_input(int rank, int size){

  int num_of_throws;
  int chunk_size;

  printf("current rank: %d\n", rank);

  if(rank == 0){

    printf("Enter number of throws. throws must be divisible by the processes\n");
    scanf("%d", &num_of_throws);

    chunk_size = num_of_throws/(size);

    printf("size: %d\n", size);
    printf("num of throws: %d\n", num_of_throws);
    printf("chunk size: %d\n", chunk_size);

    MPI_Bcast(&chunk_size, 1, MPI_INT, 0, MPI_COMM_WORLD); 

  }
  return num_of_throws;
} 
   
 

