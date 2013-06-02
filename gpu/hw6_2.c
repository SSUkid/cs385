//Cory Bekker
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
//#include<cuda.h>

#define CUDA 0

#if CUDA
//this is the kernal
__global__ void kernal(double* d_A, double* d_B, double* d_C, double size){
	
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	if(index < size){
		d_C[index] = pow(d_A[index] - d_B[index],2); //take (a[i] - b[i])^2
	}
}
#endif
void ComputeDistance(double* array_a, double* array_b, double* array_c, double N);
void InitArray(double* array, double size);

int main(void){
	
	srand(time(NULL));
	double N =1000000; //size of arrays
	double *d_a, *d_b, *d_c; //device arrays
	double *h_a, *h_b, *h_c; //host arrays
	
	h_a = (double*)malloc(N*sizeof(double));//allocate host arrays
	h_b = (double*)malloc(N*sizeof(double));
	h_c = (double*)malloc(N*sizeof(double));
	
#if CUDA
	cudaMalloc((void**)&d_a, N*sizeof(double));//allocate device arrays
	cudaMalloc((void**)&d_b, N*sizeof(double));
	cudaMalloc((void**)&d_c, N*sizeof(double));
	
	//Prints the number of CUDA-enabled hardware devices
	int *count;
	cudaGetDeviceCount(count);
	printf("cudaGetDevice: %d\n", count);	
	 //prints 3 interesting properties about the device
	struct cudaDeviceProp device_p;
	cudaGetDeviceProperties(device_p);
	printf("name: %s clockrate: %d processor count: %d\n", 
	device_p.name, device_p.clockRate, device_p.multiProcessorCount);
#endif
	InitArray(h_a,N);
	InitArray(h_b,N);
	InitArray(h_c,N);
#if CUDA
	//copies the host arrays to the device arrays
	cudaMemcpy(d_a, h_a, N*sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, h_b, N*sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(d_c, h_c, N*sizeof(double), cudaMemcpyHostToDevice);
	
	/*block size depends how big the data set is going to be. It also depends
	on how many blocks the streaming processor can have and how many threads
	that streaming process can have as well. Since I don't know both of these I am 
	not sure what size the block_size should be, but I still try to use maxThreadsPerBlock(). */
	
	int threads;
	int blocks;
	//finds out how many threads the device can have per block and uses it to create
	//the number of blocks "grid_size"
	threads = device_p.maxThreadsPerBlock; //from cudaGetDeviceProperties from above
	blocks = N/threads;
	//if the dataset doesn't divide evenly then another block is added for the "extras"
    if(N % threads !=0)blocks++; 
	
	//calls cuda kernal and passes its parameters
	kernal<<<grid_size,block_size>>>(d_a, d_b, d_c, N);
	//copies the device arrays back to the host arrays
	cudaMemcpy(h_a, d_a, N*sizeof(double), cudaMemcpyDeviceToHost);
	cudaMemcpy(h_b, d_b, N*sizeof(double), cudaMemcpyDeviceToHost);
	cudaMemcpy(h_c, d_c, N*sizeof(double), cudaMemcpyDeviceToHost);
#endif
	//adds all the elements of the output array and takes the square root
	int i;
	double sum = 0;
	double answer = 0;
	for(i=0; i<N; i++){
		sum+=h_c[i];
	}
	answer = sqrt(sum);
	printf("result device: %f\n", answer);
	
	//calculates the Euclidean distnace again, but on the host instead and serial
	ComputeDistance(h_a, h_b, h_c, N);
	
	free(h_a);
	free(h_b);
	free(h_c);
#if CUDA
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);
#endif
	
	return 0;
}

void ComputeDistance(double* array_a, double* array_b, double* array_c, double N){
	int i = 0;
	double answer = 0;
	for(i = 0; i < N; i++){
		array_c[i] = pow(array_a[i] - array_b[i],2);
	}
	double sum = 0;
	for(i=0; i<N; i++){
		sum+=array_c[i];
	}
	answer = sqrt(sum);
	printf("result host: %f\n", answer);
	
}
void InitArray(double *array, double size){
	int i;
	for(i=0; i < size; i++){
		array[i] = rand() % 99;
	}
	
}
