CUDA Wave Equation Solver

This CUDA program solves the wave equation using GPU parallelism.
Kernel: updateWave

A CUDA kernel is used to update the wave equation for each element of the grid.
It calculates the new state of the wave based on neighboring elements.
Memory Management

cudaMallocManaged is used to allocate memory on the GPU for arrays un, un_p1, and un_m1.
Main Loop

The main loop iterates through time steps, updating the wave equation and periodically saving results to data files.
CUDA Execution Configuration

The updateWave kernel is executed with a grid of blocks and threads per block to parallelize the computation.
