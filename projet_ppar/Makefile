
all:
	mpicc -o jacobi jacobi.c

both:
	mpicc -o jacobi jacobi.c && mpirun -n 2 ./jacobi 3001

seq:
	gcc -o jacobi_seq jacobi_seq.c && ./jacobi_seq 3001

exec:
	mpirun -n 15 -hostfile hostfile ./jacobi