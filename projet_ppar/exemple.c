/**
 *  jacobi.c - a Jacobi iteractive solver
 *           - an MPI example
 *  by Walfredo Cirne (walfredo@cs.ucsd.edu)
 */



#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define TRUE 1
#define FALSE 0

#define BORDER (-1)
#define CONVERGENCE 0.01
#define HALF_LOOP 25
#define MSGTYPE 1
#define MAX_STRING 200



void jacobi( double* mat, int lines, int cols, int north, int south, int east, int west );
void output( double* mat, int lines, int cols );
void commonerr( char* msg );
void indiverr( char* msg );
void swapp2d( double** a, double** b );
void MPI_RecvOK( void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm );
  

void main( int argc, char** argv )
{
	int myid;
	int numproc;
	int lines;
	int cols;
	int proc_x;
	int proc_y;
	int x;
	int y;
	int north;
	int south;
	int east;
	int west;
	int plines;	/* private lines */
	int pcols;	/* private columns */
	double* mat;
	int i;
	int j;

	MPI_Init( &argc, &argv );

	MPI_Comm_rank( MPI_COMM_WORLD, &myid );
	MPI_Comm_size( MPI_COMM_WORLD, &numproc );

	if( argc != 5 ){
	    commonerr( "usage: jacobi lines cols x-procs y-procs" );
	}
	lines = atoi( argv[1] );
	cols = atoi( argv[2] );
	proc_x = atoi( argv[3] );
	proc_y = atoi( argv[4] );

	if( proc_x * proc_y != numproc ){
	    commonerr( "the number of processes doesn't match x-procs * y-procs" ); 
	}

	if( proc_x > lines || proc_y > cols ){
	    commonerr( "too many processes" );
	}

	x = myid % proc_x;
	y = myid / proc_x;

	if( y == 0 ){
	    north = BORDER;
	}
	else {
	    north = x + (y - 1) * proc_x;
	}
	if( y == proc_y - 1 ){
	    south = BORDER;
	}
	else {
	    south = x + (y + 1) * proc_x;
	}
	if( x == 0 ){
	    west = BORDER;
	}
	else {
	    west = myid - 1;
	}
	if( x == proc_x - 1 ){
	    east = BORDER;
	}
	else {
	    east = myid + 1;
	}

	plines = lines / proc_y;
	if( y < lines % proc_y ){
	    plines++;
	}
	pcols = cols / proc_x;
	if( x < cols % proc_x ){
	    pcols++;
	}

#	define XPLINES (plines + 2)
#	define XPCOLS (pcols + 2)
#  define MAT(i, j) *(mat + (i) * XPCOLS + (j))

	if( (mat = calloc( XPLINES * XPCOLS, sizeof( double ) )) == NULL ){
	    indiverr( "cannot alocate memory" );
	}

	for( i = 0; i < XPLINES; i++ ){
	    for( j = 0; j < XPCOLS; j++ ){
	      	MAT(i, j) = 50;
	    }
	}
	if( north == BORDER ){
	    for( j = 0; j < XPCOLS; j++ ){
	      	MAT(0, j) = 0;
	    }
	}
	if( south == BORDER ){
	    for( j = 0; j < XPCOLS; j++ ){
	      	MAT(XPLINES - 1, j) = 100;
	    }
	}
	if( east == BORDER ){
	    for( i = 0; i < XPLINES; i++ ){
	      	MAT(i, XPCOLS - 1) = 100;
	    }
	}
	if( west == BORDER ){
	    for( i = 0; i < XPLINES; i++ ){
	       	MAT(i, 0) = 100;
	    }
	}

	jacobi( mat, XPLINES, XPCOLS, north, south, east, west );

	output( mat, XPLINES, XPCOLS );

#	undef XPLINES
#	undef XPCOLS
#	undef MAT

	MPI_Finalize();
}


