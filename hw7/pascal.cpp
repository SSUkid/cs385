#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

using namespace std;


int main(){
	
	srand(time(NULL));
	int start; //start time 
	int end; //end time
	int row = 20000; //starting position in the triangle
	long int c = 1;
	int num_threads = 4;

	omp_set_num_threads(num_threads);
	
	start = clock();
#pragma omp parallel for reduction(+:c)
	for(int i = 0; i < row; i++){
		for(int j = 0; j <= i; j++){
			c = c * (i - j) / (j + 1);
		}
	} 
	end = clock() - start;
	cout<<"Sum of every int in your triangle: "<<c<<endl;
	cout<<"Time: "<<end<<endl;
	
  return 0;
}