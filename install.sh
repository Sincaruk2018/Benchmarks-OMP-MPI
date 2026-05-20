#!/bin/bash
set -e

cd Mult_matriz

gcc -O3 -Wall -Wextra GeraMatrix.c -o geramatrix.out

g++ -O3 -Wall -Wextra mult_matrizes_seq.cpp -o mult_matriz_seq.out

g++ -O3 -Wall -Wextra -fopenmp \
mult_matrizes_omp.cpp \
-o mult_matriz_omp.out

mpic++ -O3 -Wall -Wextra \
mult_matrizes_MPI.cpp \
-o mult_matriz_mpi.out

cd ..

cd Crivo

g++ -O3 -Wall -Wextra \
Crivo_Seq.cpp \
-o crivo_seq.out

g++ -O3 -Wall -Wextra -fopenmp \
Crivo_OMP.cpp \
-o crivo_omp.out

mpic++ -O3 -Wall -Wextra \
Crivo_MPI.cpp \
-o crivo_mpi.out

cd ..

cd Fibonacci

g++ -O3 -Wall -Wextra \
fibonacci_seq.cpp \
-o fibonacci_seq.out

g++ -O3 -Wall -Wextra -fopenmp \
fibonacci_omp.cpp \
-o fibonacci_omp.out

cd ..
