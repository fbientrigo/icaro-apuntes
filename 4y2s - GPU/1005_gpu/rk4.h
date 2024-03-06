/**
 * Realiza una integración numérica utilizando el método Runge-Kutta de cuarto orden (RK4).
 *
 * @param y     Vector de valores de la variable dependiente en el tiempo actual.
 * @param dydx  Vector de las derivadas de la variable dependiente en el tiempo actual.
 * @param x     Valor de la variable independiente en el tiempo actual.
 * @param h     Tamaño del paso de integración.
 * @param yout  Vector de salida que contendrá los nuevos valores de la variable dependiente.
 * @param derivs Función que calcula las derivadas de la variable dependiente en función de x e y.
 */


void rk4(VecDoub_I &y, VecDoub_I &dydx, const Doub x, const Doub h,
    VecDoub_O &yout, void derivs(const Doub, VecDoub_I &, VecDoub_O &))
{
    Int n = y.size();
    VecDoub dym(n), dyt(n), yt(n);
    Doub hh = h * 0.5;
    Doub h6 = h / 6.0;
    Doub xh = x + hh;

    // Calcula y(t + h/2) utilizando el valor actual de y y las derivadas en t.
    for (Int i = 0; i < n; i++)
        yt[i] = y[i] + hh * dydx[i];
    
    // Calcula las derivadas en t + h/2 utilizando y(t + h/2).
    derivs(xh, yt, dyt);

    // Calcula y(t + h) utilizando las derivadas en t + h/2.
    for (Int i = 0; i < n; i++)
        yt[i] = y[i] + hh * dyt[i];
    
    // Calcula nuevamente las derivadas en t + h/2 usando y(t + h).
    derivs(xh, yt, dym);

    // Finalmente, calcula y(t + h) usando las derivadas en t + h/2.
    for (Int i = 0; i < n; i++) {
        yt[i] = y[i] + h * dym[i];
        dym[i] += dyt[i];
    }

    // Calcula la solución final utilizando las derivadas calculadas.
    derivs(x + h, yt, dyt);

    for (Int i = 0; i < n; i++)
        yout[i] = y[i] + h6 * (dydx[i] + dyt[i] + 2.0 * dym[i]);
}

/**
Ejemplo de uso de la función rk4.
 
int main() {
    // Define las dimensiones de los vectores.
    const int N = 2;

    // Vector de valores iniciales de la variable dependiente y.
    VecDoub y(N);

    // Vector de derivadas iniciales dy/dx.
    VecDoub dydx(N);

    // Define el valor inicial de la variable independiente x.
    Doub x = 0.0;

    // Tamaño del paso de integración.
    Doub h = 0.01;

    // Vector de salida que contendrá los nuevos valores de la variable dependiente.
    VecDoub yout(N);

    // Define una función de derivadas (puede ser una función personalizada).
    void derivs(const Doub x, VecDoub_I &y, VecDoub_O &dydx) {
        dydx[0] = y[1];        // Ecuación diferencial simple.
        dydx[1] = -y[0];       // Ecuación diferencial simple.
    }

    // Asigna valores iniciales a y y dy/dx.
    y[0] = 1.0;
    y[1] = 0.0;
    dydx[0] = 0.0;
    dydx[1] = -1.0;

    // Llama a la función rk4 para realizar la integración.
    rk4(y, dydx, x, h, yout, derivs);

    // Los nuevos valores de y se almacenan en yout.
    // Puedes imprimirlos o utilizarlos según sea necesario.

    return 0;
}
*/