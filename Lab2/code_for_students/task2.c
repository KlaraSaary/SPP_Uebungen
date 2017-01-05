#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "timing.h"


void print_arr( int* arr, int len ) {

    unsigned int i;
    for( i = 0; i < len; ++i )
        printf( "%d  ", arr[i] );
    printf("\n");
}

int is_arr_sorted( int* arr, int len ) {

    int i;
    for( i = 0; i < len - 1; ++i )
        if( arr[i] > arr[i+1] )
            return 0;
    return 1;
}

/**
 * Determine the index of the beginning of each block
 * based on the global splitters
 * int* data: local array with data
 * int len: length of local array
 * int* splitters: values of global splitter elements
 * int num_sp: number of global splitter elements
 * int* disp: index of new local splitter beginnigs
 */
void partition(int* data, int len, int* splitters, int num_sp, int* disp){
  //Iterieren über das array data bis len erreicht ist. Wenn data[j] == splitters[i], speicher den Index des gerade betrachteten Elements in disp.
  int j = 0;
  for(int i = 0; i < num_sp; i++){
    while (data[j] != splitters[i] && j < len){
        j++;
      }
    disp[i] = j;
    j++;
  }
}

/**
 * Verify that all data is sorted globally
 */
int verify_results_eff( int* arr, int len, int myrank, int nprocs ) {

    return 0;
}

int cmp_qsort (const void * a, const void * b){
   return ( *(int*)a - *(int*)b );
}
/**
 * Do the parallel sorting
 * orig_arr: data
 * orig_len: N
 * myrank: rank of processor
 * nprocs: number of processors
 *
 *
 * Note: The length of the local input array may change in this
 *       call
 */
void par_sort( int** orig_arr, int* orig_len, int myrank, int nprocs ){ //Wieso Doppel-Pointer auf orig_len, wenn doch &local_arr übergeben wird? Wie greife ich dann am besten darauf zu?

  int n = *orig_len/nproc;
	MPI_Scatter(orig_arr, n, MPI_INT, orig_arr, n, MPI_INT, 0, MPI_COMM_WORLD); // Verteile an alle Prozesse Teile der Größe n des zu sortierenden Arrays
  /* Sort locally */
  qsort(orig_arr, n, sizeof(int), cmp_qsort);
  /* Select local splitters */
  int i = 0;
  int *splitters = malloc((nprocs-1)*sizeof(int));
  for(i; i <= nprocs-2; i++){
    int j = n*(i + 1)/nprocs;
    splitters[i] = orig_arr[j];
  }

  /* Gather all the splitters on the root process */
  int *cum_split;
  if(myrank == 0){
   cum_split = malloc(nprocs*(nprocs -1)*sizeof(int));
  }
  MPI_Barrier(MPI_COMM_WORLD); //Braucht man das, damit bei Aufruf von Gather alle lokalen splitter berechnet worden sind?
	MPI_Gather(splitters, nprocs-1, MPI_INT, cum_split, nprocs-1, MPI_INT, 0, MPI_COMM_WORLD); //MPI_Recv(); //Erhalte von allen Prozessen die p-1 Splitter
  /* Select global splitters */
  int *glob_split = malloc((nprocs -1)*sizeof(int));
  if(myrank == 0){
   qsort(cum_split, nprocs*(nprocs -1), sizeof(int), cmp_qsort);
   int i = 0;
   for(i; i <= nprocs -2, i++){
     int j = (nprocs − 1) * (i + 1);
     glob_split[i] = cum_split[j];
   }
 }
  MPI_Bcast(glob_split, nprocs - 1, MPI_INT, 0, MPI_COMM_WORLD);

  /* Redistribute parts */
  // Call partition()
  int *local_glob_split_index = malloc((nprocs -1)*sizeof(int));
  partition(orig_arr, n, glob_split, nprocs-1, local_glob_split_index);
  // Tell each process how many elments I send to it
  // Allocate array of proper size
  // Send one block to each process (blocks could also be empty)

  /* Sort locally */
  

  free(splitters)
}

int main( int argc, char** argv ) {

    int myrank, nprocs;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
    MPI_Comm_size( MPI_COMM_WORLD, &nprocs );

    init_clock_time ();

    /* Init the local part of the array */
    if( argc < 2 ) {
        printf( "Usage: %s <local_len>\n", argv[0] );
        exit( 1 );
    }
    int i, local_len = atoi( argv[1] );
    int* local_arr = malloc( local_len * sizeof(int) );

    /* Randomize the input */
    srand( time(NULL) * myrank );
    for( i = 0; i < local_len; ++i )
        local_arr[i] = rand();

    double start = get_clock_time();

    /* Parallel sort */
    par_sort( &local_arr, &local_len, myrank, nprocs );

    double elapsed = get_clock_time() - start;

    /* Verify the results */
    int res = verify_results_eff( local_arr, local_len, myrank, nprocs );
    if( myrank == 0 )
        printf( "Sorted: %d\n", res );

    /* Get timing - max across all ranks */
    double elapsed_global;
    MPI_Reduce( &elapsed, &elapsed_global, 1,
                MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );
    if( myrank == 0 )
        printf( "Elapsed time (ms): %f\n", elapsed_global );

    free( local_arr );

    MPI_Finalize();

    return 0;
}