void jacobi( double* mat, int lines, int cols, int north, int south, int east, int west )
{
	double* rmat;
	double* awest;
	double* aeast;
	double ldiff;	/* local maximum difference */
	double gdiff;	/* global maximum difference */
	int i;
	int j;
	int l;
 	
	if( (rmat = calloc( lines * cols, sizeof( double ) )) == NULL ||
	    (awest = calloc( lines, sizeof( double ) )) == NULL ||
	    (aeast = calloc( lines, sizeof( double ) )) == NULL ){
	    indiverr( "cannot alocate memory" );
	}       

#  define MAT(i, j) *(mat + (i) * cols + (j))
#  define RMAT(i, j) *(rmat + (i) * cols + (j))

	if( north == BORDER ){
	    for( j = 0; j < cols; j++ ){
		 RMAT(0, j) = MAT(0, j);
	    }	       
	}		
	if( south == BORDER ){
	    for( j = 0; j < cols; j++ ){
		 RMAT(lines - 1, j) = MAT(lines - 1, j);
	    }
	}
	if( east == BORDER ){
	    for( i = 0; i < lines; i++ ){
		 RMAT(i, cols - 1) = MAT(i, cols - 1);
	    }
	}
	if( west == BORDER ){
	    for( i = 0; i < lines; i++ ){
		 RMAT(i, 0) = MAT(i, 0);
	    }
	}		       

	do {

	    for( l = 0; l < 2 * HALF_LOOP; l++ ){		

		for( i = 1; i < lines - 1; i++ ){
		     for( j = 1; j < cols - 1; j++ ){
			  RMAT(i, j) = (MAT(i - 1, j) + MAT(i + 1, j) + MAT(i, j - 1) + MAT(i, j + 1)) / 4;
		     }
		}
		swapp2d( &mat, &rmat );

		if( west != BORDER ){
		    for( i = 0; i < lines; i++ ){
		        *(awest + i) = MAT(i, 1);
		    }
		    MPI_Send( awest, lines, MPI_DOUBLE, west, MSGTYPE, MPI_COMM_WORLD );
	    	}
	    	if( east != BORDER ){
		    for( i = 0; i < lines; i++ ){ 
		    	*(aeast + i) = MAT(i, cols - 2);  
		    }
		    MPI_Send( aeast, lines, MPI_DOUBLE, east, MSGTYPE, MPI_COMM_WORLD );
	    	}
	    	if( north != BORDER ){
		    MPI_Send( mat + cols, cols, MPI_DOUBLE, north, MSGTYPE, MPI_COMM_WORLD );
	    	}
	    	if( south != BORDER ){
		    MPI_Send( mat + (lines - 2) * cols, cols, MPI_DOUBLE, south, MSGTYPE, MPI_COMM_WORLD );
	    	}       

	        if( west != BORDER ){
		    MPI_RecvOK( awest, lines, MPI_DOUBLE, west, MSGTYPE, MPI_COMM_WORLD );
		    for( i = 0; i < lines; i++ ){
		     	MAT(i, 0) = *(awest + i);
		    }
	    	}
	        if( east != BORDER ){
		    MPI_RecvOK( aeast, lines, MPI_DOUBLE, east, MSGTYPE, MPI_COMM_WORLD );
		    for( i = 0; i < lines; i++ ){
		    	MAT(i, cols - 1) = *(aeast + i);
		    }
	    	}
	    	if( north != BORDER ){
		    MPI_RecvOK( mat, cols, MPI_DOUBLE, north, MSGTYPE, MPI_COMM_WORLD );
	    	}  
	    	if( south != BORDER ){
		    MPI_RecvOK( mat + (lines - 1) * cols, cols, MPI_DOUBLE, south, MSGTYPE, MPI_COMM_WORLD );
	    	}

       }

	    ldiff = 0;
	    for( i = 1; i < lines - 1; i++ ){
	     	for( j = 1; j < cols - 1; j++ ){
		    if( ldiff < fabs( MAT(i, j) - RMAT(i, j) ) ){
			ldiff = fabs( MAT(i, j) - RMAT(i, j) );
		    }
		}
	    }
		
	    MPI_Reduce( &ldiff, &gdiff, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );
	    MPI_Bcast( &gdiff, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );

	} while( gdiff > CONVERGENCE );	

	#undef RMAT
	#undef MAT
}


void output( double* mat, int lines, int cols )
{
	char proc_name[MPI_MAX_PROCESSOR_NAME + 1];
	int len;
	int i;
	int j;
	FILE* fout;
	
	MPI_Get_processor_name( proc_name, &len );
	if( (fout = fopen( proc_name, "w" )) == NULL ){
		indiverr( "cannot open output file" );
	}

#  define MAT(i, j) *(mat + (i) * cols + (j))
	for( i = 0; i < lines; i++ ){
	    for( j = 0; j < cols; j++ ){
		fprintf( fout, "%6.2f ", MAT(i, j) );
	    }
	    fprintf( fout, "\n" );
	}
#	undef MAT

	fclose( fout );
}


void commonerr( char* msg )
{
	int myid;

	MPI_Comm_rank( MPI_COMM_WORLD, &myid );
	if( myid == 0 ){
	    fprintf( stderr, "common error: %s\n", msg );
	}
	fflush( stderr );
	MPI_Finalize();
	exit( 1 );
}


void indiverr( char* msg )
{
	char proc_name[MPI_MAX_PROCESSOR_NAME + 1];
	int len;

	MPI_Get_processor_name( proc_name, &len );
	fprintf( stderr, "error in process %s: %s\n", proc_name, msg );
	fflush( stderr );
	MPI_Finalize();
	exit( 1 );
}


void swapp2d( double** a, double** b )
{
	double* temp;

	temp = *a;
	*a = *b;
	*b = temp;
}


void MPI_RecvOK( void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm )
{
	int result;
	MPI_Status status;

	result = MPI_Recv( buf, count, datatype, source, tag,comm, &status );
	if( result != MPI_SUCCESS ){
	    indiverr( "communication error" );
	}
}
