/* Implementation of a Jacobi iteration 

   Sequential version

   Copyright (C) 2013 Université Pierre et Marie Curie Paris 6.
   
   Contributor: Christoph Lauter

   All rights reserved.

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>
#include <string.h>

/* Some constants */
#define DEFAULT_PROBLEM_SIZE  1024
#define JACOBI_EPS            1e-13
#define JACOBI_MAX_ITER       100

#define JACOBI_MAX_RESIDUAL   1e-7
#define DISPLAY_FIRST_ENTRIES 10


void printVector(double* vector, int size) {
  int i;

  for(i = 0; i < size; i++) {
    printf("%1.2e ", vector[i]);
  }
  printf("\n");
}

void printMatrix(double* vector, int w, int h) {
  int i, j;

  for(i = 0; i < h; i++) {
    for(j = 0; j < w; j++) {
    printf("%1.2e ", vector[i * w + j]);
    }
    printf("\n");
  }
}

/* Compute time differences in seconds */
double computeTimeDifferenceInSeconds(struct timeval *before, struct timeval *after) {
  double secs, usecs;

  secs = ((double) after->tv_sec) - ((double) before->tv_sec);
  usecs = ((double) after->tv_usec) - ((double) before->tv_usec);
  
  return secs + (usecs / 1000000.0);
}

/* Generate a random double precision number between -absMax and
   absMax 
*/
double randomDoubleInDomain(double absMax) {
  double res;

  res = absMax * ((double) random()) / ((double) RAND_MAX);

  if (random() & 1) res = -res;

  return res;
}

/* Generate a matrix of size n * n that is certainly diagonally
   dominant 
*/
void generateRandomDiagonallyDominantMatrix(double *A, int w, int h, int rank) {
  int i, j;
  double absMax;

  absMax = 0.5 / ((double) w);

  for (i=0; i < h; i++) {
    for (j=0; j < w; j++) {
      A[i * w + j] = absMax;//randomDoubleInDomain(absMax);
    }
    A[i * w + i + h * rank] = 3.0; //+ randomDoubleInDomain(1.0);
  }
}



/* Generate a random vector of size n */
void generateRandomVector(double *v, int n, int rank) {
  int i;

  for (i=0;i<n;i++) {
    v[i] = (i + rank * n) % 1024;//randomDoubleInDomain(1024.0);
  }
}

/* Compute the residual b - A * x for a n * n matrix A, a n sized
   right-hand side b and a n sized solution x, using no extra
   precision. 
*/
void computeResidual(double *r, double *A, double *x, double *b,
		     int w, int h, int rank) {
  int i, j;
  double c;

  for (i = 0; i < h; i++) {
    c = b[i];
    for (j = 0; j < w; j++) {
      c -= A[i * w + j] * x[j];
    }
    r[i] = c;
  }
}

/* Get the maximum magnitude of the entries in an n sized vector */
double maxAbsVector(double *v, int n) {
  int i;
  double c, res;

  res = 0.0;
  for (i=0;i<n;i++) {
    c = fabs(v[i]);
    if (c > res) res = c;
  }

  return res;
}

/* Perform Jacobi iteration on a n * n sized, diagonally dominant
   matrix A and a n sized right-hand size b.

   Stop the iteration when the difference between two consecutive solutions 
   becomes less than eps in magnitude in each line. 

   Stop in any case after maxIter iterations.

   Put the n sized solution in the vector x, which is supposed to be
   allocated.

   Do not change the system matrix A nor the right-hand side b.

   Uses a scratch vector xp of the same size as x.

   Returns one of the pointers x or xp to indicate in which one of 
   both the final result has been stored.

*/
double *jacobiIteration(double *x, double *xp, double *A, double *b,
			double eps, int n, int maxIter, int hlocal, int rank,
                        int proc_num, double* my_prev) { 
  int i, j, convergence, iter, local_convergence; 
  double c, d, delta;
  double *xNew, *xPrev, *myPrev, *xt;

  for (i=0;i<n;i++) {
    x[i] = 1.0;
  }

  myPrev = my_prev;

  xPrev = x;
  xNew = xp;
  iter = 0;
  MPI_Status* status = NULL;
  int source = 0;
  int prev = (rank - 1) < 0 ? proc_num - 1 : rank - 1;
  int next = (rank + 1) >= proc_num ? 0 : rank + 1;
  int k;
  int actual_j;
  double diagonale;
  int comm = 0;

  do {
    iter++;
    delta = 0.0;

    for(k = 0; k < proc_num; k++) {
      source = (k + rank) % proc_num;

      if(!local_convergence) {
        #pragma omp parallel for default(shared) private(c, d) 
        for (i = 0; i < hlocal; i++) {
          diagonale = A[i * n + i + rank * hlocal]; 

          if(k == 0) {
            c = b[i];
          } else {
            c = xNew[i];
          }
        


          for (j = 0; j < hlocal; j++) {
            actual_j = j + source * hlocal;
            if (actual_j != (i + rank * hlocal)) { // si pas diagonale
              c -= A[i * n + actual_j] * xPrev[j];
            }
          } // for j


          if(k == proc_num - 1) {
            c /= A[i * n + i + rank * hlocal]; // division par diagonale
    
            d = fabs(myPrev[i] - c);
            if (d > delta) delta = d;
            local_convergence = (delta < eps);

          }
        
          xNew[i] = c;

        } // for i
      }


      if(iter != 0 && k != proc_num - 1) {

      
        comm++;
        MPI_Sendrecv(
              xPrev, hlocal, MPI_DOUBLE, next, 0,
              xPrev, hlocal, MPI_DOUBLE, prev, 0,
              MPI_COMM_WORLD, status);

      } // if first iter

    } //for k



    xt = xPrev;

    // sémantique :
    xPrev = xNew;
    memcpy(myPrev, xPrev, hlocal * sizeof(double));
    
    
    //printf("%d/%d xPrev : ", rank, iter);
    //printVector(xPrev, hlocal);

    //printf("comm : %d\n", comm); 
    xNew = xt;
    convergence = local_convergence;
    
    for(k = 0; k < proc_num; k++) {
        MPI_Sendrecv(
            &convergence, 1, MPI_INT, next, 0,
            &local_convergence, 1, MPI_INT, prev, 0,
            MPI_COMM_WORLD, status);

        convergence = local_convergence * convergence;
    } 
  } while ((!convergence) && (iter < maxIter));

  return xPrev;
}

