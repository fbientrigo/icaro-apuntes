/*
Codigo con el objetivo de realizar una simulacion de una onda con 3 dimensiones
*/

//#include "nr3.h" 
//#include "rk4.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

/*

*/


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
    // Define las dimensiones de los vectores.
    const int N = 2;

    // Vector de valores iniciales de la variable dependiente y.
    VecDoub y(N);

    // Vector de derivadas iniciales dy/dx.
    VecDoub dydx(N);
    VecDoub dy2dx2(N);
    Doub x = 0.0; //var independiente, como input a funciones

    Doub h = 0.01; // time-step
    VecDoub yout(N); //salida


    // Asigna valores iniciales a y y dy/dx.
    y[0] = 1.0;
    y[1] = 0.0;
    dydx[0] = 0.0;
    dydx[1] = -1.0;
    dy2dx2[0] = 0.0;
    dy2dx2[1] = -1.0;

    // Llama a la función rk4 para realizar la integración.
    rk4(y, dydx, x, h, yout, derivs);
    rk4(y, dydx, x, h, yout, derivs);

    // Los nuevos valores de y se almacenan en yout.
    printf("t \t|\ty0 \t|\ty1 \t|\n");
    // Define el número de pasos de integración.
    int numSteps = 10;

    // Realiza un bucle para imprimir los valores en forma de tabla.
    for (int i = 0; i <= numSteps; ++i) {
        printf("%.4f \t|\t%.4f \t|\t%.4f \t|\n", x, yout[0], yout[1]);

        // Actualiza los valores de x, y y dy/dx para el siguiente paso.
        x += h;
        y[0] = yout[0];
        y[1] = yout[1];

    }
    return 0;
}