# Parallel Computing

This repository contains the solutions developed for the master's course Parallel Computational Systems. The projects involve using a variety of libraries (OpenMP, Pthreads, MPI and CUDA) and techniques to parallelize tasks and compare the performance of parallel algorithms with their serial counterparts. The code is structured to demonstrate the application of key parallel computing concepts and techniques covered throughout the course.

## Project 1

### Question 1
The goal of this question was to calculate an approximation of Pi using the Monte Carlo method and compare performance of the parallel algorithm to the serial one. Files: er1_openmp.c, er1_pthreads.c and er1_serial.c.

### Question 2
The goal of this question was to explore and resolve the false sharing problem encountered during matrix multiplication. The main implementation (pth_mat_vect_rand_split.c) demonstrates this issue, while two different methods are applied to address it:

1. Lock-Step Method: Implemented in er2_1.c, this approach synchronizes threads in a step-by-step manner to avoid false sharing.

2. Local Variable and Outer Lock Method: Implemented in er2_2.c, this technique uses a local variable and an outer lock to minimize shared memory contention.

### Question 3
The goal of this question was was to modify the file omp_mat_vect_rand_split.c, which calculates the product of an upper triangular matrix with a square matrix, and to explore the scheduling capabilities of OpenMP. To achieve this, two different scheduling methods were implemented:
1. Guided Scheduling: Implemented in er3_guided.c, this method uses dynamic scheduling with a decreasing chunk size to balance the workload among threads.

2. Static Scheduling: Implemented in er3_static.c, this approach divides the iterations into fixed-size chunks assigned to threads at compile time.

### Question 4
The goal of this question was to solve an upper triangular system of linear equations using backwards substitution (file er4.c).

---

## Project 2

### Question 1
The goal of this question was to calculate an approximation of Pi using the Monte Carlo method and compare performance of the parallel algorithm to the serial one. Files: monte_carlo_mpi.c and monte_carlo_serial.c.

### Question 2
The goal of this question was to parallelize the multiplication of square the matrix with a vector using the MPI library, and compare the results of the parallel implementation (mat_vec_mpi.c) to the linear one (mat_vec_serial.c). 

### Question 3
The goal of this project was to explore and resolve the race condition problem encountered while updating a variable in a multithreaded environment. The project implements and compares three different methods to address this issue:
1. Atomic Operations: Implemented in counter_pthreads_atomics.c, this method uses atomic operations to ensure that variable updates are thread-safe without the need for locks, preventing race conditions.
   
2. Locks: Implemented in counter_pthreads_locks.c, this approach employs mutex locks to manage access to the shared variable, ensuring that only one thread can update the variable at a time.
  
3. Sequential Execution: Implemented in counter_sequential.c, this method provides a baseline for comparison by updating the variable sequentially, without any parallelism or synchronization mechanisms.

### Question 4
The goal of this question was to parallelize the multiplication of two matrixes using CUDA.
