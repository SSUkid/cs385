//Cory Bekker

#include<stdlib.h>
#include<stdio.h>
//#include<cuda.h>

#define CUDA 0

#if CUDA
__global__ void add_vec(double* device_arr_a, double* device_arr_b, double elements){

	int index = blockIdx.x * blockDim.x + threadIdx.x;

	if(index < elements)
		device_arr_a[index] += device_arr_b[index];
	
	if(index + 1 < elements)
		device_arr_a[index+1] += device_arr_b[index+1];
	
}
#endif
int main(void){
	
	double *device_array_a, *device_array_b;
	double *host_array_a, *host_array_b;
	double num_elements = 50000;
	
	host_array_a = (double*)malloc(num_elements*sizeof(double));//allocate host arrays
	host_array_b = (double*)malloc(num_elements*sizeof(double));
#if CUDA
	cudaMalloc((void**)&device_array_a, num_elements*sizeof(double));//allocate device arrays
	cudaMalloc((void**)&device_array_b, num_elements*sizeof(double));
#endif	
	int i = 0;
	for(i = 0; i < num_elements; i++){ //initialize arrays
		host_array_a[i] = i;
		host_array_b[i] = i;
	}
#if CUDA	
	//copy the host(s) array to the device(s) array 
	cudaMemcpy(device_array_a, host_array_a, num_elements*sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(device_array_b, host_array_b, num_elements*sizeof(double), cudaMemcpyHostToDevice);
#endif	
	double block_size = 1024;
	double grid_size = num_elements/block_size;
	
#if CUDA
	//these are the parameters for the kernal "add_vec" which is the code that the gpu runs
	add_vec<<<grid_size,block_size>>>(device_array_a,device_array_b,num_elements);
	cudaMemcpy(host_array_a, device_array_a, num_elements*sizeof(double), cudaMemcpyDeviceToHost);
#endif
	//print results
	for(i = 0; i < num_elements; i++){
		printf("index %d has: %d \n",i,host_array_a[i]);
	}
	
return 0;	
}


/*A) When I mapped two elements to each thread I decided to give the current thread
	index and index+1. By doing this each thread can get two indexes. If the indexes
	don't distribute evenly there is a another if statement which will make sure it will
	only work on the indexes in the acceptable range. The odd indexes will still be 
	distributed, but the thread with the odd index will only work on that index because
	the index+1 will be out of range. 
 B) Since there are 50,000 elements and the block size is 1024, I believe there will be 49 blocks.
	If the block size is 1024 then there can be 1024 threads with each thread working
	on its own part of the block. If it is applied to problem A, then each thread
	can have two indexes which cuts the threads down to 512 threads. 
 C) 
	
	
*/
/*
 file add_vector.cu
Compile it with nvcc: nvcc -o add_vector add_vector.cu
Run it: ./add_vector

If you don't have a Cuda capable GPU, compile it in emulation mode:
nvcc -deviceemu -o add_vector_emu add_vector.cu
Run it: ./add_vector_emu
*/