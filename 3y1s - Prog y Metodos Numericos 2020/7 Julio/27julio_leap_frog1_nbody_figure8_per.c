# include <stdio.h>
# include <math.h>

int main(){

  int ns, k, n=3,i;
  double r[n][3], v[n][3], a[n][3], rij[3],r3, dist;
  double dt = 0.01;
  double r2;
  double tprint, tiempo, t_end = 100.0;
    double Ekin, Epot, E_in,E_out;
  const double m = 1;
    double Mtot, vcom[3];
  FILE *out, *out2, *out3, *out4;

    // poner condiciones iniciales en la posición de las n=3 particulas
    
    r[0][0] = 0.9700436;
    r[0][1] = -0.24308753;
    r[0][2] = 0.;
    v[0][0] = 0.466203685;
    v[0][1] = 0.43236573;
    v[0][2] = 0.;

    r[1][0] = -r[0][0];
    r[1][1] = -r[0][1];
    r[1][2] = -r[0][2];
    v[1][0] =  v[0][0];
    v[1][1] =  v[0][1];
    v[1][2] =  v[0][2];

    r[2][0] = 0;
    r[2][1] = 0;
    r[2][2] = 0;
    v[2][0] = -2 * v[0][0];
    v[2][1] = -2 * v[0][1] ;
    v[2][2] = -2 * v[0][2] ;

    //aplicamos perturbación:
    v[0][0] += 0.01;
    
    //nuevo centro de velocidad:
    for( int k = 0; k < 3; k++)
        vcom [k]= 0.0;
    
    for( int k = 0; k < 3; k++){
        Mtot = 0;
        for( i = 0; i < n ; i++){
            vcom[k]+= m * v[i][k];
            Mtot += m;
        }
        vcom[k] /= Mtot;
        for( i = 0; i < n ; i++){
            v[i][k] -= vcom[k];
        }
    }
    
//iniciar todas las aceleraciones en 0 (n particulas, 3 dimensiones), despues iremos sumandole aceleraciones parciales por la interacción con las otras n-1 partículas
    
    for( i = 0; i < n ; i++)
        for( int k = 0; k < 3; k++)
            a[i][k] = 0.0;
    
    //ahora calculamos las distancias entre cada par de partículas rij[3], y sumamos las aceleraciones debidas a cada una
    
    for( i = 0; i < n ; i++){
        for( int j = i+1; j < n ; j++){
        
            double rij[3];
            for( int k = 0; k < 3; k++)
                rij[k] = r[j][k] - r[i][k];  //vector que va de particula i a particula j
            
            r2=0.0;
             for( int k = 0; k < 3; k++)
                 r2 += rij[k]*rij[k];       //modulo cuadrado del vector entre i y j
            double r3=r2*sqrt(r2);
            
            for (int k = 0; k <3; k++){
                a[i][k] += m * rij[k] / r3;  //vamos sumando las aceleraciones que siente i debido a cada j, para cada coordenada
                
                a[j][k] -= m * rij[k] / r3;  //aprovechamos acción y reacción
            }
            
            
        }
    }
    
    
    //sumamos energías cinéticas de cada partícula y energías potenciales considerando cada par de partículas
    
    Ekin = Epot = 0.0;
    
    for( i = 0; i < n ; i++){
        for( int j = i+1; j < n ; j++){
            for( int k = 0; k < 3; k++)
                rij[k] = r[j][k] - r[i][k];  //vector que va de particula i a particula j
            r2=0.0;
            for( int k = 0; k < 3; k++)
                r2 += rij[k]*rij[k];       //modulo cuadrado del vector entre i y j
            double dist = sqrt(r2);
            Epot -= m*m/dist;
        }
        for( int k = 0; k < 3; k++)
            Ekin+= 0.5 * m * v[i][k] * v[i][k];
    }
    
    printf("E tot inicial= %.10lf\n",Ekin+Epot);
    E_in=Ekin+Epot;
    

    tprint=0.0;

    out=fopen("leapfrog1_body1_fig8.qdp","w");
    out2=fopen("leapfrog1_body2_fig8.qdp","w");
    out3=fopen("leapfrog1_body3_fig8.qdp","w");
    out4=fopen("leapfrog1_body4_fig8.qdp","w");
    
    fprintf(out,"@forward_euler1.pco\n");

    
    //empezamos a iterar en el tiempo
    
    //for(ns=0; ns<300000; ns++){
    for(tiempo=0; tiempo<t_end; tiempo = tiempo + dt){
        
        for( i = 0; i < n ; i++){   // calculamos nueva velocidad (primer aumento) y nueva posición, para cada partícula i, y cada coordenada k
            
            for (k=0; k<3; k++){
                v[i][k] += 0.5 * a[i][k] * dt;
                r[i][k] += v[i][k] * dt;
            }
        }
        
        for( i = 0; i < n ; i++)
            for (k=0; k<3; k++)
                a[i][k] = 0.0;  // vamos a sumar componentes de las aceleraciones más abajo así que aquí las volvemos a poner en 0
        
        for( i = 0; i < n ; i++){
             for( int j = i+1; j < n ; j++){
                 for( int k = 0; k < 3; k++)
                     rij[k] = r[j][k] - r[i][k];  //vector que va de particula i a particula j
                 r2=0.0;
                 for( k = 0; k < 3; k++)
                     r2 += rij[k]*rij[k];
                 r3 = r2 * sqrt(r2);
                 for( k = 0; k < 3; k++){
                     a[i][k] += m * rij[k] / r3;  //vamos sumando las aceleraciones que siente i debido a cada j, para cada coordenada
                     
                     a[j][k] -= m * rij[k] / r3;  //aprovechamos acción y reacción
                 }
             }
        }
        
        for( i = 0; i < n ; i++){   // sumamos el segundo aumento a la  velocidad para cada partícula i, y cada coordenada k
                for (k=0; k<3; k++){
                    v[i][k] += 0.5 * a[i][k] * dt;
                }
            
        }
        
        if(tiempo>tprint){   //imprimimos las posiciones x,y de cada partícula (n=3) cada tprint tiempos
            
            //fprintf(out,"%lf %lf %lf %lf %lf %lf %lf %lf\n",ns*dt,r[0],r[1],r[2],v[0],v[1],v[2],sqrt(r2));
            
            fprintf(out,"%lf %lf \n",r[0][0],r[0][1]);
            fprintf(out2,"%lf %lf \n",r[1][0],r[1][1]);
            fprintf(out3,"%lf %lf \n",r[2][0],r[2][1]);
         //   fprintf(out4,"%lf %lf \n",r[3][0],r[3][1]);

            tprint+=0.1;
        }
    }
 
    fclose(out);
    fclose(out2);
    fclose(out3);
    fclose(out4);

    //sumamos las energías cinéticas de cada partícula y energías potenciales FINALES considerando cada par de partículas
    
    Ekin = Epot = 0.0;
    
    for( i = 0; i < n ; i++){
        for( int j = i+1; j < n ; j++){
            for( int k = 0; k < 3; k++)
                rij[k] = r[j][k] - r[i][k];  //vector que va de particula i a particula j
            r2=0.0;
            for( int k = 0; k < 3; k++)
                r2 += rij[k]*rij[k];       //modulo cuadrado del vector entre i y j
            double dist = sqrt(r2);
            Epot -= m*m/dist;
        }
        for( int k = 0; k < 3; k++)
            Ekin+= 0.5 * m * v[i][k] * v[i][k];
    }
    
    printf("E tot final= %.10lf\n",Ekin+Epot);
    E_out=Ekin+Epot;
    printf("error relativo= %.g\n",(E_in-E_out)/E_in);
    


 
}
