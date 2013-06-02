#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
using namespace std;

int main(void){
	int num_of_throws = 1000000;
	int num_of_threads = 4;
	int toss; 
	double distance = 0; 
	double number_in_circle;
	int start,end;
	double pi_estimate;
	double x;
	double y;
	srand(time(NULL));
	
	omp_set_num_threads(num_of_threads);

	start = clock();
#pragma omp parallel for private(x,y,distance)
   for(toss = 0; toss < num_of_throws; toss++){
      x = (double)rand()/(double)RAND_MAX;
      y = (double)rand()/(double)RAND_MAX;
      distance += (x*x) + (y*y);
	}
	
	end = clock() - start;
	cout<<"Time: "<<end<<endl;
	cout<<"number in circle: "<<number_in_circle<<endl;
   pi_estimate = (4 * number_in_circle)/((double)num_of_throws);
	cout<<"pi estimate: "<<pi_estimate<<endl;
   
  return 0;
}

 

