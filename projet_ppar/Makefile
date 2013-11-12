
all:
	mpicc -O3 -o jacobi jacobi.c

both:
	mpicc -o jacobi jacobi.c && mpirun -n 2 ./jacobi 10000

ring:
	mpicc -O3 -o jacobi_ring jacobi_ring.c

seq:
	gcc -o jacobi_seq jacobi_seq.c && ./jacobi_seq 3001

exec:
	mpirun -n 10 -hostfile hostfile ./jacobi 60000