/* A small testing main program */
int main(int argc, char *argv[]) {
  int i, n;
  double *A, *b, *x, *r, *xA, *xB, *result, *my_prev;
  double maxAbsRes;
  struct timeval before, after;

  MPI_Init( &argc, &argv );
  int rank;
  int numproc;
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &numproc );


  
  /* Get the argument that indicates the problem size */
  if(argc > 1) {
    n = atoi(argv[1]);
  } else {
    n = DEFAULT_PROBLEM_SIZE;
    /* fprintf(stderr, "Using default problem size n = %d\n",n); */
  }

  int hlocal = n / numproc;

  /* Initialize the random seed */
  srandom((unsigned int) time(NULL));

  /* Allocate memory */
  if ((A = (double *) calloc(hlocal * n, sizeof(double))) == NULL) {
    fprintf(stderr, "Not enough memory.\n");
    return 1;
  }

  if ((b = (double *) calloc(hlocal, sizeof(double))) == NULL) {
    free(A);
    fprintf(stderr, "Not enough memory.\n");
    return 1;
  }
  
  if ((xA = (double *) calloc(n, sizeof(double))) == NULL) {
    free(A);
    free(b);
    fprintf(stderr, "Not enough memory.\n");
    return 1;
  }

  if ((xB = (double *) calloc(n, sizeof(double))) == NULL) {
    free(A);
    free(b);
    free(xA);
    fprintf(stderr, "Not enough memory.\n");
    return 1;
  }

  if ((r = (double *) calloc(n, sizeof(double))) == NULL) {
    free(A);
    free(b);
    free(xA);
    free(xB);
    fprintf(stderr, "Not enough memory.\n");
    return 1;
  }

  if ((result = (double *) calloc(n, sizeof(double))) == NULL) {
    free(A);
    free(b);
    free(xA);
    free(xB);
    free(r);
    fprintf(stderr, "Not enough memory.\n");
    return 1;
  }

  if ((my_prev = (double *) calloc(hlocal, sizeof(double))) == NULL) {
    free(A);
    free(b);
    free(xA);
    free(xB);
    free(r);
    free(result);
    fprintf(stderr, "Not enough memory.\n");
    return 1;
  }


  /* Generate a random diagonally dominant matrix A and a random
     right-hand side b 
  */
  generateRandomDiagonallyDominantMatrix(A, n, hlocal, rank);
  generateRandomVector(b, hlocal, rank);
  
  /* Perform Jacobi iteration 

     Time this (interesting) part of the code.

  */
  gettimeofday(&before, NULL);
  x = jacobiIteration(xA, xB, A, b, JACOBI_EPS, n,
 		      JACOBI_MAX_ITER, hlocal, rank, numproc, my_prev);
  gettimeofday(&after, NULL);


  MPI_Allgather(x, hlocal, MPI_DOUBLE, result, hlocal, MPI_DOUBLE,
		MPI_COMM_WORLD);

  /* Compute the residual */
  computeResidual(r, A, result, b, n, hlocal, rank);

//  printVector(r, n);
  MPI_Allgather(r, hlocal, MPI_DOUBLE, result, hlocal, MPI_DOUBLE,
		MPI_COMM_WORLD);

  /* Compute the maximum absolute value of the residual */
  maxAbsRes = maxAbsVector(result, n);
  
  /* Display maximum absolute value of residual and a couple of
     entries of the solution vector and corresponding residual 
  */
  printf("Maximum absolute value of residual: %1.8e\n", maxAbsRes);
  /* printf("\n"); */
  /* for (i=0;i<DISPLAY_FIRST_ENTRIES;i++) { */
  /*   printf("%1.8e\t%1.8e\n", x[i], r[i]); */
  /* } */
  printf("\n");

  /* Decide if residual is small enough */
  if (maxAbsRes <= JACOBI_MAX_RESIDUAL) {
    printf("Solution OKAY\n");
  } else {
    printf("Solution NOT okay\n");
  }

  /* Display time for the Jacobi iteration */
  printf("Computing the solution with Jacobi iteration took %12.6fs\n", computeTimeDifferenceInSeconds(&before, &after));

  /* /\* Free the memory *\/ */
  free(A);
  free(b);
  free(xA);
  free(xB);
  free(r);
  free(result);

  MPI_Finalize();

  /* Return success */
  return 0;
}
