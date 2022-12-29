//-----------------------------------------------------------------------------
// basado en el código hermite1.C creado por Piet Hut y Jun Makino y distribuido
// en su proyecto The Art of Computational Science http://www.artcompsci.org/msa/web/vol_1/v1_web/node6.html
//-----------------------------------------------------------------------------
#include  <stdio.h>
#include  <math.h>
#include  <stdlib.h>
#include <unistd.h>  //para getop()
#include <stdbool.h>

/*modulacion, todo esto funciona*/
double energy(const double m[],const double r[][3],const double v[][3], int n);

bool read_options(int argc, char *argv[], double *dt_param, double *dt_dia, double *dt_out, double *dt_tot, bool *i_flag,bool *x_flag, bool *c_flag);

void get_snapshot(double m[], double r[][3], double v[][3], int n);

void put_snapshot(const double m[], const double r[][3], const double v[][3], int n, double t);

void diagnostics (const double m[], const double r[][3], const double v[][3], int n, double t, long int counter, double e_in);

void center(const double m[], double v[][3], int n);
/*modulacion todo lo de arriba funciona*/

/*---------------Defina aqui sus funciones nuevas---*/
double ruk0(double m[],double r[][3],double v[][3],double a[][3],double kr[][12],double kv[][12],int n,double dt_param); //ruk0 es una funcion que entrega el valor de coll_time, esto debido a que el uso de pointers daba resultado a varios problemas

void ruk1(double m[],double r[][3],double v[][3],double a[][3],double kr[][12],double kv[][12],int n, double dt);

// ruk2 utiliza los valores conocidos de kr y kv para calcular los ultimos
//hasta ahora poseo k1r,k2r; k1v,k2v,k3v.
// k2v -> k3r -> k4v
// k3v -> k4r
void ruk2(double m[],double r[][3],double v[][3],double a[][3],double kr[][12],double kv[][12],int n, double dt);

//experimental
//esto se utiliza para intentar disminuir el energy_drift del metodo RK4
void correct_step(double r[][3], double v[][3], double a[][3], double old_r[][3], double old_v[][3], double old_a[][3], int n, double dt);
/*
* Resultados:
* el error antiguo sin esta funcion era de un 43% y ahora disminuyo a solo un 0.2%
* se muestra una clara mejora con la adicion de esta funcion
*
*/






