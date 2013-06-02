#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
using namespace std;

int main(){

	double time;
	int thread_count = 1;
	int i;
	int j;
	int prime;
	int total = 0;
	int n = 10000;
	
	omp_set_num_threads(thread_count);
	
	time = omp_get_wtime();
	
#pragma omp parallel for shared(n) private(i,j,prime) reduction ( + : total )
	for (i = 2; i <= n; i++)
	{ 
		prime = 1;
		for (j = 2; j < i; j++)
		{
			if (i % j == 0)
			{
			prime = 0;
			break;
			}
		}
	total = total + prime;
	}
	time = omp_get_wtime() - time;

	
	cout<<"Time: "<<time<<endl;
	cout<<"there are "<<total<<" prime numbers between 1 and "<<n<<endl;
	
  return 0;
}