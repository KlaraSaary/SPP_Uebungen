#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/**
 * Merge arr1 of length n1 and arr2 of length n2 into
 * tmp_arr. This function expects array tmp_arr to be of 
 * length n1 + n2
 */
int compare (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

void print(int num,int* arr, int local_len){
	int o;
	printf("process %d: [",num);
	for(o=0;o<local_len;o++){
		printf("%d ",arr[o]);
	}
	printf("] %d\n",o);
}
void merge_arr( int* arr1, int n1, int* arr2, int n2, 
                int* tmp_arr ) {
	
	//print(13,arr2,n2);
	
	//printf("%d long array arr1 is",n1);
	
	int i = 0, j = 0, k = 0;
	//printf(" = ");
    	while (i<n1 && j<n2){
			//rintf("%d ",arr1[i]);
        	if (arr1[i] < arr2[j]){
	            tmp_arr[k] = arr1[i];
				//printf("case1 because %d is smaler as %d at %d and %d\n",arr1[i],arr2[j],i,j);
	            i++;
        	}
        	else{
	            tmp_arr[k] = arr2[j];
	            j++;
				//printf("case2\n");
        	}
	        k++;
    	}

	
    	while (i<n1){
        	tmp_arr[k] = arr1[i];
	        	i++;
	        	k++;
    	}

    	while (j <n2){
    	    	tmp_arr[k] = arr2[j];
    	    		j++;
    	    		k++;
    	}
	
	
	
}


/**
 * Do the parallel sorting
 */
void par_sort( int** orig_arr, int* orig_len, int myrank, 
               int nprocs ) {

	int height=log2(nprocs)+1;
	int z,t,q,position,d,i,layer;
	int* sendbuf = malloc(*orig_len*sizeof(int));
	int* sendbuf3 = malloc(nprocs*(*orig_len)*sizeof(int));
	
	
	int* solution = malloc(*orig_len*sizeof(int));
	int* proc_i=malloc(*orig_len*sizeof(int));
	int* proc_z=malloc(*orig_len*sizeof(int));
	layer=1;
	
	//sort and first merge
	
	
	if(myrank%2!=0){
			qsort(*orig_arr,*orig_len,sizeof(int),compare);
			sendbuf=*orig_arr;
			//print(myrank,sendbuf,*orig_len);
			MPI_Send(sendbuf,*orig_len,MPI_INT,myrank-1,myrank,MPI_COMM_WORLD);
	}
	else{
			qsort(*orig_arr,*orig_len,sizeof(int),compare);
			//print(myrank,sendbuf,*orig_len);
	}
			
	
	//now go through all layers and merge parralel
	
	
	int* sendbuf5 = malloc(nprocs*(*orig_len)*sizeof(int));
	
	t=1;
	while(t<height){
		if(myrank%(int)pow(2,t)==0){
			//printf("%d revc from %d; orig_arr: %d  snedbuf5: %d\n",myrank,myrank+(int)pow(2,(t-1)),*orig_len*(int)pow(2,t-1),*orig_len*(int)pow(2,t));
			int* sendbuf2 = malloc(2*(int)pow(2,t-1)*(*orig_len)*sizeof(int));
			MPI_Recv(sendbuf5,*orig_len*(int)pow(2,t-1),MPI_INT,myrank+(int)pow(2,(t-1)),myrank+(int)pow(2,(t-1)),MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			//print(123,*orig_arr,*orig_len*(int)pow(2,t-1));
			//print(234,sendbuf5,*orig_len*(int)pow(2,t-1));
			merge_arr(*orig_arr,*orig_len*(int)pow(2,t-1),sendbuf5,*orig_len*(int)pow(2,t-1),sendbuf2);
			//print(444,*orig_arr,*orig_len*(int)pow(2,t));
			*orig_arr=sendbuf2; //pass merged value into orig_arr
			//printf("%d: ",myrank);
			//print(444,*orig_arr,*orig_len*(int)pow(2,t));
				if(myrank%(int)pow(2,t+1)!=0){
				sendbuf5 = *orig_arr;
				MPI_Send(sendbuf5,*orig_len*(int)pow(2,t),MPI_INT,myrank-(int)pow(2,(t)),myrank,MPI_COMM_WORLD);
				}
			
		}
	t++;
	}
	if(myrank==0){
		/*
	free(sendbuf);
	free(sendbuf2);
	free(sendbuf5);
	free(sendbuf3);
	*/
	}
	
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
    for( i = 0; i < local_len; ++i ){
        local_arr[i] = rand()%10;
	}
    /* Parallel sort */
	
    par_sort( &local_arr, &local_len,myrank,nprocs);

    /* Verify the results */
    if( myrank == 0 ) {
		print(666,local_arr,nprocs*local_len);
        int res = is_arr_sorted( local_arr, local_len );
        printf( "Sorted: %d\n", res );
    }

    free( local_arr );

    MPI_Finalize();

    return 0;
}
