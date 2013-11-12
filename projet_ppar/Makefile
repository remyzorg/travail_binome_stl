
all:
	mpicc -o jacobi jacobi.c

both:
	mpicc -o jacobi jacobi.c && mpirun -n 2 ./jacobi 3001

ring:
	mpicc -o jacobi_ring jacobi_ring.c && mpirun -n 4 ./jacobi_ring 400

seq:
	gcc -o jacobi_seq jacobi_seq.c && ./jacobi_seq 3001

exec:
	mpirun -n 15 -hostfile hostfile ./jacobi
