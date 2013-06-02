
/* openMP vector increment starter code.
 * Your group's names go here: _________
 */
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>


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
    int rank, size; // Still valid concepts in OpenMP
    long long int i, vect_length; // User-specified vector length
    int* vector; // The only vector we need! (shared memory)
    int num_threads;
    char infilename[100], outfilename[100];

    // Get user inputs and read the input vector
    printf("Vector length:\n");
    scanf("%lld", &vect_length);
    printf("Name of input file:\n");
    scanf("%99s", infilename);
    printf("Name of output file:\n");
    scanf("%99s", outfilename);
    printf("Desired number of threads:\n");
    scanf("%d", &num_threads);
    printf("\n");
    vector = (int *) malloc(vect_length * sizeof(int));
    read_input_file(infilename, vect_length, vector);
   
    // Parallelize me!
#pragma omp for
    for (i=0; i < vect_length; i++)
        vector[i]++;


     
    write_output_file(outfilename, vect_length, vector);
    free(vector);

    return 0;
}
