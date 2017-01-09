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
  int i = 0;
  for(i = 0; i < num_sp; i++){
    while (data[j] < splitters[i] && j < len){
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

/* compar function fpr qsort() */
int cmp_qsort (const void * a, const void * b){
   return ( *(int*)a - *(int*)b );
}

/* function to select global splitter elements*/
void global_split_elements(int* glob_split, int nprocs, int* all_splits){
  int n = nprocs - 1;
  qsort(all_splits, nprocs*(nprocs -1), sizeof(int), cmp_qsort);
  int k = 0;
  for(k=0; k <= nprocs -2; k++){
    int m = n *(k+1);
    glob_split[k] = all_splits[m];
  }
}

/*How many ints will one process receive from all the other? */
void sizeof_receiving_blocks(int* recv_block_size, int myrank, int* split_index, int nprocs){
  int l = 0;
  MPI_Status status;
  recv_block_size[myrank] = 0;
  int needed_space = 0;
  for(l=0; l < nprocs; l++){
    if(myrank != l){
      MPI_Recv(&recv_block_size[l], 1, MPI_INT, l, myrank, MPI_COMM_WORLD, &status);
      if(l == 0){
        int block_size;
	      block_size = split_index[l];
        MPI_Send(&block_size, 1, MPI_INT, l, l, MPI_COMM_WORLD);
      }
      else{
        int block_size;
	      block_size = split_index[l] - split_index[l-1]; //local_glob_split_index[l] - local_glob_split_index[l-1] = the size of the splitter block l
        MPI_Send(&block_size, 1, MPI_INT, l, l, MPI_COMM_WORLD);
	}
    }
    needed_space = needed_space + recv_block_size[l];
  };
  recv_block_size[l] = needed_space;
}

/* Send one block to each process (blocks could also be empty) and receive blocks from the other process */
void send_and_receive_blocks(int* orig_arr, int* my_array, int myrank, int* split_index, int* recv_block_size, int nprocs){
  int n = 0;
  MPI_Status status;
  int pointer_recv = 0;
  for(n=0; n < nprocs; n++){
    if(myrank != n){
      MPI_Recv(&my_array[pointer_recv], recv_block_size[n], MPI_INT, n, myrank, MPI_COMM_WORLD, &status);
      if(n == 0){
        MPI_Send(&orig_arr[n], split_index[n] , MPI_INT, n, n, MPI_COMM_WORLD);
      }
      else{
        int send_count;
	send_count = split_index[1] - split_index[n-1];
        MPI_Send(&orig_arr[split_index[n]], send_count , MPI_INT, n, n, MPI_COMM_WORLD);
	}
    }
    pointer_recv += recv_block_size[n];
  };
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
 printf("in par_sort");
  //int n = *orig_len/nprocs;
	//MPI_Scatter(*orig_arr, n, MPI_INT, *orig_arr, n, MPI_INT, 0, MPI_COMM_WORLD); // Verteile an alle Prozesse Teile der Größe n des zu sortierenden Arrays
  /* Sort locally */
  qsort(*orig_arr, *orig_len, sizeof(int), cmp_qsort);
  /* Select local splitters */
  int i = 0;
  int *splitters = malloc((nprocs-1)*sizeof(int));
  for(i=0; i <= nprocs-2; i++){
    int j = (*orig_len)*(i + 1)/nprocs;
    splitters[i] = (*orig_arr)[j];
  };

  /* Gather all the splitters on the root process */
  int *cum_split;
  if(myrank == 0){
   cum_split = malloc(nprocs*(nprocs -1)*sizeof(int));
  };
  MPI_Barrier(MPI_COMM_WORLD); //Braucht man das, damit bei Aufruf von Gather alle lokalen splitter berechnet worden sind?
	MPI_Gather(splitters, nprocs-1, MPI_INT, cum_split, nprocs-1, MPI_INT, 0, MPI_COMM_WORLD); //Erhalte von allen Prozessen die p-1 Splitter
  /* Select global splitters */
  int *glob_split = malloc((nprocs -1)*sizeof(int));
  if(myrank == 0){
   global_split_elements(glob_split, nprocs, cum_split);
  }
  MPI_Bcast(glob_split, nprocs - 1, MPI_INT, 0, MPI_COMM_WORLD);

  /* Redistribute parts */
  // Call partition()
  int *local_glob_split_index = malloc((nprocs -1)*sizeof(int));
  partition(*orig_arr, *orig_len, glob_split, nprocs-1, local_glob_split_index);

  // Tell each process how many elments I send to it
  int recv_block_size[nprocs+1];
  sizeof_receiving_blocks(recv_block_size, myrank, local_glob_split_index, nprocs);

  // Allocate array of proper size
  int *my_array = malloc(recv_block_size[nprocs]*sizeof(int));
  // Send one block to each process (blocks could also be empty) and receive blocks
  send_and_receive_blocks(*orig_arr, my_array, myrank, local_glob_split_index, recv_block_size, nprocs);

  /* Sort locally */
  qsort(my_array, recv_block_size[nprocs], sizeof(int), cmp_qsort);

//  **orig_arr = *my_array; //kopiere my_array in orig_arr
  free(splitters);
  free(local_glob_split_index);
  free(cum_split);
  free(glob_split);
  free(my_array);
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
      //  exit( 1 );
    }
    int i, local_len = atoi( argv[1] );
    int* local_arr = malloc( local_len * sizeof(int) );

    /* Randomize the input */
    srand( time(NULL) * myrank );
    for( i = 0; i < local_len; ++i )
        local_arr[i] = rand();

    double start = get_clock_time();

    /* Parallel sort */
    printf("par sort, adress local_arr %i, local_len: %i, nprocs: %i, myrank: %i \n", *local_arr, local_len, nprocs, myrank);
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
