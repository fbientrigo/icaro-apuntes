# README
Este proyecto se concentra en el uso de GPU y C, utilizando las herramientas de NVidia, dentro del curso se utiliza un servidor asi que se aportara una guia de uso cuando sea necesaria




# Programación GPU: Guía completa para usuarios de Kosmos y Flare

Este repositorio es un *fork* de las clases de GPU, enriquecido con notas y consejos que sirven como un manual de supervivencia para aquellos que desean adentrarse en la programación GPU en servidores académicos como Kosmos y Flare.

## Primera conexión y uso

Para iniciar, establezcamos una conexión SSH:

```bash
ssh user@kosmos.ifa.uv.cl
```

En este caso, usaremos `ftrigo` como usuario. Será necesario ingresar la contraseña, y si deseas cambiarla, puedes utilizar el comando `passwd`.

Para compilar con CUDA en Flare, ejecuta:

```bash
ssh ftrigo@flare
module load nvhpc/20
nvcc hola_mundo.cu -o hola_mundo.x
```

Si prefieres trabajar desde tu propio computador, puedes clonar el repositorio:

```bash
git clone https://github.com/graemecan/programacion_gpu
```

Adicionalmente, puedes consultar el libro *Professional CUDA C Programming* de John Cheng y Max Grossman, así como el [documento](https://www.cs.utexas.edu/~rossbach/cs380p/papers/cuda-programming.pdf) sobre programación CUDA.

Para verificar información sobre las GPUs, utiliza:

```bash
nvidia-smi
```

Algunos conceptos clave en la programación CUDA incluyen:

- `__global__`: Crea un kernel que se ejecuta en la GPU y la CPU.
- `__host__`: Corre en la CPU.
- `__device__`: Llamada desde la GPU, ejecutándose en ella.

Compilar un archivo CUDA se realiza con el comando:

```bash
nvcc -arch=sm_50 hola_mundo.cu -o hola_mundo.x
```

El límite de *threads* por bloque es 1024.

## Acceso a Kosmos y compilación con CUDA

Para ingresar a Kosmos y compilar con CUDA:

```bash
ssh ftrigo@kosmos.ifa.uv.cl
```

Y desde ahí, acceder a Flare:

```bash
ssh ftrigo@flare
module load nvhpc/20.9
nvcc hola_mundo.cu -o hola_mundo.x
```

## Transferencia de archivos entre Kosmos y PC

Para copiar archivos desde tu computador local a Kosmos:

```bash
scp -r hola_mundo.cu ftrigo@kosmos.ifa.uv.cl:/home/ftrigo/gpu/
```

Y para copiar archivos desde Kosmos a tu PC:

```bash
scp -r kosmos:/home/ftrigo/gpu/ ./Downloads
```

Recuerda ejecutar estos comandos localmente fuera del servidor. Si tienes Kosmos agregado como una clave pública en tu PC, puedes omitir `ftrigo@kosmos.ifa.uv.cl`.

## Descargar una carpeta

Si deseas descargar una carpeta completa desde Kosmos:

```bash
scp -r kosmos:/home/ftrigo/gpu/ ./Downloads
```

¡Ahora estás listo para sumergirte en el mundo de la programación GPU en servidores académicos!
