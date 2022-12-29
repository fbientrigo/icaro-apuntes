# include <stdio.h>
# include <math.h>

int main(){

  int ns, k;
  double r[3], v[3], a[3];
  double dt = 0.01;
  double r2;
  double tprint;
  double Ekin, Epot;
    
  FILE *out;

  r[0]=1;
  r[1]=0;
  r[2]=0;
  v[0]=0;
  v[1]=0.5;
  v[2]=0;
    
    Ekin = 0.5 * (v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    Epot = -1.0/sqrt(r[0]*r[0]+r[1]*r[1]+r[2]*r[2]);
    printf("E tot inicial= %lf\n",Ekin+Epot);

    tprint=0.0;

    out=fopen("leapfrog1_tprint.qdp","w");
    
    fprintf(out,"@forward_euler1.pco\n");

    //calculamos aceleración inicial

    r2 = r[0]*r[0]+r[1]*r[1]+r[2]*r[2];
    for (k=0; k<3; k++){
        a[k] = -r[k]/(r2*sqrt(r2));
    }

    
    //empezamos a iterar
    
    for(ns=0; ns<30000; ns++){
    
        //aumentamos la velocidad por la mitad de la aceleración anterior y la posicion por la velocidad y la aceleracion
        
        for (k=0; k<3; k++){
           // r[k] += v[k] * dt + 0.5 * a[k] * dt * dt;
           // v[k] += 0.5 * a[k] * dt;
            
            v[k] += 0.5 * a[k] * dt;
            r[k] += v[k] * dt;
        }
     
        //usamos nueva posición para calcular la aceleración y aumentamos la velocidad por la mitad de la aceleración actual
        
        r2 = r[0]*r[0]+r[1]*r[1]+r[2]*r[2];
   
        for (k=0; k<3; k++){
            a[k] = -r[k]/(r2*sqrt(r2));
            v[k] += 0.5 * a[k] * dt;
        }

        if(ns*dt>tprint){
          //fprintf(out,"%lf %lf %lf %lf %lf %lf %lf %lf\n",ns*dt,r[0],r[1],r[2],v[0],v[1],v[2],sqrt(r2));
          
          fprintf(out,"%lf %lf \n",r[0],r[1]);
          tprint+=0.1;
      }
  }
    fclose(out);
    
    Ekin = 0.5 * (v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    Epot = -1.0/sqrt(r[0]*r[0]+r[1]*r[1]+r[2]*r[2]);
    printf("E tot final= %lf\n",Ekin+Epot);

 
}
