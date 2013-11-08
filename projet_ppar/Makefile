

all:
	mpicc -o jacobi jacobi.c && mpirun -n 2 ./jacobi 4000

run:
	mpirun -n 15 -hostfile hostfile ./jacobi
