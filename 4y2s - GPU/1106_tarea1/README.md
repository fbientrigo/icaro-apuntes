A y B son matrices de 1024 x 1024 elementos. Todos los elementos de la matriz A son igual a 1. Todos los elementos de la matriz B son igual a 2. En esta tarea hay que escribir programas que calculan la suma de esta dos matrices, es decir los valores de la matriz C, donde C = A + B.

1) Escribir un código (en C) que calcula la suma en una forma secuencial. Debes incluir una confirmación de que el resultado es correcto (puede ser simplemente por verificación en un ciclo que todos los elementos de C son igual a 3).

2) Escribir otro código que implementa el cálculo en el GPU. Hay que verificar que el código corre bien en Flare.

3) Escribir otro código que implementa el cálculo en el GPU, usando memoria compartida (asignada dinámicamente).

4) Escribir otro código que implementa el cálculo en el GPU, usando memoria global, pero repartiendo el cálculo entre 4 "streams" que corren asincrónicamente (es decir, en paralelo).

Tus códigos deberían funcionar para cualquier elección (razonable) de número de bloques/threads.

____

# Docs Propia

## decoradores host, device, global
En CUDA, `__global__` es un especificador de función que indica que una función es un **kernel CUDA**, es decir, una función que se ejecuta en el GPU y debe ser llamada desde el host (CPU). 

1. `__global__`: Este especificador se usa para las funciones del kernel que se ejecutan en el dispositivo (GPU) pero son llamadas desde el host (CPU). Las funciones `__global__` deben ser de tipo `void`.

2. `__device__`: Este especificador se usa para las funciones que se ejecutan en el dispositivo (GPU) y son llamadas desde el dispositivo (GPU). Estas funciones sólo pueden ser llamadas desde otras funciones `__global__` o `__device__`.

3. `__host__`: Este especificador se usa para las funciones que se ejecutan en el host (CPU) y son llamadas desde el host (CPU). Si no se especifica ningún prefijo para una función, se asume que es `__host__`.

las funciones del kernel son llamadas desde el host (CPU) y se ejecutan en el dispositivo (GPU). Cuando llamas a una función del kernel, puedes pasarle parámetros, que son copiados desde la memoria del host a la memoria del dispositivo.

## cudaMemcpy
En cuanto a la función `cudaMemcpy`, se utiliza para copiar datos entre el host y el dispositivo. Aquí está su sintaxis:

```c
cudaError_t cudaMemcpy ( void* dst, const void* src, size_t count, cudaMemcpyKind kind )
```

- `dst`: Es un puntero al destino de la memoria donde se copiarán los datos.
- `src`: Es un puntero a la fuente de la memoria desde donde se copiarán los datos.
- `count`: Es el número de bytes a copiar.
- `kind`: Es un enumerado de tipo `cudaMemcpyKind` que especifica el tipo de transferencia de memoria. Puede ser uno de los siguientes:
    - `cudaMemcpyHostToHost`: Copia de memoria del host al host.
    - `cudaMemcpyHostToDevice`: Copia de memoria del host al dispositivo.
    - `cudaMemcpyDeviceToHost`: Copia de memoria del dispositivo al host.
    - `cudaMemcpyDeviceToDevice`: Copia de memoria del dispositivo al dispositivo.

Por lo tanto, en tu código, `cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);` está copiando `size` bytes de memoria desde la matriz `A` en el host a la matriz `d_A` en el dispositivo. 

# Debugging
En medio de mi codigo experimente un SegmentationFault, inicialmente no enenid por qu era, asi que utilize `gdb`

Como un tutorial, haz de compilar con simbolos de deubbing con la flag `-g`
```bash
gcc -g 1_confirmation.c -o 1.x
```

Luego a la hora de iniciar el debugging
```bash
gdb 1.x

(gdb) break 11 # para hacer break en esa linea
(gdb) run # corre el programa hasta el break

(gdb) continua # usa continua para pasar al siguiente paso del programa
```

Aqui un ejemplo de como se ve una sesion de `gdb`:

