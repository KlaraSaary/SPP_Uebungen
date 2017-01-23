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

typedef struct {
    int m, n;   //dimensions of the matrix
    double * v; //1D array of double values to hold m*n elements of a matrix
} mat_t, mat;

#define MAX_INT 100
#define EPSILON 0.00000001

//get current wall time
double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        exit(-1);
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}


//creates a new structure of mat type with m*n dimensions and returns its pointer
mat* matrix_new(int m, int n)
{
    mat *x = malloc(sizeof(mat_t));
    x->v = calloc(m * n, sizeof(double));
    x->m = m;
    x->n = n;
    return x;
}

//delete a matrix
void matrix_delete(mat *m)
{
    free(m->v);
    free(m);
}

//calculate transpose of a matrix
void matrix_transpose(mat *m)
{
    int i,j;
    for (i = 0; i < m->m; i++) {
        for (j = 0; j < i; j++) {
            double t = m->v[i*m->m+j];
            m->v[i*m->m+j] = m->v[j*m->n+i];
            m->v[j*m->n+i] = t;
        }
    }
}

//Create a new matrix and initialize its elements randomly
mat* matrix_create(int m, int n)
{
    mat *x = matrix_new(m, n);
    srand(time(NULL));
    int i;
    for (i = 0; i < m*n; i++)
        x->v[i] = rand() % MAX_INT;
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

//calculate minor of a matrix given d. Set first d diagonal entries to 1 and
//set the rest of elements of first d rows and columns to zero. Then copy rest
//of the elements from the given matrix and return the pointer to new object
mat* matrix_minor(mat *x, int d)
{
    mat *c = matrix_new(x->m, x->n);
    int i,j;
    for (i = 0; i < d; i++)
        c->v[i*(x->n)+i] = 1;
    for (i = d; i < x->m; i++)
        for (j = d; j < x->n; j++)
            c->v[i*(x->n)+j] = x->v[i*(x->n)+j];
    
    return c;
}

/* c = a + b * s */
double *vmadd(double a[], double b[], double s, double c[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        c[i] = a[i] + s * b[i];
    return c;
}

//multiply a vector with a matrix and save the results into a new matrix object.
// M[i,j] = v[i] * v[j] * -2
// M[i,i] += 1;
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

/* ||x|| */
double vnorm(double x[], int n)
{
    double sum = 0;
    int i;
    for (i = 0; i < n; i++) sum += x[i] * x[i];
    return sqrt(sum);
}


/* y = x / d */
double* vdiv(double x[], double d, double y[], int n)
{
    int i;
    for (i = 0; i < n; i++) y[i] = x[i] / d;
    return y;
}

/* take c-th column of m, put in v */
double* mcol(mat *m, double *v, int c)
{
    int i;
    for (i = 0; i < m->m; i++)
        v[i] = m->v[i*m->n+c];
    return v;
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
    mat *q;
    mat *z = m, *z1;
    int k,i;
    
    //for 0 - n-1 columns of m
    for (k = 0; k < m->n && k < m->m - 1; k++) {
        double e[m->m], x[m->m], a;
        z1 = matrix_minor(z, k);
        if (z != m) matrix_delete(z);
        z = z1;
        
        
        mcol(z, x, k);
        a = vnorm(x, m->m);
        if (m->v[k*m->m+k] > 0) a = -a;
        
        for (i = 0; i < m->m; i++)
            e[i] = (i == k) ? 1 : 0;
        
        vmadd(x, e, a, e, m->m);
        vdiv(e, vnorm(e, m->m), e, m->m);
        q = vmul(e, m->m);
        
        z1 = matrix_mul(q, z);
        
        if (z != m) matrix_delete(z);
        z = z1;

        if(k==0){
            *Q = q;
        }
        else if(k>0){
             z1 = matrix_mul(q, *Q);
             matrix_delete(*Q);
            *Q = z1;
            matrix_delete(q);
        }
    }
    matrix_delete(z);
    
    *R = matrix_mul(*Q, m);
    
    matrix_transpose(*Q);
}

//check if two matrixes are equal with their corresponding element's values
//being within an epsilon
int is_equal(mat *m, mat *x){
    if(m->m != x->m || m->n != x->n) return 0;
    int i;
    
    for(i=0; i< (m->m * m->n); ++i)
        if(abs(m->v[i] - x->v[i]) > EPSILON) return 0;
    
    return 1;
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
    
    mat *R, *Q;
    
    //create a random row*col matrix
    mat *x = matrix_create(row, col);
    //puts("x"); matrix_show(x);
    
    double time_start = get_wall_time();

    //compute R and Q
    householder(x, &R, &Q);
    
    double time_end = get_wall_time();

    //puts("Q"); matrix_show(Q);
    //puts("R"); matrix_show(R);
    
    // to show their product is the input matrix
    mat *m = matrix_mul(Q, R);
    //puts("Q * R"); matrix_show(m);

    //verify results
    printf("Verification: ");
    if(is_equal(m, x))
        printf("Successful\n");
    else
        printf("Unsuccessful\n");

    printf("Time taken: %f Seconds\n",time_end - time_start);

    matrix_delete(x);
    matrix_delete(R);
    matrix_delete(Q);
    matrix_delete(m);
    return 0;
}
