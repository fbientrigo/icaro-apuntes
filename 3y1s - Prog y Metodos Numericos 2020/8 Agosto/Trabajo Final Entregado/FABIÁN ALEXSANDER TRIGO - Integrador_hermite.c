//-----------------------------------------------------------------------------
// basado en el código hermite1.C creado por Piet Hut y Jun Makino y distribuido
// en su proyecto The Art of Computational Science http://www.artcompsci.org/msa/web/vol_1/v1_web/node6.html
//-----------------------------------------------------------------------------
#include  <stdio.h>
#include  <math.h>
#include  <stdlib.h>
#include <unistd.h>  //para getop()
#include <stdbool.h>

double energy(const double m[],const double r[][3],const double v[][3], int n);
void acc_jerk(double m[],double r[][3],double v[][3],double a[][3],double jk[][3],int n, double *coll_time);

bool read_options(int argc, char *argv[], double *dt_param, double *dt_dia, double *dt_out, double *dt_tot, bool *i_flag, bool *x_flag);

void get_snapshot(double m[], double r[][3], double v[][3], int n);

void put_snapshot(const double m[], const double r[][3], const double v[][3], int n, double t);

void diagnostics (const double m[], const double r[][3], const double v[][3], int n, double t, long int counter, double e_in);

void center(const double m[], double v[][3], int n);

void predict_step(double r[][3], double v[][3], double a[][3], double jk[][3], int n, double dt);

void correct_step(double r[][3], double v[][3], double a[][3], double jk[][3], double old_r[][3], double old_v[][3], double old_a[][3], double old_j[][3], int n, double dt);

void evolve_step(double m[3], double r[][3], double v[][3], double a[][3], double jk[][3], int n, double dt, double *t, double *coll_time);

void evolve(double m[3], double r[][3], double v[][3], int n, double t_start, double dt_param, double dt_dia, double dt_out, double dt_tot, double init_out, bool x_flag);

void correct_step(double r[][3], double v[][3], double a[][3], double jk[][3], double old_r[][3], double old_v[][3], double old_a[][3], double old_j[][3], int n, double dt);

int main(int argc, char *argv[])
{
    double  dt_param = 0.03;     // control parameter to determine time step size
    double  dt_dia = 1;          // time interval between diagnostics output
    double  dt_out = 1;          // time interval between output of snapshots
    double  dt_tot = 10;         // duration of the integration
    bool  init_out = false;    // if true: snapshot output with start at t = 0
                                //          with an echo of the input snapshot
    bool  x_flag = false;      // if true: extra debugging diagnostics output
    
    if (! read_options(argc, argv, &dt_param, &dt_dia, &dt_out, &dt_tot, &init_out,
                       &x_flag))
        return 1;                // halt criterion detected by read_options()

    int n;
    scanf("%d",&n);
    
    double t_start;
    scanf("%lf",&t_start);
    
    
    double m[n];
    double r[n][3];
    double v[n][3];
    
    get_snapshot(m,r,v,n);

    center(m,v,n);
    
    put_snapshot(m,r,v,n,t_start);
    
    evolve(m,r,v,n, t_start,dt_param, dt_dia, dt_out, dt_tot, init_out, x_flag);
    
}


//-----------------------------------------------------------------------------

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

void acc_jerk(double m[],double r[][3],double v[][3],double a[][3],double jk[][3],int n, double *coll_time ){

    double coll_time_sq=1.e300;
    double coll_est_sq, coll_time_limit=10.0;
    
    for (int i = 0; i < n; i++)
        for (int k = 0; k < 3; k++)
            a[i][k] = jk[i][k] = 0.0;
    
    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){
            double rji[3], vji[3];
            for (int k = 0; k < 3; k++){
                rji[k] = r[j][k] - r[i][k];
                vji[k] = v[j][k] - v[i][k];
            }
            double r2 = 0;
            double v2 = 0; //nuevo
            for (int k = 0; k < 3; k++){
                r2 += rji[k] * rji[k];
                v2 += vji[k] * vji[k]; //nuevo
            }
            coll_est_sq=r2/v2; //nuevo
            if(coll_time_sq > coll_est_sq) //nuevo
                coll_time_sq = coll_est_sq; //nuevo (encontramos minimo valor de coll_time_sq)
            
            double r3 = r2 * sqrt(r2);
            double rv = 0;
            for (int k = 0; k < 3; k++)
                rv += rji[k] * vji[k];
            rv /= r2;
            for (int k = 0; k < 3; k++){
                a[i][k] += m[j] * rji[k] / r3;
                a[j][k] -= m[i] * rji[k] / r3;
                jk[i][k] += m[j] * (vji[k] - 3 * rv * rji[k]) / r3;
                jk[j][k] -= m[i] * (vji[k] - 3 * rv * rji[k]) / r3;
            }
        }
    }
    if (coll_time_sq > coll_time_limit * coll_time_limit)
        coll_time_sq = coll_time_limit * coll_time_limit;
    *coll_time = sqrt(coll_time_sq);

}


/*-----------------------------------------------------------------------------
 *  read_options  --  reads the command line options, and implements them.
 *
 *  note: when the help option -h is invoked, the return value is set to false,
 *        to prevent further execution of the main program; similarly, if an
 *        unknown option is used, the return value is set to false.
 *-----------------------------------------------------------------------------
 */

