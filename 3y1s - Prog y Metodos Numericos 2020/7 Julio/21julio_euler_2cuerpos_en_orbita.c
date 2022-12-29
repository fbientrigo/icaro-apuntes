# include <stdio.h>
# include <math.h>

int main(){

  int ns, k;
  double r[3], v[3], a[3];
  double dt = 0.1;
  double r2;
    FILE *out;

  r[0]=1;
  r[1]=0;
  r[2]=0;
  v[0]=0;
  v[1]=1;
  v[2]=0;

    out=fopen("forward_euler1.qdp","w");
    
    fprintf(out,"@forward_euler1.pco\n");

  for(ns=0; ns<3000;ns++){
    r2 = r[0]*r[0]+r[1]*r[1]+r[2]*r[2]; 
    for (k=0; k<3; k++){
      a[k] = -r[k]/(r2*sqrt(r2));
      r[k] += v[k]*dt;
      v[k] += a[k]*dt;
    }
      //fprintf(out,"%lf %lf %lf %lf %lf %lf %lf %lf\n",ns*dt,r[0],r[1],r[2],v[0],v[1],v[2],sqrt(r2));
          
      fprintf(out,"%lf %lf \n",r[0],r[1]);
  }
    fclose(out);
 
}
