/*
CUDA Wave Equation Solver
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Kernel CUDA para actualizar la ecuación de onda
__global__ void updateWave(double* un, double* un_m1, double* un_p1, int N, double delta_x, double delta_t, double c) {
    // Calcula el ID global del hilo
    int i = threadIdx.x + blockIdx.x * blockDim.x;

    if (i > 0 && i < N - 1) {
        // Coeficiente CFL para la ecuación de onda
        double cfl = c * c * delta_t * delta_t / (delta_x * delta_x);
        // Actualización usando el método de punto medio
        un_p1[i] = cfl * (un[i + 1] + un[i - 1] - 2.0 * un[i]) - un_m1[i] + 2.0 * un[i];
    }
}

// Inicializa los arreglos un, un_m1 y un_p1
void initialiseArrays(double* un, double* un_m1, double* un_p1, int N, double delta_x, double delta_t, double t, double c) {
    for (int i = 0; i < N; i++) {
        double xpos = i * delta_x;

        // Inicialización necesaria con una solución conocida (gaussiana)
        un[i] = gaussian(xpos, t, c);
        un_p1[i] = 0.0;
        un_m1[i] = gaussian(xpos, t - delta_t, c);
    }
}

int main() {
    double c, delta_t, delta_x, t_ini;

    c = 1.0;
    delta_t = 0.1;
    delta_x = 1.0;

    t_ini = 0.0;

    // Coeficiente CFL para la ecuación de onda
    double cfl = c * c * delta_t * delta_t / (delta_x * delta_x);

    int Ni = 10000;
    int Nt = 20000;
    int array_size = sizeof(double) * Ni;

    int count = 0;

    double* un;
    double* un_p1;
    double* un_m1;
    double* tmp;

    // Asignación de memoria en el dispositivo (GPU) utilizando Unified Memory
    cudaMallocManaged(&un, array_size);
    cudaMallocManaged(&un_p1, array_size);
    cudaMallocManaged(&un_m1, array_size);

    char filename[20];

    // Inicializa los arreglos
    initialiseArrays(un, un_m1, un_p1, Ni, delta_x, delta_t, t_ini, c);

    for (int t = 0; t < Nt; t++) {
        if (t % 1000 == 0) {
            sprintf(filename, "u_%05d.dat", count);
            count += 1;

            FILE* datafile = fopen(filename, "w");
            fwrite(&(un[0]), sizeof(double), Ni, datafile);
            fclose(datafile);
        }

        // Llama al kernel CUDA para actualizar la ecuación de onda
        updateWave<<<(Ni + 255) / 256, 256>>>(un, un_m1, un_p1, Ni, delta_x, delta_t, c);

        // Intercambia los arreglos para la próxima iteración
        tmp = un;
        un_m1 = un;
        un = un_p1;
        un_p1 = tmp;
    }

    // Libera la memoria asignada en el dispositivo (GPU)
    cudaFree(un);
    cudaFree(un_p1);
    cudaFree(un_m1);

    return 0;
}