bool read_options(int argc, char *argv[], double *dt_param, double *dt_dia,
                  double *dt_out, double *dt_tot, bool *i_flag, bool *x_flag)
{
    int c;
    while ((c = getopt(argc, argv, "hixd:e:o:t:")) != -1)   //entre "" van las opciones, los que tienen : despues de la letra tiene argumentos.
        
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
            case 'h':
            case '?': fprintf(stderr,"usage: %s \n [-h (for help)] \n[-d step_size_control_parameter]\t [-e diagnostics_interval] \n[-o output_interval]\t\t\t [-t total_duration] \n[-i (start output at t = 0)]\t\t[-x (extra debugging diagnostics)]\n", argv[0]);
                
                return false;      // execution stops after help or error
        }
    
    return true;                         // ready to continue program execution
}

/*****************
*/

void get_snapshot(double m[], double r[][3], double v[][3], int n){
    
    for (int i = 0; i < n ; i++){
        scanf("%lf", &m[i]);
        for (int k = 0; k < 3; k++)
            scanf("%lf",&r[i][k]);
        for (int k = 0; k < 3; k++)
            scanf("%lf", &v[i][k]);
            }
}

/*******************************/

void put_snapshot(const double m[], const  double r[][3], const double v[][3], int n, double t){
    
    printf("%d \n", n);
    printf("%lf \n", t);

    for (int i = 0; i < n ; i++){
        printf("%.10lf ", m[i]);
        for (int k = 0; k < 3; k++)
            printf("%.10lf ",r[i][k]);
        for (int k = 0; k < 3; k++)
            printf("%.10lf ", v[i][k]);
        printf("\n");
}
}
/*******************************/


void diagnostics (const double m[], const double r[][3], const double v[][3], int n, double t, long int counter, double e_in)
{
    
    double e_out=energy(m,r,v,n);
    fprintf(stderr,"relative energy error: (E_out - E_in) / E_in =  %g\n",(e_out-e_in)/e_in );
    fprintf(stderr,"tiempo actual =%lf , pasos dados= %ld\n",t, counter );
}

/*******************************/

void center(const double m[], double v[][3], int n){

    double vcom [3];
    for( int k = 0; k < 3; k++)
    vcom [k]= 0.0;

    for( int k = 0; k < 3; k++){
        double Mtot = 0;
        for( int i = 0; i < n ; i++){
            vcom[k]+= m[i] * v[i][k];
            Mtot += m[i];
        }
        vcom[k] /= Mtot;
        for( int i = 0; i < n ; i++){
            v[i][k] -= vcom[k];
        }
    }
}


/**********************************/
void predict_step(double r[][3], double v[][3], double a[][3], double jk[][3], int n, double dt){

    for (int i = 0; i < n; i++){
        for (int k = 0; k < 3; k++){
        
            r[i][k] += v[i][k]*dt + a[i][k]*dt*dt/2 + jk[i][k]*dt*dt*dt/6;
            v[i][k] += a[i][k]*dt + jk[i][k]*dt*dt/2;
        }
    }
}
/**********************************/

void correct_step(double r[][3], double v[][3], double a[][3], double jk[][3], double old_r[][3], double old_v[][3], double old_a[][3], double old_j[][3], int n, double dt){
    
    for (int i = 0; i < n; i++){
        for (int k = 0; k < 3; k++){
            v[i][k] = old_v[i][k] + (old_a[i][k] + a[i][k])*dt/2
            + (old_j[i][k] - jk[i][k])*dt*dt/12;
            r[i][k] = old_r[i][k] + (old_v[i][k] + v[i][k])*dt/2
            + (old_a[i][k] - a[i][k])*dt*dt/12;
        }
    }
}

/**********************************/

void evolve_step(double m[3], double r[][3], double v[][3], double a[][3], double jk[][3], int n, double dt, double *t, double *coll_time){

    double old_r[n][3];
    double old_v[n][3];
    double old_a[n][3];
    double old_j[n][3];
    
    
    // guardar condiciones anteriores

    for (int i = 0; i < n; i++){
        for (int k = 0; k < 3; k++){
            old_r[i][k] = r[i][k];
            old_v[i][k] = v[i][k];
            old_a[i][k] = a[i][k];
            old_j[i][k] = jk[i][k];
        }
    }


    // dar paso predictor de nueva pos, vel

    predict_step(r,v,a,jk,n,dt);

    // obtener nuevo acc, jerk y collision time

    acc_jerk(m,r,v,a,jk,n,coll_time);

    //dar paso corrector de pos y vel

    correct_step(r,v,a,jk,old_r,old_v,old_a,old_j,n,dt);
    
    *t += dt;
}

/******************************/

void evolve(double m[3], double r[][3], double v[][3], int n, double t_start, double dt_param, double dt_dia, double dt_out, double dt_tot, double init_out, bool x_flag){
    
    double e_out,e_in;
    double coll_time,dt;
    long int counter=0;
    
    double a[n][3];
    double jk[n][3];


    FILE *out, *out2, *out3;

    out=fopen("herminte1_body1.qdp","w");
    out2=fopen("herminte1_body2.qdp","w");
    out3=fopen("herminte1_body3.qdp","w");

    acc_jerk(m,r,v,a,jk,n,&coll_time);

    dt = dt_param * coll_time;

    e_in = energy(m,r,v,n);

    fprintf(stderr, "Initial total energy E_in = %.10lf\n",e_in );

    double t_out = dt_out + t_start;

    double t_dia = dt_dia + t_start;

    double t_tot = dt_tot + t_start;

    double t=t_start;

    while(true){
        evolve_step(m,r,v,a,jk,n,dt,&t,&coll_time);
    
        dt = dt_param * coll_time;
    
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
