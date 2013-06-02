/* MPI vector increment starter code.
 * Your group's names go here: Cory Bekker
 */
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

void read_input_file(char* infilename, long long int length, int* vector) {
   // Read from file into input vector
   long long int i;
   FILE* infile = fopen(infilename, "r");
   for (i=0; i < length; i++)
       fscanf(infile, "%d", &(vector[i]));
   fclose(infile);
}

void write_output_file(char* outfilename, long long int length, int* vector) {
   // Write vector to output file
   long long int i;
   FILE* outfile = fopen(outfilename, "w");
   for (i=0; i < length; i++)
       fprintf(outfile, "%d\n", vector[i]);
   fclose(outfile);
}

int main(void) {
    int rank, size; // MPI parameters
    long long int vect_length; // User-specified vector length
    long long int i;
    int* vector; // The master vector --  not yet allocated
    int* chunk_vector; // The chunk vector -- not yet allocated
    char infilename[100], outfilename[100];

    MPI_Init (NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size); //gets size
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //gets rank
    
    // Get user input: total length of vector, filenames
    // and read vector

    if (rank == 0) {
        printf("Vector length:\n");
        scanf("%lld", &vect_length);
        printf("Name of input file:\n");
        scanf("%99s", infilename);
        printf("Name of output file:\n");
        scanf("%99s", outfilename);
        printf("\n");
        vector = (int *) malloc(vect_length * sizeof(int*)+1);
        read_input_file(infilename, vect_length, vector);
    }

    int chunk;
    chunk = vect_length / size;
    chunk_vector = (int *) malloc(chunk * sizeof(int*)+1);
 
    if(rank == 0){
      MPI_Scatter(vector, chunk, MPI_INT, chunk_vector, chunk, MPI_INT, 0, MPI_COMM_WORLD);
    }
    
      printf("rank %d of %d recieved chunk size of %d\n", rank, size, chunk);    
      int k;
      for(k = 0; k < chunk; k++){
	chunk_vector[k]++;
      }
    
    if(rank == 0){
      MPI_Gather(vector, chunk, MPI_INT, vector, vect_length, MPI_INT, 0, MPI_COMM_WORLD);//this isnt working
      write_output_file(outfilename, vect_length, vector);
    }
    
    // free(vector);
    MPI_Finalize();

    return 0;
}
