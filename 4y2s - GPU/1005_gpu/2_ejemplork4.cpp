
#include "nr3.h" // siempre agregado
#include "rk4.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>


// Define una función de derivadas (puede ser una función personalizada).
// toma un valor x, y las referencias de "y" y "dydx"
void derivs(const Doub x, VecDoub_I &y, VecDoub_O &dydx) {
    /*
    y0' = y1
    y1' = -y0
    */
    dydx[0] = y[1] + sin(y[1]) ;    
    dydx[1] = -y[0];    
}

int main() {
    // Define las dimensiones de los vectores.
    const int N = 2;

    // Vector de valores iniciales de la variable dependiente y.
    VecDoub y(N);

    // Vector de derivadas iniciales dy/dx.
    VecDoub dydx(N);
    Doub x = 0.0; //var independiente, como input a funciones

    Doub h = 0.01; // time-step
    VecDoub yout(N); //salida


    // Asigna valores iniciales a y y dy/dx.
    y[0] = 1.0;
    y[1] = 0.0;
    dydx[0] = 0.0;
    dydx[1] = -1.0;

    // Llama a la función rk4 para realizar la integración.
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
        derivs(x, y, dydx);
        rk4(y, dydx, x, h, yout, derivs);
    }
    return 0;
}