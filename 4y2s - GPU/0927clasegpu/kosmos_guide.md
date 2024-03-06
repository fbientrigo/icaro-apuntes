Mis datos para no olvidar

ftrigo@kosmos.ifa.uv.cl
lu*i9***11

Claro, aquí están los apuntes organizados y en formato Markdown:

```markdown
# Apuntes de Conexión y Compilación CUDA

## Conexión SSH

Para establecer una conexión SSH con el servidor, utiliza el siguiente comando:

```shell
ssh ftrigo@kosmos.ifa.uv.cl
```

En algún momento, se te pedirá la contraseña. Si deseas cambiarla, puedes utilizar el programa `passwd`.

## Compilación con CUDA

Para compilar un programa con CUDA, sigue estos pasos:

1. Conéctate al servidor:

```shell
ssh ftrigo@flare
```

2. Carga el módulo necesario:

```shell
module load nvhpc/20
```

3. Compila el programa CUDA:

```shell
nvcc hola_mundo.cu -o hola_mundo.x
```

4. Ejecuta el programa:

```shell
./hola_mundo.x
```

## Acceso Remoto

Si deseas acceder desde tu propia computadora, puedes utilizar el siguiente enlace:

[Repositorio en GitHub](https://github.com/graemecan/programacion_gpu)

## Comandos Útiles

- Ver información de las GPUs instaladas:

```shell
nvidia-smi
```

- Abrir un archivo en el editor de texto `gedit` desde la terminal:

```shell
gedit hola_mundo.cu
```

## Directivas de CUDA

Las siguientes directivas son importantes al trabajar con CUDA:

- `__global__`: Crea un kernel que se ejecuta en GPU y CPU.
- `__host__`: Indica que una función se ejecuta en la CPU.
- `__device__`: Indica que una función se llama desde la GPU y se ejecuta en la GPU.

Ejemplo de compilación con una arquitectura específica:

```shell
nvcc -arch=sm_50 hola_mundo.cu -o hola_mundo.x
```

## Copiar Archivos a Kosmos

Puedes copiar archivos desde tu computadora al servidor Kosmos utilizando `scp`. Por ejemplo:

```shell
scp -r hola_mundo.cu ftrigo@kosmos.ifa.uv.cl:/home/ftrigo/gpu/
```

## Pasar Archivos de Kosmos a la PC

Para copiar archivos desde Kosmos a tu PC local, es importante ejecutar este comando fuera del servidor, ya que estás haciendo una solicitud al servidor desde tu máquina local. Ejemplo:

```shell
scp ftrigo@kosmos.ifa.uv.cl:<ruta> ./
```

## Perfilado y Métricas

Para realizar el perfilado y obtener métricas de tu programa CUDA, puedes utilizar los siguientes comandos:

- Ejecutar el programa con `nvprof`:

```shell
nvprof ./suma_vectores_gpu.x
```

- Consultar métricas con `ncu`:

```shell
ncu ./suma_vectores_gpu.x --query-metrics
```

- Perfil visual con `nsys`:

```shell
nsys profile --stats=true ./suma_vectores_gpu.x
```

Recuerda que estos comandos te ayudarán a analizar el rendimiento y la eficiencia de tus aplicaciones CUDA.

¡Espero que estos apuntes sean útiles para tu programación en CUDA!
```

Estos apuntes proporcionan información detallada sobre la conexión SSH, la compilación con CUDA, directivas importantes de CUDA, comandos útiles y cómo realizar el perfilado y obtener métricas de tus programas CUDA. ¡Espero que te sean de utilidad!


# pasar una carpeta del PC a Kosmos
scp -r gpu_0927/ ftrigo@kosmos.ifa.uv.cl:/home/ftrigo/

