#include <stdio.h>
#include <time.h>
#define SIZE 1024
#define BLOCK_SIZE 16


__global__ void matrixAddShared(int *A, int *B, int *C) {
    /*se reserva memoria compartida dentro del kernel*/
    extern __shared__ int shared_memory[];

    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    // Copiar datos de la matriz A a la memoria compartida
    shared_memory[threadIdx.y * blockDim.x + threadIdx.x] = A[row * SIZE + col];
    /*cada lugar en la memoria compartida contiene la información de A*/

    // todos tienen acceso a la memoria compartida
    // por tanto B, deberia de encontrarse más "contiguo" a esta memoria que previamente a A

    __syncthreads();

    // Realizar la suma utilizando la memoria compartida
    // cada grid y block de B contiene memoria compartida
    if(row < SIZE && col < SIZE) {
        C[row * SIZE + col] = shared_memory[threadIdx.y * blockDim.x + threadIdx.x] + B[row * SIZE + col];
    }
}


int main() {
    clock_t start, end;
    double cpu_time_used, total_time = 0;

    printf("Todos los elementos de la matriz C son iguales a 3\n");


    // size es una cantidad de bytes
    // SIZE es el tamaño de las matrices
    int size = SIZE * SIZE * sizeof(int);
    


    start = clock();
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
    end = clock(); cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; total_time += cpu_time_used;
    printf("\t asignación A,B,C en %f ms\n", total_time * 1000);


    // ---------- device ................................
    // reservar memoria compartida
    start = clock();
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
    printf("\t ----dimGrid.x: %d, dimGrid.y: %d\n", dimGrid.x, dimGrid.y);
    printf("\t ----dimBlock.x: %d, dimBlock.y: %d\n", dimBlock.x, dimBlock.y);

    end = clock(); cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; total_time += cpu_time_used;
    printf("\t asignación en Device en: %f ms\n", total_time * 1000);

    // Lanzar el kernel
    start = clock();
    // el kernel se lanza con 
    // kernelfuncion<<<dimGrid,dimBlock, sharedMem>>>(parametros**);


    // Tamaño de la memoria compartida por bloque
    int shared_memory_size = BLOCK_SIZE * BLOCK_SIZE * sizeof(int);
    matrixAddShared<<<dimGrid, dimBlock, shared_memory_size>>>(d_A, d_B, d_C);
    
    end = clock(); cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; total_time += cpu_time_used;
    printf("\t kernel en: %f ms\n", total_time * 1000);
    //matrixAdd<<<dimGrid, dimBlock>>>(d_A, d_B, d_C);

    // Copiar la matriz C del device al host
    start = clock();
    // esto implica que movamos lo realizado en GPU (device) devuelta al sistema principal (host)
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
    end = clock(); cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; total_time += cpu_time_used;
    printf("\t copia a host en: %f ms\n", total_time * 1000);

    /* ----------- Verificacion de Resultados ---------------------*/
    // Verificar que todos los elementos de C son iguales a 3
    for(int i = 0; i < SIZE * SIZE; i++) {
        if(C[i] != 3) {
            printf("Error: el elemento C[%d] no es igual a 3\n", i);
            free(A);        free(B);         free(C);
            cudaFree(d_A);  cudaFree(d_B);   cudaFree(d_C);
            return 1;
        }
    }

    printf("Todos los elementos de la matriz C son iguales a 3\n");

    // Liberar memoria
    free(A);        free(B);         free(C);
    cudaFree(d_A);  cudaFree(d_B);   cudaFree(d_C);
    printf("Limpiando punteros en device y host\n");
    return 0;
}