/*--------Codigo Principal-------------
* main toma de input los argumentos que son pasados a read_options
*
* la funcion funciona en este orden:
* define parametros de tiempo por default
* lee la cantidad de cuerpos que existe: n
* lee el tiempo con el que comenzamos
*
* crea los arreglos m[], r[][], v[][], kr[][], kv[][]
*
* get_snapshot lee el estado inicial de los cuerpos, sus pos. y vel.
*
* revisa c_flag, en caso de haber corrido main con "-c" no hara correccion del centro de masa
*
* imprime las condiciones iniciales para guardarlas en un archivo de salida
*
* evoluciona el estado mediante algun algoritmo
*
------------------------------------------*/
int main(int argc, char *argv[])
{
    double dt_param = 0.03;     // control parameter to determine time step size
    double dt_dia = 1;          // time interval between diagnostics output
    double dt_out = 1;          // time interval between output of snapshots
    double dt_tot = 10;         // duration of the integration
    bool init_out = false;    // if true: snapshot output with start at t = 0
                                //          with an echo of the input snapshot
    bool  x_flag = false;      // if true: extra debugging diagnostics output
    bool c_flag = true; // if false no se hace la correccion del centro de masa
\
    if (! read_options(argc, argv, &dt_param, &dt_dia, &dt_out, &dt_tot, &init_out,
                       &x_flag, &c_flag))
        return 1;                // halt criterion detected by read_options()

    int n;
    scanf("%d",&n);

    double t_start;
    scanf("%lf",&t_start);

    double m[n];
    double r[n][3];
    double v[n][3];
    double a[n][3];

    double coll_time, dt;

    /*---------- RK4 --------------
    * kv1 = a, kv2 = a(+h/2),.. estan dentro de kv[][12] (contiene 4 arreglos de 3)
    * kr contine a su vez los 4 vectores 3d de la pendiente de r
    * kr[][12]
    *------------------------------*/
    double kr[n][12];
    double kv[n][12];

    double e_out,e_in;
    long int counter=0;

    /*se inicializan con valores 0 de manera de que la primera iteracion no tenga problemas para calcular kr2 y kv2*/
    for(int i = 0.0; i < n; i++){
      for(int k = 0.0; k < 3; k++){
        kr[i][k] = 0.0;
        kv[i][k] = 0.0;
        a[i][k] = 0.0;
      }
    }
    //printf("def de a, kr y kv funciona");




    /*-----------------------*/
    //esto actualiza los valores de n,m,r,v
    get_snapshot(m,r,v,n);

    /*-----Modificacion Centro de Masa CM----*/
    // Centro de masa, aqui se analiza un flag el cual por default es true
    // osea que siempre solemos aplicar el centro de masa antes

    if(c_flag){
    center(m,v,n);}

    //printf("antes de put_snapshot");
    put_snapshot(m,r,v,n,t_start);

    /*La integracion comienza aqui*/

    /*------------ de aqui implemento evolve---------------- */
    //necesito una funcion de evolve beta para probar el codigo y no hacer cosas a ciegas

    //abrimos los archivos de salida para escupir los datos
    FILE *out, *out2, *out3;

    out=fopen("herminte1_body1.qdp","w");
    out2=fopen("herminte1_body2.qdp","w");
    out3=fopen("herminte1_body3.qdp","w");

    /*-----------------------------RK4 preparativos------------------------------------------*/
    //primeros valores de k1v y k1r, y calculamos dt
    //printf("coll time antes: %lf\n",coll_time);
    coll_time = ruk0(m,r,v,a,kr,kv,n,dt_param);
    printf("coll time despues: %lf\n",coll_time);
    dt = (coll_time) * (dt_param);
    //printf("dt = %lf",dt);
    // funcion para calcular k2r y k2v (como bonus calculamos k3v)
    ruk1(m,r,v,a,kr,kv,n,dt);

    //hasta ahora poseo k1r,k2r; k1v,k2v,k3v.
    // k2v -> k3r -> k4v
    // k3v -> k4r
    ruk2(m,r,v,a,kr,kv,n,dt);


    e_in = energy(m,r,v,n);

    fprintf(stderr, "Initial total energy E_in = %.10lf\n",e_in );



    double t_out = dt_out + t_start;

    double t_dia = dt_dia + t_start;

    double t_tot = dt_tot + t_start;

    double t=t_start;


    while(true){
        /* -------------- Aqui va todo lo que viene a ser el el step de la evolcion ------------ */

        double old_r[n][3];
        double old_v[n][3];
        double old_a[n][3];

        // experimental: objetivo, disminuir el control del integrador
        // podemos pensar que es como suavizarlo, evitamos que diverga muy rapido
        // guardar condiciones anteriores
        for (int i = 0; i < n; i++){
            for (int k = 0; k < 3; k++){
                old_r[i][k] = r[i][k];
                old_v[i][k] = v[i][k];
                old_a[i][k] = a[i][k];
            }
        }

        //utilizamos los vectores de pendientes calculadas por el algoritmo de RK4
        //esto actualiza los vectores de posicion y velocidad
        for (int i = 0; i < n; i++){
            for (int k = 0; k < 3; k++){


                r[i][k] += (dt/6) * (kr[i][k] + 2 * kr[i][k+3] + 2 * kr[i][k+6] + kr[i][k+9] ) + (dt * dt * 0.5) * a[i][k];
                v[i][k] += (dt/6) * (kv[i][k] + 2 * kv[i][k+3] + 2 * kv[i][k+6] + kv[i][k+9] );
            }
        }

        //experimental
        // promediamos las velocidades obtenidas con las antiguas
        correct_step(r,v,a,old_r,old_v,old_a,n,dt);

        // obtener nuevo acc y collision time
        coll_time = ruk0(m,r,v,a,kr,kv,n,dt_param);
        if(coll_time == 0){
          coll_time = 0.000001;
        }
        dt = dt_param * (coll_time);
        t += dt;
        //printf("dt %lf",dt);
        /* ---------------aqui termina esto de la integracion-------------- */


        if (t >= t_dia){
            diagnostics(m,r,v,n,t,counter, e_in);
            t_dia += dt_dia;
        }

        if (t >= t_out){
            fprintf(out,"%lf %lf \n",r[0][0],r[0][1]);
            fprintf(out2,"%lf %lf \n",r[1][0],r[1][1]);
            fprintf(out3,"%lf %lf \n",r[2][0],r[2][1]);
            put_snapshot(m,r,v,n,t);

            t_out += dt_out;
        }

        if ( t >= t_tot)
            break;

        counter++;
    }
    e_out=energy(m,r,v,n);

    fprintf(stderr,"Final total energy E_in = %.10lf\n",e_out );
    fprintf(stderr,"absolute energy error: E_out - E_in = %g\n",e_out-e_in );
    fprintf(stderr,"relative energy error: (E_out - E_in) / E_in =  %g\n",(e_out-e_in)/e_in );
    fprintf(stderr,"number of steps= %ld\n",counter);

    fclose(out);
    fclose(out2);
    fclose(out3);



}

