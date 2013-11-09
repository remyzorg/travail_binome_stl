

all:
	mpicc -o jacobi jacobi.c && mpirun -n 2 ./jacobi 10000

seq:
	gcc -o jacobi_seq jacobi_seq.c && ./jacobi_seq 10000

run:
	mpirun -n 15 -hostfile hostfile ./jacobi
