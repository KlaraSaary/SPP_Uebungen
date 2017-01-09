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

void merge_arr( int* arr1, int n1, int* arr2, int n2, 
                int* tmp_arr ) {
	int i = 0, j = 0, k = 0;
    	while (i<n1 && j<n2){
        	if (arr1[i] < arr2[j]){
	            tmp_arr[k] = arr1[i];
	            i++;
        	}
        	else{
	            tmp_arr[k] = arr2[j];
	            j++;
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

void print(int num,int* arr, int local_len){
	int o;
	printf("process %d: [",num);
	for(o=0;o<local_len;o++){
		printf("%d ",arr[o]);
	}
	printf("]\n");
}

void fill(int* arr,int n, int** buf){

	//print(1234,*buf,n);
	int d;
	for(d=0;d<n;d++){
			//printf("%d and \n",arr[d]);
				//*buf[d]=arr[d];
				//printf("%d\n",*buf[d]);
	}
}

/**
 * Do the parallel sorting
 */
void par_sort( int** orig_arr, int* orig_len, int myrank, 
               int nprocs ) {

	int n = *orig_len/nprocs;
	int z,t,q,position,d,i;
	int* sendbuf = malloc(*orig_len*sizeof(int));
	int* sendbuf3 = malloc(*orig_len*sizeof(int));
	int* sendbuf2 = malloc(*orig_len*sizeof(int));
	int* solution = malloc(*orig_len*sizeof(int));
	int* proc_i=malloc(*orig_len*sizeof(int));
	int* proc_z=malloc(*orig_len*sizeof(int));
	
	
	//print(12,*orig_arr,*orig_len);
	
	
	if(myrank==0){
	
	//printf("rank 0 successfully called\n");
	sendbuf=*orig_arr;
	
	//print(12,*orig_arr,n,&sendbuf);
	print(12,*orig_arr,*orig_len);
	for(d=0;d<n;d++){
		
			printf("%d\n",sendbuf[d]);
				//*buf[d]=arr[d];
				//printf("%d\n",*buf[d]);
	}
		
	
		
	//print(myrank,sendbuf,n);
		
	position=0;
		
		for(i=0;i<nprocs;i++){
			for(d=0;d<n;d++){
				sendbuf3[d]=sendbuf[position+d];
				//printf("%d\n",sendbuf[d]);
			}
			
			MPI_Send(sendbuf3,n,MPI_INT,i,0,MPI_COMM_WORLD);
			position += n;
		}
	
		
		i=1;
		
			//printf("%d\n",i);
		for(q=0;q<n;q++){
			proc_i[q]=sendbuf[q];	
		}
		qsort(proc_i,n,sizeof(int),compare);
			//printf("here comes the plane #%d ",i);
			//print(11,proc_i,*orig_len);
		//for(t=0;t<log2(n);t++){
		while(i<4){
		MPI_Recv(sendbuf2,*orig_len,MPI_INT,i,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			for(q=0;q<*orig_len;q++){
				proc_z[q]=sendbuf2[q];
			}
		merge_arr(proc_i,i*n,proc_z,n,solution);
			for(q=0;q<*orig_len;q++){
				proc_i[q]=solution[q];
			}
		i++;	
		}
		
		
		
		print(9999, proc_i,*orig_len);
		*orig_arr=proc_i;	
	
	}else{
	//printf("%d calles successfully\n",myrank);	
		
		MPI_Recv(sendbuf3,n,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		qsort(sendbuf3,n,sizeof(int),compare);
		for(i=0;i<n;i++){
			sendbuf2[i]=sendbuf3[i];
		}
		MPI_Send(sendbuf2,n,MPI_INT,0,myrank,MPI_COMM_WORLD);
		//print(myrank,sendbuf2,n);
			
		
	
	
	
	
	}
	
}
    /* Allocate temp arrays */
	
    
    // Use merge_arr()
    

    /* Free memory */


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
        int res = is_arr_sorted( local_arr, local_len );
       // printf( "Sorted: %d\n", res );
    }

    free( local_arr );

    MPI_Finalize();

    return 0;
}
