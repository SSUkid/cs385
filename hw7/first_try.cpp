#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <omp.h> // We need to include this to use openMP

using namespace std;

int main(){

	srand(time(NULL));
	int start; //start time 
	int end; // end time
	int size = 10000; //size of the 2d array
	int thread_count = 4;
	int best = 0; 
	int current = 0;
  
	omp_set_num_threads(thread_count);
  //creates a 2d array with pointers so we can make them BIG
	int **arr = new int*[size];
	for(int i = 0; i < size; i++){
		arr[i] = new int[size];
	}
  
  //initialize the array
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			arr[i][j] = (rand() % 2000) - 1000;
		}
	}
	
	start = clock();
	//find the max sum
#pragma omp parallel for private(current) collapse(2)
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++){
			current = current + arr[i][j];
			best = best > current ? best : current; 
		}
	}
  
	end = clock() - start;
	cout<<"Time: "<<end<<endl;
	cout<<"Best: "<<best<<endl;
	return 0;
}


   

 
 


 

