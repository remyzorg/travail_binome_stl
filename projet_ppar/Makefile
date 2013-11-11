
all:
	mpicc -o jacobi jacobi.c

both:
	mpicc -o jacobi jacobi.c && mpirun -n 2 ./jacobi 3000

ring:
	mpicc -o jacobi_ring jacobi_ring.c && mpirun -n 2 ./jacobi_ring 4

seq:
	gcc -o jacobi_seq jacobi_seq.c && ./jacobi_seq 3000

exec:
	mpirun -n 15 -hostfile hostfile ./jacobi
