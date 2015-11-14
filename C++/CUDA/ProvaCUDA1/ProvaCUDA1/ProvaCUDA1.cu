#include <cstdlib>
#include <iostream>
#include "cuda_runtime.h"
#include <ctime>

using namespace std;

#define NUM_ELEMENTS 512 * 1000

__global__ void vecAddDevice(float * A, float * B, float * C) {
	int i = blockDim.x * blockIdx.x + threadIdx.x;
	C[i] = A[i] + B[i];
}

int main() {
	
	float * hA, * hB, * hC;
	float * dA, * dB, * dC;
	int size = NUM_ELEMENTS * sizeof(float);
	int device;
	char ch;
	cudaDeviceProp deviceProperties;
	
	hA = new float[NUM_ELEMENTS];
	hB = new float[NUM_ELEMENTS];
	hC = new float[NUM_ELEMENTS];

	// get device properties
	cudaGetDevice(&device);
	cudaGetDeviceProperties(&deviceProperties, device);
	cout << "Multiprocessors count: " << deviceProperties.multiProcessorCount << endl;
	cout << "Warp size: " << deviceProperties.warpSize << endl;
	cout << "Max Threads per Block: " << deviceProperties.maxThreadsPerBlock << endl;
	
	int numBlocks = NUM_ELEMENTS / deviceProperties.maxThreadsPerBlock;
	int threadsPerBlock = deviceProperties.maxThreadsPerBlock;

	// init vectors
	for(int i = 0; i < NUM_ELEMENTS; i++) {
		hA[i] = rand() / (float) RAND_MAX;
		hB[i] = rand() / (float) RAND_MAX;
		hC[i] = 0.0f;
	}

	cout << "Allocate device memory..." << endl;
	
	// allocate device memory
	cudaMalloc(&dA, size);
	cudaMalloc(&dB, size);
	cudaMalloc(&dC, size);
	
	// copy data to device memory
	cudaMemcpy(dA, hA, size, cudaMemcpyHostToDevice);
	cudaMemcpy(dB, hB, size, cudaMemcpyHostToDevice);
	
	cout << "Starting kernel..." << endl <<
		"Blocks: " << numBlocks << endl <<
		"Threads per block: " << threadsPerBlock << endl;


	clock_t t1 = clock();

	vecAddDevice<<<numBlocks, threadsPerBlock>>>(dA, dB, dC);
	
	cudaError_t e = cudaThreadSynchronize();

	if(e == cudaSuccess)
		cout << "Done." << endl;
	else
		cout << "Error: " << cudaGetErrorString(e) << endl;

	clock_t t2 = clock() - t1;
	
	double t = ((double)t2 / CLOCKS_PER_SEC * 1000.0);

	cout << "Time elapsed: " << t << " ms" << endl;

	cudaMemcpy(hC, dC, size, cudaMemcpyDeviceToHost);

	cout << "Freeing device memory..." << endl;
		
	// free device memory
	cudaFree(dA);
	cudaFree(dB);
	cudaFree(dC);
	

	cin >> ch;
	
	return 0;
}

