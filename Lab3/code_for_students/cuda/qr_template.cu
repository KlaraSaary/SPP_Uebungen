//
//  main.c
//  qr
//
//  Created by Zia Ul-Huda on 21/11/2016.
//  Copyright © 2016 TU Darmstadt. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define CUDA_ERROR_CHECK

#define CudaSafeCall( err ) __cudaSafeCall( err, __FILE__, __LINE__ )
#define CudaCheckError()    __cudaCheckError( __FILE__, __LINE__ )

void showGPUMem();

inline void __cudaSafeCall( cudaError err, const char *file, const int line )
{
#ifdef CUDA_ERROR_CHECK
    if ( cudaSuccess != err )
    {
        fprintf( stderr, "cudaSafeCall() failed at %s:%i : %s\n",
                file, line, cudaGetErrorString( err ) );
        exit( -1 );
    }
#endif

    return;
}

inline void __cudaCheckError( const char *file, const int line )
{
#ifdef CUDA_ERROR_CHECK
    cudaError err = cudaGetLastError();
    if ( cudaSuccess != err )
    {
        fprintf( stderr, "cudaCheckError() failed at %s:%i : %s\n",
                file, line, cudaGetErrorString( err ) );
	showGPUMem();
        exit( -1 );
    }

    // More careful checking. However, this will affect performance.
    // Comment away if needed.
    /*   err = cudaDeviceSynchronize();
     if( cudaSuccess != err )
     {
     fprintf( stderr, "cudaCheckError() with sync failed at %s:%i : %s\n",
     file, line, cudaGetErrorString( err ) );
     exit( -1 );
     }*/
#endif

    return;
}


typedef struct {
    int m, n;
    double * v;
} mat_t, mat;

#define BLOCK_SIZE 16
#define MAX_INT 100
#define EPSILON 0.00000001

int numBlocks;
dim3 dimGrid, dimBlock;
int numBlocksSingle, numThreadsSingle;

//get current wall time
double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        exit(-1);
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

//creates a new structure of mat type with m*n dimensions and
//returns its pointer
mat* matrix_new(int m, int n)
{
    mat *x = (mat*)malloc(sizeof(mat_t));
    x->v = (double*)calloc(sizeof(double), m * n);
    x->m = m;
    x->n = n;
    return x;
}

/**
 * Creates a new structure of type mat
 * on the device and initializes it. It returns
 * the pointer to the structure in *x
 */
void cuda_matrix_new(int m, int n, mat** x)
{
    double* d_arr;
	mat* temp=(mat_t*)malloc(sizeof(mat_t)) ;
	temp->m = m;
        temp->n = n;
	
    //allocate mat struct on device
	cudaMalloc(x,sizeof(mat_t));

    CudaCheckError();

    //allocate array on device and set it to 0

    cudaMalloc(&d_arr, m*n*sizeof(double));

    CudaCheckError();
    cudaMemset(d_arr, 0, sizeof(double) * m * n);
    CudaCheckError();

    //store the device pointer in temp object
    temp->v = d_arr;
   
    //copy the temp to device object
	fprintf(stderr,"*x: %p and &temp:%p\n",*x,temp->v);
    cudaMemcpy(*x, temp, sizeof(mat_t),
				cudaMemcpyHostToDevice);

    CudaCheckError();
free(temp);
}

//delete a matrix
void matrix_delete(mat *m)
{
    free(m->v);
    free(m);
}

/**
 * Free the memory of the structure pointed to by
 * m on the device. Make sure to also free the memory
 * of the elements of the matrix.
 */
void cuda_matrix_delete(mat *m)
{
    mat* temp;

    // Copy m to host

	cudaMemcpy(&temp,m,sizeof(mat),cudaMemcpyDeviceToHost);

    CudaCheckError();

    // Free array in m

	cudaFree(temp->v);

    CudaCheckError();

    // Free m
    cudaFree(m);
    CudaCheckError();

}

//calculate transpose of a matrix
void matrix_transpose(mat *m)
{
    int i,j;
    for (i = 0; i < m->m; i++) {
        for (j = 0; j < i; j++) {
            double t = m->v[i*m->n+j];
            m->v[i*m->n+j] = m->v[j*m->m+i];
            m->v[j*m->m+i] = t;
        }
    }
}

/**
 * Transpose the matrix on the device
 */