// Calcular la aceleracion y distancia entre los cuerpos para generar los primeros valores de kr y kv
/*
* k1r, k1v
*
* la input dt_param esta agregada debido a intentos de incluir la nueva definicion del tiempo dt dentro de esta funcion, sin embargo dejo varios problemas este hecho
*/
double ruk0(double m[],double r[][3],double v[][3],double a[][3],double kr[][12],double kv[][12],int n,double dt_param){


      double coll_time_sq=1.e300;
      double coll_est_sq, coll_time_limit=10.0;

      /* inicializa aceleracion, no utilizamos el jerk */
      for (int i = 0; i < n; i++)
          for (int k = 0; k < 3; k++){
              a[i][k] = 0.0;

              /*---------RK4---------*/
              /* inicia los primeros 3 valores de kr, lo cual
              es equivalente a k1_r de acuerdo a la teoria*/
              kr[i][k] = v[i][k];
            }

      /*doble loop para calcular distancias y aceleracion
      asi como el tiempo minimo de colision*/
      for (int i = 0; i < n; i++){
          for (int j = i+1; j < n; j++){

              double rji[3], vji[3];
              for (int k = 0; k < 3; k++){
                  rji[k] = r[j][k] - r[i][k];
                  vji[k] = v[j][k] - v[i][k];

                  /*-----RK4---- intento de definir k2v*/
                  /*aprovechar este loop para calcular el siguiente k2v de ruk kuta, se usaria para crear ri + k1_ri h/2 sin embargo no poseo h/2 dentro de este codigo, habria que darselo como input, pero eso me genero un monton de problemas, dejare esto para recordar mi fallo*/
              }

              // estas variables se reinician en cada loop
              double r2 = 0;
              double v2 = 0; // esta es usada para timpo colision

              //RK4
              //double r2r = 0;
              for (int k = 0; k < 3; k++){
                  r2 += rji[k] * rji[k];
                  v2 += vji[k] * vji[k]; //usado para coll_time

                  //r2r += rjir[k]*rjir[k];

              }

              coll_est_sq=r2/v2; //nuevo
              if(coll_time_sq > coll_est_sq) //nuevo
                  coll_time_sq = coll_est_sq; //nuevo (encontramos minimo valor de coll_time_sq)

              // calculo de la aceleracion
              double r3 = r2 * sqrt(r2);

              //RK4
              //double r3r = r2r * sqrt(r2r);
              for (int k = 0; k < 3; k++){
                  a[i][k] += m[j] * rji[k] / r3;
                  a[j][k] -= m[i] * rji[k] / r3;

                  //RK4, esto es parte del fiasco de no poseer dt aqui dentro
                  // k+3 nos movemos al segundo vector k2_vi
                  //kv[i][k+3] += m[j] * rjir[k] / r3r;
                  //kv[j][k+3] -= m[i] * rjir[k] / r3r;

              }
          } // fin de loop j
      } // fin de loop de particulas

      /*--------RK4----------*/
      //guardar la aceleracion a[][3] que fue terminada de calcular en el loop de arriba
      for(int i=0; i<n; i++){
        for(int k=0; k<3; k++)
          kv[i][k] = a[i][k]; //k1v = a
      }


      if (coll_time_sq > coll_time_limit * coll_time_limit)
          coll_time_sq = coll_time_limit * coll_time_limit;

      return sqrt(coll_time_sq);
  }

