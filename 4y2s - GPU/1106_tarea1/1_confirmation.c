#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 1024

// suma de matrices lineal
void linearMatrixSum(int **A, int **B, int **C);
// chequea el funcionamiento correcto de la suma
int checkCfor3(int **C);

int main() {
    int **A, **B, **C;
    int i, j;

    // Reservar memoria para las matrices A, B y C
    A = (int **)malloc(SIZE * sizeof(int *));
    B = (int **)malloc(SIZE * sizeof(int *));
    C = (int **)malloc(SIZE * sizeof(int *));
    for(i = 0; i < SIZE; i++) {
        A[i] = (int *)malloc(SIZE * sizeof(int));
        B[i] = (int *)malloc(SIZE * sizeof(int));
        C[i] = (int *)malloc(SIZE * sizeof(int));
    }

    // Inicializar las matrices A y B
    for(i = 0; i < SIZE; i++) {
        for(j = 0; j < SIZE; j++) {
            A[i][j] = 1;
            B[i][j] = 2;
        }
    }

    // seccion de medicion de tiempo
    clock_t start, end;
    double cpu_time_used, total_time = 0;

    // Calcular la suma de las matrices 3 veces y tomar el tiempo promedio
    for(i = 0; i < 3; i++) {
        start = clock();
        linearMatrixSum(A, B, C);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        total_time += cpu_time_used;
    }

    printf("Se han repetido `linearMatrixSum(A, B, C)` 3 veces, con un \n");
    printf("\ttiempo promedio para calcular la suma de las matrices: %f ms\n", (total_time / 3) * 1000);
    

    // Verificar que todos los elementos de C son iguales a 3
    printf("Testing correct code working...\n");
    int correct_code = checkCfor3(C);
    if (correct_code==1) {
        printf("El codigo ha funcionado de manera correcta\n");
        return 0;
    } else {
        printf("El codigo NO ha funcionado de manera esperada\n");
        return 1;
    }
    
    // Liberar la memoria reservada
    for(i = 0; i < SIZE; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
}

void linearMatrixSum(int **A, int **B, int **C){
    int i, j;
    // Calcular la suma de las matrices
    for(i = 0; i < SIZE; i++) {
        for(j = 0; j < SIZE; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

int checkCfor3(int **C){
    int i, j;
    for(i = 0; i < SIZE; i++) {
        for(j = 0; j < SIZE; j++) {
            if(C[i][j] != 3) {
                printf("\tError: el elemento C[%d][%d] no es igual a 3\n", i, j);
                return 0;
            }
        }
    }
    printf("\tTodos los elementos de la matriz C son iguales a 3\n");
    return 1;
}