__global__
void cuda_matrix_transpose(mat* m){
    //Calculate the row of current element
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    //Calculate the column of current element
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    //Just the threads in lower triangle should swap m elements
    if(row<m->m && col<m->n && row<col){
        double t = m->v[row*m->n+col];
        m->v[row*m->n+col] = m->v[col*m->m+row];
        // Finish swapping
       	m->v[col*m->m+row] = t;
    }
}

//Create a new matrix and initialize its elements randomly
mat* matrix_create(int m, int n)
{
    mat *x = matrix_new(m, n);
    srand(time(NULL));
    int i,j;
    for (i = 0; i < m*n; i++){
        j=rand() % MAX_INT;
        x->v[i] = j;
    }
    return x;
}

//multiplication of two matrixes
mat* matrix_mul(mat *x, mat *y)
{
    if (x->n != y->m) return NULL;
    mat *r = matrix_new(x->m, y->n);
    int i,j,k;
    for (i = 0; i < x->m; i++)
        for (j = 0; j < y->n; j++)
            for (k = 0; k < x->n; k++)
                r->v[i*r->n+j] += x->v[i*x->n+k] * y->v[k*y->n+j];
    return r;
}


/**
 * Multiply matrices x and y on the device and store
 * the result in r on the device. r contains already
 * enough memory for the result matrix.
 */
__global__
void cuda_matrix_mul(mat* x, mat* y, mat* r)
{
    //calculate the row and column index of matrixes x and y respectively
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if(row < x->m && col < y->n){
        double rValue=0;

        //each thread computes one element of r
        int k;
        for(k=0; k < x->n; ++k)
            rValue += x->v[row*x->n+k]*y->v[k*y->n+col];

        r->v[row*r->n+col] = rValue;
    }
}

//calculate minor of a matrix given int d. Set first d
//diagonal entries to 1 and and set the rest of elements of
//first d rows and columns to zero. Then copy rest of the
//elements from the given matrix and return the pointer to new
//object
mat* matrix_minor(mat *x, int d)
{
    mat *m = matrix_new(x->m, x->n);
    int i,j;
    for (i = 0; i < d; i++)
        m->v[i*m->n+i] = 1;
    for (i = d; i < x->m; i++)
        for (j = d; j < x->n; j++)
            m->v[i*m->n+j] = x->v[i*x->n+j];
    return m;
}

/**
 * Calculate minor of a matrix given int d on device
 */
__global__
void cuda_matrix_minor(mat* x, int d, mat* m){
    //calculate the row and column index of matrixes x and y
    //respectively
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if(row < x->m && col < x->n){
        if (row == col && row < d)
            m->v[row*m->n+col]=1;
        if(row >= d && row < x->m && col >= d && col < x->n)
            m->v[row*m->n+col]=x->v[row*x->n+col];
    }
}