// funcion para calcular k2r y k2v (como bonus calculamos k3v)
// esta funcion asume que poseemos k1r y k1v
// la funcion ruk0 hace el trabajo de calcular le tiempo de colision y cambiar el dt, para facilitar y saltar errores previos solo tomara dt ya calculado en el main
void ruk1(double m[],double r[][3],double v[][3],double a[][3],double kr[][12],double kv[][12],int n, double dt){

  for(int i = 0; i < n; i++){
    for(int k = 0; k < 3; k++){
      //completo el valor de k2r
      // de acuerdo al archivo enviado en .pdf, por temas de unidades asumo que va una suma en esta operacion, ya que eso concuerda con las unidades
      kr[i][k+3] = v[i][k] + kv[i][k] * dt/2;

    }} //los dejare juntos para indicar que son de la misma estructura o funcion

    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){

            double rji2[3]; // se utiliza para k2_v
            double rji3[3]; // se usa para k3_v
            for (int k = 0; k < 3; k++){

                //k1r calculada con ruk0
                rji2[k] = r[j][k] + kr[j][k] * dt/2 - r[i][k] -  kr[i][k] * dt/2;

                //aprovechamos k2r que lo tenemos del loop anterior
                rji3[k] = r[j][k] + kr[j][k+3] * dt/2 - r[i][k] -  kr[i][k+3] * dt/2;
            }

            //calculo el cuadrado de las distancias
            //esto sucede entre cada particulas
            //recordemos estamos dentro de un loop de particulas ij
            double r22 = 0;
            double r23 = 0;
            for (int k = 0; k < 3; k++){
                r22 += rji2[k] * rji2[k];
                r23 += rji3[k] * rji3[k];
            }

            // calculo de la k2v y k3v
            // llamo a k2v = kv2, son lo mismo, escribalo o recuerdelo de la forma que le sea conveniente, a mi se me olvida entonces esas dos notaciones son lo mismo
            double r32 = r22 * sqrt(r22); //para k2v
            double r33 = r23 * sqrt(r23); //para k3v
            double ep = 1.e-11;

            for (int k = 0; k < 3; k++){
                //RK4
                // k+3 nos movemos al segundo vector k2_vi
                // k+6 nos move al tercer vector k3_vi
                kv[i][k+3] += m[j] * rji2[k] / (r32 + ep);
                kv[j][k+3] -= m[i] * rji2[k] / (r32 + ep);

                kv[i][k+6] += m[j] * rji3[k] / (r33 + ep);
                kv[j][k+6] -= m[i] * rji3[k] / (r33 + ep);
            }
        }}


         //loop entre particulas i y j
} // fin de ruk1