```bash
(gdb) break 11
Breakpoint 1 at 0x4005c8: file 1_confirmation.c, line 11.
(gdb) break 20
Breakpoint 2 at 0x40062e: file 1_confirmation.c, line 20.
(gdb) break 30
Breakpoint 3 at 0x400649: file 1_confirmation.c, line 30.
(gdb) run
Starting program: /home/ftrigo/c_coding/1106_tarea1/1.x 

Breakpoint 1, main () at 1_confirmation.c:15
15          for(i = 0; i < SIZE; i++) {
Missing separate debuginfos, use: debuginfo-install glibc-2.17-317.el7.x86_64
(gdb) continue
Continuing.

Program received signal SIGSEGV, Segmentation fault.
0x0000000000400609 in main () at 1_confirmation.c:18
18                  B[i][j] = 2;
(gdb) quit
A debugging session is active.
```

Para ello descrubri que el problema se relacionaba a la pila y el heap

En el código original, declare las matrices A, B y C como arrays bidimensionales de tamaño fijo en la función `main`. Esto significa que se asignaron en la **pila**:

```c
int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
```

En la versión modificada del código, las matrices se asignan en el **heap** utilizando `malloc`:

```c
A = (int **)malloc(SIZE * sizeof(int *));
for(i = 0; i < SIZE; i++) {
    A[i] = (int *)malloc(SIZE * sizeof(int));
}
```

La **pila** y el **heap** son dos áreas de la memoria en tiempo de ejecución de un programa:

- La **pila** es una región de memoria que crece y se contrae automáticamente cuando una función es llamada y retorna, respectivamente. Es rápida de asignar y desasignar, pero su tamaño es limitado.

- El **heap** es una región de memoria que debe ser gestionada manualmente. Es más lenta para asignar y desasignar, pero su tamaño es mucho mayor.

Cuando intentas asignar una gran cantidad de memoria (como una matriz de 1024x1024) en la pila, puedes superar su límite y causar un error de segmentación. Al asignar la memoria en el heap, puedes evitar este problema porque el heap tiene un tamaño mucho mayor.

### Ejemplo

1. **Usando `int **A;` y `malloc`**:

    ```c
    int **A;
    A = (int **)malloc(SIZE * sizeof(int *));
    for(i = 0; i < SIZE; i++) {
        A[i] = (int *)malloc(SIZE * sizeof(int));
    }
    ```

    En este código, estás creando una matriz bidimensional de enteros en el heap.

    - `int **A;`: Aquí, `A` es un puntero a puntero de entero. Es decir, `A` puede contener la dirección de otro puntero que apunta a un entero.

    - `A = (int **)malloc(SIZE * sizeof(int *));`: Aquí, estás asignando memoria en el heap para `SIZE` número de punteros a enteros. `malloc` devuelve un puntero a esta memoria, que se almacena en `A`. Ahora, `A` es un array de punteros.

    - `for(i = 0; i < SIZE; i++) { A[i] = (int *)malloc(SIZE * sizeof(int)); }`: Aquí, para cada puntero en el array `A`, estás asignando memoria en el heap para `SIZE` número de enteros. Cada `A[i]` ahora apunta a un array de enteros. En conjunto, `A` ahora apunta a una matriz bidimensional de enteros.

2. **Usando `int* A[SIZE][SIZE];`**:

    ```c
    int* A[SIZE][SIZE];
    ```

    En este código, estás creando una matriz bidimensional de punteros a enteros en la pila.

    - `int* A[SIZE][SIZE];`: Aquí, `A` es una matriz bidimensional de punteros a enteros. Es decir, cada elemento de `A` puede contener la dirección de un entero.

La principal diferencia entre estos dos códigos es dónde se almacena la matriz:

- Con `int **A;` y `malloc`, la matriz se almacena en el heap, que tiene un tamaño mucho mayor pero debe ser gestionado manualmente.

- Con `int* A[SIZE][SIZE];`, la matriz se almacena en la pila, que tiene un tamaño limitado pero se gestiona automáticamente.
