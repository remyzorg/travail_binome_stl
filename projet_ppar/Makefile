
all:
	mpicc -O3 -o jacobi_ring jacobi_ring.c

exec:
	mpirun -n 10 -hostfile hostfile ./jacobi 67000




both:
	mpicc -o jacobi jacobi.c && mpirun -n 2 ./jacobi 10000

ring:
	mpicc -O3 -o jacobi_ring jacobi_ring.c

mp:
	mpicc -O3 -fopenmp -o jacobi_mp jacobi_mp.c

seq:
	gcc -o jacobi_seq jacobi_seq.c && ./jacobi_seq 3001