// ruk2 utiliza los valores conocidos de kr y kv para calcular los ultimos
//hasta ahora poseo k1r,k2r; k1v,k2v,k3v.
// k2v -> k3r (listo) -> k4v (listo)
// k3v -> k4r (listo)
void ruk2(double m[],double r[][3],double v[][3],double a[][3],double kr[][12],double kv[][12],int n, double dt){

  for(int i = 0; i < n; i++){
    for(int k = 0; k < 3; k++){
      //completo el valor de k3r (este es necesario para el ultimo k4v :) estamos cerca)
      kr[i][k+6] = v[i][k] + kv[i][k+3] * dt/2;

      // k4r se puede calcular altiro con el bonus de k3v uwu, notece que es dt y no dt/2, esto es por que k4r es la pendiente al final del step dt (vea RK4 en wikipedia para mas info)
      kr[i][k+9] = v[i][k] + kv[i][k+6] * dt;

    }} //los dejare juntos para indicar que son de la misma estructura o funcion

    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){ //loop de interaccion entre particulas

            double rji4[3]; // se utiliza para k4_v
            for (int k = 0; k < 3; k++){

                rji4[k] = r[j][k] + kr[j][k+6] * dt - r[i][k] -  kr[i][k+6] * dt;
            }

            //calculo el cuadrado de las distancias
            //esto sucede entre cada particulas
            //recordemos estamos dentro de un loop de particulas ij
            double r24=0;
            for (int k = 0; k < 3; k++){
                r24 += rji4[k] * rji4[k];
            }

            // calculo de la k4v
            double r34 = r24 * sqrt(r24); //para k4v
            double ep = 1.e-11;
            for (int k = 0; k < 3; k++){
                //RK4
                // k+9 nos movemos al ultimo vector k4_vi
                kv[i][k+9] += m[j] * rji4[k] / (r34 + ep);
                kv[j][k+9] -= m[i] * rji4[k] / (r34 + ep);

            }
        }} //loop entre particulas i y j
}


//agregare un correct_step para restarle poder al integrador y disminuir energy_drift
void correct_step(double r[][3], double v[][3], double a[][3], double old_r[][3], double old_v[][3], double old_a[][3], int n, double dt){

    for (int i = 0; i < n; i++){
        for (int k = 0; k < 3; k++){
            v[i][k] = old_v[i][k] + (old_a[i][k] + a[i][k])*dt/2;
            r[i][k] = old_r[i][k] + (old_v[i][k] + v[i][k])*dt/2 + (old_a[i][k] - a[i][k])*dt*dt/12;
        }}
}



















/*Modulacion de codigo, todo aqui debajo funciona correctamente y es usado unicamente para fines de diagnostico e impresion de informacion*/
/****************************************************************************
* energy: calcula la energia del sistema, la energia cinetica clasica
*  y la energia potencial gravitatoria (no toma en cuenta la energia potencial efectiva)
*
* inputs: (masa: m, vector posicion: r, vector velocidad: v, numero particulas: n)
*
* funcion del tipo diagnostico: osea todos sus inputs son constantes, no hace
* ninguna modificacion
*
*
****************************************************************************/

double energy(const double m[],const double r[][3],const double v[][3], int n){
  double ekin = 0, epot = 0;
  for (int i = 0; i < n; i++){
    for (int j = i+1; j < n; j++){
        double rji[3];
        for (int k = 0; k < 3; k++)
            rji[k] = r[j][k] - r[i][k];
        double r2 = 0;
        for (int k = 0; k < 3; k++)
            r2 += rji[k] * rji[k];
        double r = sqrt(r2);
        epot -= m[i]*m[j]/r;
    }
    for (int k = 0; k < 3; k++)
        ekin += 0.5 * m[i] * v[i][k] * v[i][k];
        }
        double e_in = ekin + epot;

    return(e_in);
}

/*-----------------------------------------------------------------------------
 *  read_option: lee las opciones de la linea de comando y las implementa
 *  corra el programa con -h para mas info
 *
 *  note: cuando llamamos a la opcion -h (o cualquier letra no incluida)
 *  la ejecucion del programa se detiene
 *  y no habra mas output del programa que las instrucciones de como correr el
 *  programa
 *-----------------------------------------------------------------------------*/