// c = a + b * s
double *vmadd(double a[], double b[], double s, double c[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        c[i] = a[i] + s * b[i];
    return c;
}

/**
 * c = a + b * s on device
 */
__global__
void cuda_vmadd(double a[], double b[], double *s, double c[], int n){
    int row = blockIdx.x * blockDim.x + threadIdx.x;
    if(row <n)
        c[row] = a[row] + b[row]*(*s);
}

// m = I - 2vv^T
mat* vmul(double v[], int n)
{
    mat *x = matrix_new(n, n);
    int i,j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            x->v[i*x->n+j] = -2 *  v[i] * v[j];
    for (i = 0; i < n; i++)
        x->v[i*x->n+i] += 1;

    return x;
}

/**
 * m = I - 2vv^T  on device
 */
__global__
void cuda_vmul(double v[], int n, mat* m)
{
    //calculate the row and column index of matrixes x and y respectively
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if(row < n && col < n){
        m->v[row*m->n+col] = -2*v[row]*v[col];
        if(row == col)
            m->v[row*m->n+col] += 1;
    }

}

// ||x||
double vnorm(double x[], int n)
{
    double sum = 0;
    int i;
    for (i = 0; i < n; i++) sum += x[i] * x[i];
    return sqrt(sum);
}


/**
 * Call with <<1,1>>
 * ||x|| on device and result is given in *a.
 * If flag is true (!= 0) a is multiplied with -1
 */
__global__
void cuda_vnorm(double x[], int n, double *a, int flag)
{
    if(blockIdx.x == 0 && threadIdx.x == 0 ) {
        double sum = 0;
        int i;
        for (i = 0; i < n; i++)
            sum += x[i]*x[i];
        *a = sqrt(sum);
        if (flag) *a = *a*(-1);
    }
}


// y = x / d
double* vdiv(double x[], double d, double y[], int n)
{
    int i;
    for (i = 0; i < n; i++) y[i] = x[i] / d;
    return y;
}

/**
 * y = x / d on device
 */
__global__
void cuda_vdiv(double x[], double *d, double y[], int n)
{
    int row = blockIdx.x * blockDim.x + threadIdx.x;
    if(row <n)
       y[row]=x[row]/(*d);
}

// take c-th column of m, put in v
double* mcol(mat *m, double *v, int c)
{
    int i;
    for (i = 0; i < m->m; i++)
        v[i] = m->v[i*m->n+c];
    return v;
}

/**
 * Take c-th column of m, put in v on device
 */
__global__
void cuda_mcol(mat *m, double *v, int c)
{
    int row = blockIdx.x * blockDim.x + threadIdx.x;
    if(row < m->m)
        v[row] = m->v[row*m->n+c];
}

/**
 * Initialize vector e where k-th element is set to 1
 * and all other are 0 on device
 */
__global__
void cuda_initialize_e(double* e, int n, int k){
    int row = blockIdx.x * blockDim.x + threadIdx.x;
    if(row < n){
        if(row=k){
            e[row] = 1;
		}else{
            e[row] = 0;
		}
    }
}

//visualize a matrix
void matrix_show(mat *m)
{
    int i,j;
    for(i = 0; i < m->m; i++) {
        for (j = 0; j < m->n; j++) {
            printf(" %8.3f", m->v[i*m->n+j]);
        }
        printf("\n");
    }
    printf("\n");
}

//householder calculations
void householder(mat *m, mat **R, mat **Q)
{
    mat *q[m->m];
    mat *z = m, *z1;
    int i,k;
    for (k = 0; k < m->n && k < m->m - 1; k++) {
        double e[m->m], x[m->m], a;
        z1 = matrix_minor(z, k);
        if (z != m) matrix_delete(z);
        z = z1;

        mcol(z, x, k);
        a = vnorm(x, m->m);
        if (m->v[k*m->n+k] > 0) a = -a;

        for (i = 0; i < m->m; i++)
            e[i] = (i == k) ? 1 : 0;

        vmadd(x, e, a, e, m->m);
        vdiv(e, vnorm(e, m->m), e, m->m);
        q[k] = vmul(e, m->m);
        z1 = matrix_mul(q[k], z);
        if (z != m) matrix_delete(z);
        z = z1;
    }
    matrix_delete(z);
    *Q = q[0];
    *R = matrix_mul(q[0], m);
    for (i = 1; i < m->n && i < m->m - 1; i++) {
        z1 = matrix_mul(q[i], *Q);
        if (i > 1) matrix_delete(*Q);
        *Q = z1;
        matrix_delete(q[i]);
    }
    matrix_delete(q[0]);
    z = matrix_mul(*Q, m);
    matrix_delete(*R);
    *R = z;
    matrix_transpose(*Q);
}

/**
 * Householder calculations with calls to device kernels
 */
void cuda_householder(mat *m, mat **R, mat **Q, mat *original)
{
    mat *q;
    mat *z = m, *z1;
    int k;
    double *e, *x, *a;

    // Alloc vector e
    cudaMalloc((void**)&e, sizeof(double) * original->m);
    CudaCheckError();

    // Alloc vector x
    cudaMalloc((void**)&x, sizeof(double) * original->m);
    CudaCheckError();

    // Alloc scalar a
    cudaMalloc((void**)&a, sizeof(double));
    CudaCheckError();

    //showGPUMem();

    for (k = 0; k < original->n && k < original->m - 1; k++) {

        // Allocate and init matrix z1
	fprintf(stderr,"here lays the matrix z1: %p",&z1);

        cuda_matrix_new(original->m,original->n, &z1);
	fprintf(stderr, "MatrixNew\n");

        // One thread calculates one element of matrix z1
        cuda_matrix_minor<<<dimGrid, dimBlock>>>(original, k, z1 ); //Versuch, Idee alternativ (original, k, z1) wenn original->v == m->v
		fprintf(stderr, "matrix_minor\n");
        CudaCheckError();
        if (z != m) cuda_matrix_delete(z);
        z = z1;

        // One thread calculates one element of vector x
        cuda_mcol<<<numBlocksSingle,numThreadsSingle>>>(z, x, k); //Abgeschrieben von sequentieller Funktion
        //z müsste ein Möglichkeit bieten auf z->v bzw m->v zuzugreifen
		fprintf(stderr, "mcol\n");
        CudaCheckError();

        int f = (original->v[k*original->n+k] > 0) ? 1 : 0;
        // Call cuda_vnorm with only one thread
        cuda_vnorm<<<1,1>>>(x, original->m, a, f); //Eingetragen: 1,1
		fprintf(stderr, "vnorm\n");
        CudaCheckError();

        // One thread calculates one element of vector e
        cuda_initialize_e<<<numBlocksSingle,numThreadsSingle>>>(e, original->m, k); //Eingetragen (e, original->m, k)
		fprintf(stderr, "cuda_initalize\n");
        CudaCheckError();

        // One thread calculates one element of vector e
        cuda_vmadd<<<1,1>>>(x, e, a, e, original->m); //Eingetragen 1,1 und original->m
		fprintf(stderr, "vmadd\n");
        CudaCheckError();

        // Call cuda_vnorm with only one thread
        cuda_vnorm<<<1,1>>>(e, original->m, a, 0);
        CudaCheckError();
        // One thread calculates one element of vector e with cuda_vdiv
        cuda_vdiv<<<1,1>>>(e, a, e, original->m); //EIngetragen: cuda_cdiv und 1,1
		fprintf(stderr, "vdiv\n");
        CudaCheckError();

        // Allocate matrix q
	fprintf(stderr,"here lays the matrix q: %p",&q);
        cuda_matrix_new(original->m, original->n, &q);
        // One thread calculates one element of matrix q
        cuda_vmul<<<dimGrid, dimBlock>>>(e, original->m, q);
	fprintf(stderr, "vmul\n");
        CudaCheckError();

        // Allocate matrix z1
        cuda_matrix_new(original->m,original->n,&z1);
        // One thread calculates one element of matrix z1
        // Calculate matrix product z1 = q*z with cuda_matrix_mul
        cuda_matrix_mul<<<numBlocksSingle,numThreadsSingle>>>(q,z,z1); //Komplett selbst geschrieben
	fprintf(stderr, "cuda_matrix_mul\n");
        CudaCheckError();

        if (z != m) cuda_matrix_delete(z);
        z = z1;

        if(k==0){
            *Q = q;
        }
        else if(k>0){
            cuda_matrix_new(original->m, original->m, &z1);
            cuda_matrix_mul<<<dimGrid, dimBlock>>>(q, *Q, z1);
            CudaCheckError();

            cuda_matrix_delete(*Q);
            *Q = z1;
            cuda_matrix_delete(q);
        }

    }

    // Free temporary storage on device
    cudaFree(e);
    CudaCheckError();
    cudaFree(x);
    CudaCheckError();
    cudaFree(a);
    CudaCheckError();
    cuda_matrix_delete(z);


    cuda_matrix_new(original->m, original->n, R);
    // Result matrix R
    cuda_matrix_mul<<<dimGrid, dimBlock>>>(*Q, m, *R);
    CudaCheckError();
    // Result matrix Q
    cuda_matrix_transpose<<<dimGrid, dimBlock>>>(*Q);
    CudaCheckError();
}

/** Task2
 * Deep copy of matrix x to the device.
 * Return pointer to new structure on device in *dX
 */
void copyToDevice(mat** dX, mat* x){
    mat temp;
    temp.m = x->m;
    temp.n = x->n;
    double* d_arr;

    //allocate device matrix
    cudaMalloc((void**)dX, sizeof(mat));
    CudaCheckError();

    //allocate device array
    cudaMalloc((void**)&d_arr, x->m*x->n*sizeof(double));
	CudaCheckError();

    //copy contents of x array
    cudaMemcpy(d_arr, x->v, x->m*x->n*sizeof(double), cudaMemcpyHostToDevice);
    CudaCheckError();

    //save d_arr in temp
    temp.v = d_arr;

    //copy the temp to device object
    cudaMemcpy(*dX, &temp, sizeof(mat_t), cudaMemcpyHostToDevice);
    CudaCheckError();
}

/**
 * Deep copy of matrix dX to the host.
 * Return pointer to new structure on host in *x
 */
void copyToHost(mat** x, mat* dX){
    *x = (mat*)malloc(sizeof(mat_t));
    cudaMemcpy(*x, dX, sizeof(mat_t), cudaMemcpyDeviceToHost);
    CudaCheckError();

    double* temp = (double*)malloc(sizeof(double) * (*x)->m * (*x)->n);
    // Copy array of dX to temp
    cudaMemcpy(temp, (*x)->v, sizeof(double) * (*x)->m * (*x)->n, cudaMemcpyDeviceToHost);
    CudaCheckError();

    (*x)->v = temp;
}

//check if two matrixes are equal with their corrsponding element's values being within an epsilon
int is_equal(mat *m, mat *x){
    if(m->m != x->m || m->n != x->n) return 0;
    int i;

    for(i=0; i< (m->m * m->n); ++i)
        if(abs(m->v[i] - x->v[i]) > EPSILON) return 0;

    return 1;
}

void showGPUMem(){
    // show memory usage of GPU

    size_t free_byte ;
    size_t total_byte ;
    cudaError_t cuda_status = cudaMemGetInfo( &free_byte, &total_byte ) ;

    if ( cudaSuccess != cuda_status ){
        printf("Error: cudaMemGetInfo fails, %s \n", cudaGetErrorString(cuda_status) );
        exit(1);
    }

    double free_db = (double)free_byte ;
    double total_db = (double)total_byte ;
    double used_db = total_db - free_db ;

    printf("GPU memory usage: used = %f MB, free = %f MB, total = %f MB\n",
            used_db/1024.0/1024.0, free_db/1024.0/1024.0,
            total_db/1024.0/1024.0);
}

int main(int argc, char *argv[])
{
    if(argc != 3){
        puts("Usage: qr #rows #cols\n //#rows > 2 and #cols > 1\n");
        exit(0);
    }
    int row = atoi(argv[1]), col = atoi(argv[2]);

    if(row < 3 || col < 2){
        puts("Error: invalid number of rows or columns\n");
        exit(0);
    }

    int maxDim = (row > col) ? row : col;
    //use maxDim to calculate dimensions of grids and blocks for 2D cuda kernels
    numBlocks = maxDim / BLOCK_SIZE;
    if(maxDim % BLOCK_SIZE) numBlocks++;

    dimGrid.x = numBlocks; dimGrid.y = numBlocks;
    // Every CUDA block is of size (x,y,z) = (BLOCK_SIZE,BLOCK_SIZE,1) threads
    dimBlock.x = BLOCK_SIZE; dimBlock.y = BLOCK_SIZE;

    //dimensions of blocks and threads for 1D cuda kernels for vectors
    // Every CUDA block is of size (x,y,z) = (BLOCK_SIZE*BLOCK_SIZE,1,1)
    numThreadsSingle = BLOCK_SIZE * BLOCK_SIZE;
    numBlocksSingle = maxDim/numThreadsSingle;
    if(maxDim % numThreadsSingle) ++numBlocksSingle;

    mat *R = NULL, *Q = NULL, *dX = NULL, *dQ = NULL, *dR = NULL;
    //showGPUMem();
    //create a random row*col matrix
    mat *x = matrix_create(row, col);
    //puts("x"); matrix_show(x);
	fprintf(stderr,"matix x %d,%d,%p\n",x->m,x->n,x->v);

    double time_start = get_wall_time();

    //copy x to device
    copyToDevice(&dX, x);
	fprintf(stderr,"adress &X: %p\n",dX);
    //showGPUMem();
    //householder calculations on device
    cuda_householder(dX, &dR, &dQ, x);

    //copy the calculated dR and dQ to host
    copyToHost(&R, dR);
    copyToHost(&Q, dQ);

    double time_end = get_wall_time();

    //puts("Q"); matrix_show(Q);
    //puts("R"); matrix_show(R);

    // to show their product is the input matrix
    mat* dM = NULL;
    cuda_matrix_new(x->m, x->n, &dM);
    cuda_matrix_mul<<<dimGrid, dimBlock>>>(dQ, dR, dM);

    //copy resultant matrix to host
    mat* m = NULL;
    copyToHost(&m, dM);
    //puts("Q * R"); matrix_show(m);


    printf("Verification: ");
    if(is_equal(m, x))
        printf("Successful\n");
    else
        printf("Unsuccessful\n");

    printf("Time taken: %8.3f seconds\n",time_end - time_start);

    matrix_delete(x);
    matrix_delete(R);
    matrix_delete(Q);
    matrix_delete(m);
    cuda_matrix_delete(dX);
    cuda_matrix_delete(dQ);
    cuda_matrix_delete(dR);
    cuda_matrix_delete(dM);
	fflush(stdout);
    return 0;
}
