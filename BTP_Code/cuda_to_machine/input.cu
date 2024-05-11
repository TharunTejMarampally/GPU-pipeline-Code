#include <iostream>
#include <cmath>

// Define the neural network architecture
#define INPUT_SIZE 784
#define HIDDEN_SIZE 128
#define OUTPUT_SIZE 10

// Sigmoid activation function
__device__ float sigmoid(float x) {
    return 1.0f / (1.0f + expf(-x));
}

// Kernel function for neural network inference
__global__ void neuralNetworkInference(float *input, float *weightsInputHidden, float *weightsHiddenOutput, float *output) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    // Compute the hidden layer activations
    float hidden[HIDDEN_SIZE] = {0};
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        float sum = 0;
        for (int j = 0; j < INPUT_SIZE; j++) {
            sum += input[j] * weightsInputHidden[j * HIDDEN_SIZE + i];
        }
        hidden[i] = sigmoid(sum);
    }

    // Compute the output layer activations
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        float sum = 0;
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            sum += hidden[j] * weightsHiddenOutput[j * OUTPUT_SIZE + i];
        }
        output[i] = sigmoid(sum);
    }
}

int main() {
    // Input data (example)
    float input[INPUT_SIZE];
    for (int i = 0; i < INPUT_SIZE; i++) {
        input[i] = static_cast<float>(rand()) / RAND_MAX;
    }

    // Model weights (example)
    float weightsInputHidden[INPUT_SIZE * HIDDEN_SIZE];
    float weightsHiddenOutput[HIDDEN_SIZE * OUTPUT_SIZE];
    for (int i = 0; i < INPUT_SIZE * HIDDEN_SIZE; i++) {
        weightsInputHidden[i] = static_cast<float>(rand()) / RAND_MAX;
    }
    for (int i = 0; i < HIDDEN_SIZE * OUTPUT_SIZE; i++) {
        weightsHiddenOutput[i] = static_cast<float>(rand()) / RAND_MAX;
    }

    // Allocate device memory
    float *d_input, *d_weightsInputHidden, *d_weightsHiddenOutput, *d_output;
    cudaMalloc((void **)&d_input, INPUT_SIZE * sizeof(float));
    cudaMalloc((void **)&d_weightsInputHidden, INPUT_SIZE * HIDDEN_SIZE * sizeof(float));
    cudaMalloc((void **)&d_weightsHiddenOutput, HIDDEN_SIZE * OUTPUT_SIZE * sizeof(float));
    cudaMalloc((void **)&d_output, OUTPUT_SIZE * sizeof(float));

    // Copy data from host to device
    cudaMemcpy(d_input, input, INPUT_SIZE * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_weightsInputHidden, weightsInputHidden, INPUT_SIZE * HIDDEN_SIZE * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_weightsHiddenOutput, weightsHiddenOutput, HIDDEN_SIZE * OUTPUT_SIZE * sizeof(float), cudaMemcpyHostToDevice);

    // Launch neural network inference kernel
    int blockSize = 256;
    int numBlocks = (OUTPUT_SIZE + blockSize - 1) / blockSize;
    neuralNetworkInference<<<numBlocks, blockSize>>>(d_input, d_weightsInputHidden, d_weightsHiddenOutput, d_output);

    // Copy result from device to host
    float output[OUTPUT_SIZE];
    cudaMemcpy(output, d_output, OUTPUT_SIZE * sizeof(float), cudaMemcpyDeviceToHost);

    // Output the inference result
    std::cout << "Inference result: ";
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        std::cout << output[i] << " ";
    }
    std::cout << std::endl;

    // Free device memory
    cudaFree(d_input);
    cudaFree(d_weightsInputHidden);
    cudaFree(d_weightsHiddenOutput);
    cudaFree(d_output);

    return 0;
}


// **************************************************************
// #include <iostream>
// #include <queue>

// #define N 100 // Maximum number of nodes in the graph

// __global__ void bfs(int *adjacencyList, int *queue, bool *visited, int *level, int *queueSize, int *outputQueue, int *outputQueueSize) {
//     int tid = blockIdx.x * blockDim.x + threadIdx.x;

//     if (tid == 0) {
//         *queueSize = 0;
//         *outputQueueSize = 0;
//     }

//     __syncthreads();

//     if (tid == 0) {
//         queue[*queueSize] = 0; // Start BFS from node 0
//         atomicAdd(queueSize, 1);
//         visited[0] = true;
//         level[0] = 0;
//     }

//     __syncthreads();

//     while (*queueSize > 0) {
//         int vertex;
//         if (tid < *queueSize) {
//             vertex = queue[tid];
//         }

//         __syncthreads();

//         if (tid < *queueSize) {
//             atomicSub(queueSize, 1);