bool read_options(int argc, char *argv[], double *dt_param, double *dt_dia,double *dt_out, double *dt_tot, bool *i_flag,bool *x_flag,bool *c_flag){
    int c;
    while ((c = getopt(argc, argv, "hixcd:e:o:t:")) != -1)   //entre "" van las opciones, los que tienen : despues de la letra tiene argumentos.

        switch(c){
            case 'd': *dt_param = atof(optarg);
                break;
            case 'e': *dt_dia = atof(optarg);
                break;
            case 'i': *i_flag = true;
                break;
            case 'o': *dt_out = atof(optarg);
                break;
            case 't': *dt_tot = atof(optarg);
                break;
            case 'x': *x_flag = true;
                break;
            case 'c': *c_flag = false;
                break;
            case 'h':
            case '?': fprintf(stderr,"uso de: %s\n[-h (para ayuda)]\n[-d step_size_control_parameter]\t [-e diagnostics_interval]\n[-o output_interval]\t\t\t   [-t total_duration]\n[-i (comienza el output con t = 0)]\t[-x (diagnosticos extras para debuggin)]\n[-c (quita la correccion del centro de masa para la velocidad)]\n", argv[0]);

                return false;      // en el caso de pedir ayuda o cualquier comando no compatible
                                  // la ejecucion se detiene
        }

    return true;                         // ready to continue program execution
}

/********************************************
* get_snapshot: lee el archivo que le demos y modifica los arreglos
* masa: m, vector posicion r, y vector velocidad v.
* input: archivo externo
*
* sintaxis del archivo externo (// comentario)
*
* n // numero de particulas
* m[1] r[1][1] r[1][2] r[1][3] v[1][1] v[1][2] v[1][3]
* // masa;  rx, ry, rz; vx, vy, vz
* m[2] r[2][1] r[2][2] r[2][3] v[2][1] v[2][2] v[2][3]
* ...
*
***********************************************/
void get_snapshot(double m[], double r[][3], double v[][3], int n){

    for (int i = 0; i < n ; i++){
        scanf("%lf", &m[i]);
        for (int k = 0; k < 3; k++)
            scanf("%lf",&r[i][k]);
        for (int k = 0; k < 3; k++)
            scanf("%lf", &v[i][k]);
        }
    //printf("get_snapshot funciona correctamente");
}

/*******************************
* put_snapshot: imprime los datos de la simulacion de la misma manera que
* los lee, ver sintaxis de get_snapshot
*
* input: m[] (arreglo de masas), r[][3] (arreglo de vector posicion), v[][3]
* ..(arreglo de vector velocidad), n (numero particulas), t (tiempo actual)
*
* no modifica ningun dato, unicamente imprime o en la pantalla o a otro archivo
* mediante pipes
*
********************************/
void put_snapshot(const double m[], const double r[][3], const double v[][3], int n, double t){

    printf("%d \n", n);
    printf("%lf \n", t);

    for(int i = 0; i < n ; i++){
        printf("%.10lf ", m[i]);
        for (int k = 0; k < 3; k++)
            printf("%.10lf ",r[i][k]);
        for (int k = 0; k < 3; k++)
            printf("%.10lf ", v[i][k]);
        printf("\n");
    }

}
/*******************************
*
*
*
*
*********************************/
void diagnostics (const double m[], const double r[][3], const double v[][3], int n, double t, long int counter, double e_in){

    double e_out=energy(m,r,v,n);
    fprintf(stderr,"relative energy error: (E_out - E_in) / E_in =  %g\n",(e_out-e_in)/e_in );
    fprintf(stderr,"tiempo actual =%lf , pasos dados= %ld\n",t, counter );
}

/******************************
* center: corrige los vectores velocidades para que se tomen respecto al centro
* de masa
*
* input: m[], v[][3], n
*
* esta funcion se encuentra activada en el main por default, para desactivarla
* basta con correr el programa con la opcion "-c" y la correcion no se hara
*
***********************************/

void center(const double m[], double v[][3], int n){

    // inicializa el arreglo de centro de masa con 0, recordemos C no hace
    // esto por default e inicializa con valores aleatorios
    double vcom [3];
    for( int k = 0; k < 3; k++)
      vcom[k]= 0.0;

    for( int k = 0; k < 3; k++){
        double Mtot = 0;
        for( int i = 0; i < n ; i++){
            vcom[k]+= m[i] * v[i][k];
            Mtot += m[i];
        }
        vcom[k] /= Mtot;

        // aqui es donde cambia los vectores velocidad de cada particula
        for( int i = 0; i < n ; i++){
            v[i][k] -= vcom[k];
        }
    }
}
/**********************************/
