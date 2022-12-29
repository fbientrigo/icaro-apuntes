#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14159265


int main(){
    int n,nmax;
  double e,factorial;
    double e2;

    
    nmax=1000;
    e=1.0;
    factorial=1.0;
    
    for(n=1;n<nmax;n++){
        
        factorial*=n;
        
        e+=1.0/factorial;
        
        e2=pow(1.0+1.0/n,n);
        
        printf("%d %.8g %.8g\n",n,e,e2);

    }
    
    
    

}
