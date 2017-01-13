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

/* Verifiy if a local array is sorted */
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
  disp[0] = 0;
  for(j = 0; j < len; j++){
    if(data[j] >= splitters[i]){
      disp[i+1] = j;
      if(i < num_sp) i++;
      else break;
    }
  }
}

/**
 * Verify that all data is sorted globally
 */
int verify_results_eff( int* arr, int len, int myrank, int nprocs ) {
    int sorted = 1;
    int max;
    MPI_Status status;
    sorted = is_arr_sorted(arr, len);
    if(sorted == 0) return 0;
    else if(myrank == 0){
      MPI_Send(&arr[len], 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
      MPI_Send(&sorted, 1, MPI_INT, 1, sorted, MPI_COMM_WORLD);
    }
    else if(myrank != nprocs-1 && sorted){
      MPI_Recv(&max, 1, MPI_INT, myrank -1, myrank, MPI_COMM_WORLD, &status);
      MPI_Recv(&sorted, 1, MPI_INT, myrank -1, sorted, MPI_COMM_WORLD, &status);
      if(arr[0] <= max){
        sorted = 0;
      }
      MPI_Send(&arr[len], 1, MPI_INT, myrank+1, myrank+1, MPI_COMM_WORLD);
      MPI_Send(&sorted, 1, MPI_INT, myrank+1, sorted, MPI_COMM_WORLD);
    }
    else if (myrank == nprocs-1){
      MPI_Recv(&max, 1, MPI_INT, myrank -1, myrank, MPI_COMM_WORLD, &status);
      MPI_Recv(&sorted, 1, MPI_INT, myrank -1, sorted, MPI_COMM_WORLD, &status);
    }
    MPI_Bcast(&sorted, 1, MPI_INT, myrank, MPI_COMM_WORLD);
    return sorted;
}

/* compare function fpr qsort() */
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
      if(myrank == 0){ //Otherwise ther would be a deadlock, if all threads would wait to receive. So one have to send first: Thread 0
        int block_size;
        block_size = split_index[l] - split_index[l-1]; //local_glob_split_index[l] - local_glob_split_index[l-1] = the size of the splitter block l
        MPI_Send(&block_size, 1, MPI_INT, l, l, MPI_COMM_WORLD);
        MPI_Recv(&recv_block_size[l], 1, MPI_INT, l, myrank, MPI_COMM_WORLD, &status);
      }
      else{
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
      if(myrank == 0){ //One thread has to send first and receive after that. Otherwise there would be a deadlock.
        int send_count;
        send_count = split_index[1] - split_index[n-1];
        MPI_Send(&orig_arr[split_index[n]], send_count , MPI_INT, n, n, MPI_COMM_WORLD);
        MPI_Recv(&my_array[pointer_recv], recv_block_size[n], MPI_INT, n, myrank, MPI_COMM_WORLD, &status);
      }
      else{
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
void par_sort( int** orig_arr, int* orig_len, int myrank, int nprocs ){
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
  //MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(splitters, nprocs-1, MPI_INT, cum_split, nprocs-1, MPI_INT, 0, MPI_COMM_WORLD); //Get all splitters from all processes

  /* Select global splitters */
  int *glob_split = malloc((nprocs -1)*sizeof(int));
  if(myrank == 0){
   global_split_elements(glob_split, nprocs, cum_split);
  }
  MPI_Bcast(glob_split, nprocs - 1, MPI_INT, 0, MPI_COMM_WORLD);

  /* Redistribute parts */
  // Call partition()
  int *local_glob_split_index = malloc((nprocs)*sizeof(int));
  partition(*orig_arr, *orig_len, glob_split, nprocs-1, local_glob_split_index);

  //Generate an array to use Alltoall and alltoallv
  //recv_block_size stores how many integers this process will receive from each other process
  //For example: From process i this process will receive recv_block_size[i] integers
  int *recv_block_size = malloc(nprocs * sizeof(int));
  //send_block_size stores how many integers this process will send to all the other processes
  //For example this process will send send_block_size[i] integers to process i
  int *send_block_size = malloc(nprocs * sizeof(int));
  int l;
  for(l=0; l < nprocs; l++){
    if(l == nprocs -1){
      send_block_size[l] = *orig_len - local_glob_split_index[l];
    }
    else
    send_block_size[l] = local_glob_split_index[l+1] - local_glob_split_index[l]; //local_glob_split_index[l] - local_glob_split_index[l-1] = the size of the splitter block l
  }
  // Tell each process how many elments I send to it
  MPI_Alltoall(send_block_size, 1, MPI_INT, recv_block_size, 1, MPI_INT, MPI_COMM_WORLD);
  //sizeof_receiving_blocks(recv_block_size, myrank, local_glob_split_index, nprocs);

  //Calculate the needed space by adding the block size this process will receive from all the other processes.
  int needed_space = 0;
  int m;
  for(m = 0; m < nprocs; m++){
    needed_space += recv_block_size[m];
  }
  // Allocate array of proper size
  int *my_array = malloc(needed_space*sizeof(int));
  // Send one block to each process (blocks could also be empty) and receive blocks
  int *recv_split_index = malloc(nprocs*sizeof(int));
  recv_split_index[0] = 0;
  int k;
  for(k=0; k+1 < nprocs; k++){
    recv_split_index[k+1] = recv_split_index[k] + recv_block_size[k];
  }

  fflush(stdout);
  //Send block[i] from my local orig-arr to process i. block[i] is send_block_size[i] long and begins at index local_glob_split_index[i]
  //Receive from all other processes a block
  MPI_Alltoallv(*orig_arr, send_block_size, local_glob_split_index, MPI_INT, my_array, recv_block_size, recv_split_index, MPI_INT, MPI_COMM_WORLD);
  //send_and_receive_blocks(*orig_arr, my_array, myrank, local_glob_split_index, recv_block_size, nprocs);

  /* Sort locally */
  qsort(my_array, needed_space, sizeof(int), cmp_qsort);

  free(*orig_arr);
  *orig_arr = my_array; //Redirect pointer *orig_arr to my_array
  *orig_len = needed_space; //update array length

  free(splitters);
  free(local_glob_split_index);
  free(glob_split);
  free(send_block_size);
  free(recv_block_size);
  free(recv_split_index);
  if(myrank == 0) free(cum_split);
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
    for( i = 0; i < local_len; ++i ){
        local_arr[i] = rand();
      }

    double start = get_clock_time();

    /* Parallel sort */
  //  printf("par sort, adress local_arr %p,local_len: %i, nprocs: %i, myrank: %i \n", local_arr, local_len, nprocs, myrank);
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
