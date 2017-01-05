#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * Merge arr1 of length n1 and arr2 of length n2 into
 * tmp_arr. This function expects array tmp_arr to be of 
 * length n1 + n2
 */
void merge_arr( int* arr1, int n1, int* arr2, int n2, 
                int* tmp_arr ) {

}


/**
 * Do the parallel sorting
 */
void par_sort( int** orig_arr, int* orig_len, int myrank, 
               int nprocs ) {

    /* Sort locally */


    /* Allocate temp arrays */


    /* The loop repeats log2(nprocs) times */
    // Use merge_arr()
    

    /* Free memory */
}

int is_arr_sorted( int* arr, int len ) {

    int i;
    for( i = 0; i < len - 1; ++i )
        if( arr[i] > arr[i+1] )
            return 0;
    return 1;
}

int main( int argc, char** argv ) {

    int myrank, nprocs;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
    MPI_Comm_size( MPI_COMM_WORLD, &nprocs );

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

    /* Parallel sort */
    par_sort( &local_arr, &local_len, myrank, nprocs );

    /* Verify the results */
    if( myrank == 0 ) {
        int res = is_arr_sorted( local_arr, local_len );
        printf( "Sorted: %d\n", res );
    }

    free( local_arr );

    MPI_Finalize();

    return 0;
}
