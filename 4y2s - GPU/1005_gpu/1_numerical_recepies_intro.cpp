
#include "nr3.h" // siempre agregado
#include "rk4.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

// función
Doub f(const Doub x) {
    return x*x;
}

// function objects o functors
// objeto con un operator () que ha sido sobrecargado
struct Square {
    Doub operator()(const Doub x) {
        return x*x;
    }
};



// Ahora una aplicación más compleja de ellos:
struct Contimespow {
    Doub c,p;
    // esta linea es el constructor, la sintaxis se me hace distinta
    Contimespow(const Doub cc, const Doub pp): c(cc), p(pp) {} 

    // el operator sobrecargado
    Doub operator()(const Doub x) {
        return c * pow(x,p);
    }
};

int main() {
    // para usarlo se declara una instancia
    Square f2;

    /* acabamos de programar una manera de tener una función que cacule rapidamente
    c * x^p
    */

    Contimespow h(0.4, 2); // osea h(x) = 0.4 * x^2

    Doub x = 1;

    printf("%f \n", x);
    printf("%f \n", h(x));
    return 0;
}
