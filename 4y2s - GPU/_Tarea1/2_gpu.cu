#include <stdio.h>
#define SIZE 1024
#define BLOCK_SIZE 16


// global, es llamada por el host, pero ejecutada en gpu
// estas deben ser tipo void
// variaciones incluyen
/*
    __device__; es llamada en device, solo puede ser llamada dentro de un host u device
    __host__; el prefijo por default, llamada y ejecutada en el CPU (host)
*/
__global__ void matrixAdd(int *A, int *B, int *C) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if(row < SIZE && col < SIZE) {
        C[row * SIZE + col] = A[row * SIZE + col] + B[row * SIZE + col];
    }
}

int main() {
    
    // size es una cantidad de bytes
    // SIZE es el tamaño de las matrices
    int size = SIZE * SIZE * sizeof(int);
    
    
    // Reservar memoria en el host
    int *A, *B, *C;
    A = (int*)malloc(size);
    B = (int*)malloc(size);
    C = (int*)malloc(size);

    // Inicializar las matrices A y B, de manera plana
    for(int i = 0; i < SIZE * SIZE; i++) {
        A[i] = 1;
        B[i] = 2;
    }

    // Reservar memoria en el device
    int *d_A, *d_B, *d_C;
    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    // Copiar las matrices A y B al device
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    // Definir la configuración de ejecución
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 dimGrid((SIZE + dimBlock.x - 1) / dimBlock.x, (SIZE + dimBlock.y - 1) / dimBlock.y);

    // Lanzar el kernel
    // el kernel se lanza con 
    // kernelfuncion<<<dimGrid,dimBlock>>>(parametros**);
    matrixAdd<<<dimGrid, dimBlock>>>(d_A, d_B, d_C);

    // Copiar la matriz C del device al host
    // esto implica que movamos lo realizado en GPU (device) devuelta al sistema principal (host)
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

    /* ----------- Verificacion de Resultados ---------------------*/
    // Verificar que todos los elementos de C son iguales a 3
    for(int i = 0; i < SIZE * SIZE; i++) {
        if(C[i] != 3) {
            printf("Error: el elemento C[%d] no es igual a 3\n", i);
            return 1;
        }
    }

    printf("Todos los elementos de la matriz C son iguales a 3\n");

    // Liberar memoria
    free(A);
    free(B);
    free(C);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}
