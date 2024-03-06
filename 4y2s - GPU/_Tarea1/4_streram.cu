#include <stdio.h>
#include <time.h>
#define SIZE 1024
#define BLOCK_SIZE 16
#define NUM_STREAMS 4

#define CUDA_CHECK_ERROR() __cuda_check_errors(__FILE__, __LINE__)
#define CUDA_SAFE_CALL(err) __cuda_safe_call(err, __FILE__, __LINE__)

inline void __cuda_check_errors (const char *filename, const int line_number);
inline void __cuda_safe_call (cudaError err, const char *filename, const int line_number);



__global__ void matrixAddGlobal(int *A, int *B, int *C, int size) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;


    if (row < SIZE && col < SIZE) {
        C[row * SIZE + col] = A[row * SIZE + col] + B[row * SIZE + col];
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
    A = (int *)malloc(size);
    B = (int *)malloc(size);
    C = (int *)malloc(size);

    // Inicializar las matrices A y B, de manera plana
    for (int i = 0; i < SIZE * SIZE; i++) {
        A[i] = 1;
        B[i] = 2;
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    total_time += cpu_time_used;
    printf("\t asignación A,B,C en %f ms\n", total_time * 1000);

    // ---------- device ................................
    // Reservar memoria en el device
    int *d_A, *d_B, *d_C;
    cudaMalloc((void **)&d_A, size);
    cudaMalloc((void **)&d_B, size);
    cudaMalloc((void **)&d_C, size);

    // Copiar las matrices A y B al device
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    // Definir la configuración de ejecución
    // dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
    // dim3 dimGrid((SIZE + dimBlock.x - 1) / dimBlock.x, (SIZE + dimBlock.y - 1) / dimBlock.y);
    // printf("\t ----dimGrid.x: %d, dimGrid.y: %d\n", dimGrid.x, dimGrid.y);
    // printf("\t ----dimBlock.x: %d, dimBlock.y: %d\n", dimBlock.x, dimBlock.y);

    // Crear streams
    cudaStream_t streams[NUM_STREAMS];
    for (int i = 0; i < NUM_STREAMS; i++) {
        cudaStreamCreate(&streams[i]);
    }

    int streamSize = SIZE / NUM_STREAMS;
    int streamBytes = streamSize * SIZE * sizeof(int);

    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 dimGrid((SIZE + dimBlock.x - 1) / dimBlock.x, (SIZE + dimBlock.y - 1) / dimBlock.y);
    printf("\t ----dimGrid.x: %d, dimGrid.y: %d\n", dimGrid.x, dimGrid.y);
    printf("\t ----dimBlock.x: %d, dimBlock.y: %d\n", dimBlock.x, dimBlock.y);

    start = clock();
    // Distribuir el cálculo entre los streams
    
    for (int i = 0; i < NUM_STREAMS; i++) {
        // nota para futuro:
        // !!!!!!!
        // no es necesario especificar offset, pues ya se hace con threadId
        /*
        cudaMemcpyAsync(d_A + offset* SIZE, A + offset * SIZE, streamBytes, cudaMemcpyHostToDevice, streams[i]);
        cudaMemcpyAsync(d_B + offset * SIZE, B + offset * SIZE, streamBytes, cudaMemcpyHostToDevice, streams[i]);
        matrixAddGlobal<<<dimGrid, dimBlock, 0, streams[i]>>>(d_A + offset * SIZE, d_B + offset * SIZE, d_C + offset * SIZE, SIZE);
        */

        // Copiar porciones de las matrices A y B a los streams
        cudaMemcpyAsync(d_A, A, streamBytes, cudaMemcpyHostToDevice, streams[i]);
        cudaMemcpyAsync(d_B, B, streamBytes, cudaMemcpyHostToDevice, streams[i]);

        // Lanzar el kernel en cada stream
        matrixAddGlobal<<<dimGrid, dimBlock, 0, streams[i]>>>(d_A, d_B, d_C, SIZE);
    }

    // Sincronizar los streams
    for (int i = 0; i < NUM_STREAMS; i++) {
        //cudaStreamSynchronize(streams[i]);
        cudaStreamDestroy(streams[i]);
    }



    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    total_time += cpu_time_used;
    printf("\t kernel en: %f ms\n", total_time * 1000);

    // Copiar la matriz C del device al host
    start = clock();
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    total_time += cpu_time_used;
    printf("\t copia a host en: %f ms\n", total_time * 1000);




    /* ----------- Verificación de Resultados ---------------------*/
    // Verificar que todos los elementos de C son iguales a 3
    for (int i = 0; i < SIZE * SIZE; i++) {
        if (C[i] != 3) {
            printf("C[0]=%d \n",C[0]);
            printf("Error: el elemento C[%d]=%d no es igual a 3\n", i, C[i]);
            free(A);
            free(B);
            free(C);
            cudaFree(d_A);
            cudaFree(d_B);
            cudaFree(d_C);
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
    printf("Limpiando punteros en device y host\n");
    printf("Tiempo total %f\n", total_time);
    return 0;
}

/* Encontre codigo para intentar arreglar mi problema de C[256]=0*/

// See: http://codeyarns.com/2011/03/02/how-to-do-error-checking-in-cuda/
inline void
__cuda_check_errors (const char *filename, const int line_number)
{
  cudaError err = cudaDeviceSynchronize ();
  if (err != cudaSuccess)
    {
      printf ("CUDA error %i at %s:%i: %s\n",
          err, filename, line_number, cudaGetErrorString (err));
      exit (-1);
    }
}

inline void
__cuda_safe_call (cudaError err, const char *filename, const int line_number)
{
  if (err != cudaSuccess)
    {
      printf ("CUDA error %i at %s:%i: %s\n",
          err, filename, line_number, cudaGetErrorString (err));
      exit (-1);
    }
}