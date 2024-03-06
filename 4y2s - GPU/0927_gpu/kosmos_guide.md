


ftrigo@kosmos.ifa.uv.cl
lu******11

Para conectarse primero hay que establecer una conexion ssh
ssh ftrigo@kosmos.ifa.uv.cl

Entonces en algun momento pedira la contraseña, en caso de desear reemplazarla existe un programa llamado "passwd"


Para compilar con CUDA
ssh ftrigo@flare
module load nvhpc/20
nvcc hola_mundo.cu -o hola_mundo.x

Para poder ingresar de mi propio computador

https://github.com/graemecan/programacion_gpu
//Professional CUDA C Programming John Cheng, Max Grossman
//https://www.cs.utexas.edu/~rossbach/cs380p/papers/cuda-programming.pdf
//en el terminal
nvidia-smi
gedit hola_mundo.cu
__global__ #crea un kernel gpu y cpu
__host__ #corre en cpu
__device__ #llamar desde el gpu corre en gpu

nvcc -arch=sm_50 hola_mundo.cu -o hola_mundo.x //compilar el archivo
./hola_mundo.x | wc -l //contar numero de lineas
1024 hay un limite de threads por bloque


//para ingresar a kosmos y compilar con CUDA
ssh ftrigo@kosmos.ifa.uv.cl

ssh ftrigo@flare
module load nvhpc/20.9
nvcc hola_mundo.cu -o hola_mundo.x 

//para copiar archivos en kosmos
scp -r hola_mundo.cu ftrigo@kosmos.ifa.uv.cl:/home/ftrigo/gpu/  //Copiar un archivo desde el computador de la u a kosmos
ssh ftrigo@flare
module avail hola_mundo.cu -o hola_mundo.x  //module list
module load nvhpc/20.9
module list //opcional

nvcc hola_mundo.cu -o hola_mundo.x
./hola_mundo.x
//utilizar nano para copiar el codigo
nvcc suma_vectores_gpu.cu -o suma_vectores_gpu.x
./suma_vectores_gpu.x

nvprof ./suma_vectores_gpu.x
ncu ./suma_vectores_gpu.x --query-metrics //profile visual
nsys profile --stats=true ./suma_vectores_gpu.x

scp -r -oProxyJump=gc //desde mi computador a kosmos ?


# pasar archivos de kosmos a pc
Es importante correr esto de manera local, osea fuera del servidor, pues estamos de manera local haciendo una request para el servidor
//scp ftrigo@kosmos.ifa.uv.cl: <ruta> ./
//scp ftrigo@kosmos.ifa.uv.cl:/home/ftrigo/gpu/u_00000.dat ./Downloads

