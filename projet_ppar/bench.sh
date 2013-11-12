#!/bin/sh


gcc -o jacobi_seq jacobi_seq.c
mpicc -o jacobi jacobi.c
mpicc -o jacobi_ring jacobi_ring.c


./jacobi 512
mpirun -n 2 ./jacobi 512

./jacobi 512
mpirun -n 2 ./jacobi 512

./jacobi 1024
mpirun -n 2 ./jacobi 1024


mpicc -o jacobi jacobi.c && 
mpicc -o jacobi_ring jacobi_ring.c && mpirun -n 4 ./jacobi_ring 400
gcc -o jacobi_seq jacobi_seq.c && ./jacobi_seq 3001
mpirun -n 10 -hostfile hostfile ./jacobi 60000