//             // Add the vertex to the output queue
//             int pos = atomicAdd(outputQueueSize, 1);
//             outputQueue[pos] = vertex;

//             // Visit neighbors of the vertex
//             for (int i = adjacencyList[vertex]; i < adjacencyList[vertex + 1]; i++) {
//                 int neighbor = adjacencyList[i];
//                 if (!visited[neighbor]) {
//                     visited[neighbor] = true;
//                     level[neighbor] = level[vertex] + 1;
//                     int queuePos = atomicAdd(queueSize, 1);
//                     queue[queuePos] = neighbor;
//                 }
//             }
//         }

//         __syncthreads();
//     }
// }

// int main() {
//     int adjacencyList[N + 1] = {0, 1, 2, 3, 4, 5}; // Example adjacency list
//     int queue[N];
//     bool visited[N] = {false};
//     int level[N];
//     int queueSize, outputQueueSize;
//     int outputQueue[N];

//     int *d_adjacencyList, *d_queue, *d_level, *d_outputQueue, *d_queueSize, *d_outputQueueSize;
//     bool *d_visited;

//     // Allocate device memory
//     cudaMalloc((void **)&d_adjacencyList, (N + 1) * sizeof(int));
//     cudaMalloc((void **)&d_queue, N * sizeof(int));
//     cudaMalloc((void **)&d_visited, N * sizeof(bool));
//     cudaMalloc((void **)&d_level, N * sizeof(int));
//     cudaMalloc((void **)&d_queueSize, sizeof(int));
//     cudaMalloc((void **)&d_outputQueue, N * sizeof(int));
//     cudaMalloc((void **)&d_outputQueueSize, sizeof(int));

//     // Copy data from host to device
//     cudaMemcpy(d_adjacencyList, adjacencyList, (N + 1) * sizeof(int), cudaMemcpyHostToDevice);
//     cudaMemcpy(d_visited, visited, N * sizeof(bool), cudaMemcpyHostToDevice);

//     // Launch BFS kernel
//     bfs<<<1, N>>>(d_adjacencyList, d_queue, d_visited, d_level, d_queueSize, d_outputQueue, d_outputQueueSize);

//     // Copy result from device to host
//     cudaMemcpy(outputQueue, d_outputQueue, N * sizeof(int), cudaMemcpyDeviceToHost);
//     cudaMemcpy(&outputQueueSize, d_outputQueueSize, sizeof(int), cudaMemcpyDeviceToHost);

//     // Output the BFS traversal
//     std::cout << "BFS Traversal: ";
//     for (int i = 0; i < outputQueueSize; i++) {
//         std::cout << outputQueue[i] << " ";
//     }
//     std::cout << std::endl;

//     // Free device memory
//     cudaFree(d_adjacencyList);
//     cudaFree(d_queue);
//     cudaFree(d_visited);
//     cudaFree(d_level);
//     cudaFree(d_queueSize);
//     cudaFree(d_outputQueue);
//     cudaFree(d_outputQueueSize);

//     return 0;
// }


// *************************************************
// __global__ void addKernelPTXv4(float4  *c, const float4 *a, const float4 *b)
// {
//     int i = threadIdx.x;
//     c[i].x = a[i].x + b[i].x;
//     c[i].y = a[i].y + b[i].y;
//     c[i].z = a[i].z + b[i].z;
//     c[i].w = a[i].w + b[i].w;
// }











// ************************************
// #include <iostream>
// #include <stdio.h>

// // Matrix dimensions
// #define N 32
// #define BLOCK_SIZE 16

// // Kernel function to perform matrix multiplication
// __global__ void matrixMul(int *a, int *b, int *c) {
//     // Calculate the row and column indices of the element
//     int row = blockIdx.y * blockDim.y + threadIdx.y;
//     int col = blockIdx.x * blockDim.x + threadIdx.x;

//     // Perform the matrix multiplication for the element in c[row][col]
//     int sum = 0;
//     for (int i = 0; i < N; i++) {
//         sum += a[row * N + i] * b[i * N + col];
//     }

//     // Store the result in c[row][col]
//     c[row * N + col] = sum;
// }

// int main() {
//     // Initialize matrices
//     int *a, *b, *c;
//     int size = N * N * sizeof(int);

//     // Allocate memory on the host
//     a = (int *)malloc(size);
//     b = (int *)malloc(size);
//     c = (int *)malloc(size);

//     // Initialize matrices with random values
//     for (int i = 0; i < N * N; i++) {
//         a[i] = rand() % 10;
//         b[i] = rand() % 10;
//     }

//     // Allocate memory on the device
//     int *d_a, *d_b, *d_c;
//     cudaMalloc((void **)&d_a, size);
//     cudaMalloc((void **)&d_b, size);
//     cudaMalloc((void **)&d_c, size);

//     //
