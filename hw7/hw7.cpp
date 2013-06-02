
/*This program takes a 2d array and finds the largest consecutive sum
of numbers in the array. Adding all the numbers of the array will not
give the largest consecutive sum because this array has negative and
positive numbers */


#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

using namespace std;

int main(){

	srand(time(NULL));
	int num_threads = 4;
	int start;
	int end;
	int best = 0; 
	int current = 0;
	int size = 2000;
	int arr[size][size];
	cout<<"a"<<endl;

	omp_set_num_threads(num_threads);	
	start = clock();

//#pragma omp parallel for 
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			arr[i][j] = (rand() % 2000) - 1000;
		}
	}	
//#pragma omp parallel for private(best, current)
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++){
			current = current + arr[i][j];			
			best = best > current ? best : current;
		}
	}

	end = clock() - start;
	cout<<"Time: "<<end<<endl;
	cout<<"best: "<<best<<endl;
	return 0;
}
