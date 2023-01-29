#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include<pthread.h>
#define MSG "* running cpubench with seed %s mode %s using %s with size %s and %s threads with debug %s...\n"

#define USAGE "usage: ./cpubench <seed> <mode> <type> <size> <threads> <debug> \n" \
"     - seed: 0 \n" \
"     - mode: flops / matrix \n" \
"     - type: single / double \n" \
"     - size: 256 / 1024 / 4096 / 16386 -- MegaFlops / array dimension \n" \
"     - threads: 1 / 2 / 4 \n" \
"     - debug: true / false \n" \


#define MEGAFLOPS 1000000
#define MEGABYTES 1024*1024

int DEBUG = false;


//MUST MODIFY TO SATISFY ASSIGNMENT BELOW THIS LINE//
//=================================================//

int SIZE = 500;
int size;
int num_threads;
int **arr1,**arr2,**arr3;
double **arr4, **arr5, **arr6;
double ** allocate_double_mat(int size){
	double * vals = (double *) malloc( size * size * sizeof(double) );
	double ** ptrs = (double **) malloc( size * sizeof(double*) );
	int i;
	for (i = 0; i < size; ++i) {
		ptrs[ i ] = &vals[ i * size ];
	}
	return ptrs;
}
int ** allocate_int_mat(int size){
	int * vals = (int *) malloc( size * size * sizeof(int) );
	int ** ptrs = (int **) malloc( size * sizeof(int*) );
	int i;
	for (i = 0; i < size; ++i) {
		ptrs[ i ] = &vals[ i * size ];
	}
	return ptrs;
}
void * worker_double(void *arg){
	int i, j, k, tid, portion_size, row_start, row_end;
	double sum;
	
	tid = *(int *)(arg); // get the thread ID assigned sequentially.
	portion_size = size / num_threads;
	row_start = tid * portion_size;
	row_end = (tid+1) * portion_size;
	if(tid==num_threads-1){
		if(SIZE % num_threads!=0){
			row_end +=(SIZE % num_threads);
		}
	}

	for (i = row_start; i < row_end; ++i) { // hold row index of 'matrix1'
		for (j = 0; j < size; ++j) { // hold column index of 'matrix2'
		sum = 0; // hold value of a cell
		/* one pass to sum the multiplications of corresponding cells
		in the row vector and column vector. */
		for (k = 0; k < size; ++k) { 
		sum += arr4[ i ][ k ] * arr5[ k ][ j ];
		}
		arr6[ i ][ j ] = sum;
		}
	}

}
void * worker_int(void *arg){
	int i, j, k, tid, portion_size, row_start, row_end;
	int sum;
	
	tid = *(int *)(arg); // get the thread ID assigned sequentially.
	portion_size = size / num_threads;
	row_start = tid * portion_size;
	row_end = (tid+1) * portion_size;
	if(tid==num_threads-1){
		if(SIZE % num_threads!=0){
			row_end +=(SIZE % num_threads);
		}
	}

	for (i = row_start; i < row_end; ++i) { // hold row index of 'matrix1'
		for (j = 0; j < size; ++j) { // hold column index of 'matrix2'
		sum = 0; // hold value of a cell
		/* one pass to sum the multiplications of corresponding cells
		in the row vector and column vector. */
		for (k = 0; k < size; ++k) { 
		sum += arr1[ i ][ k ] * arr2[ k ][ j ];
		}
		arr3[ i ][ j ] = sum;
		}
	}

}
// This function multiplies mat1[][] and mat2[][],
// and stores the result in res[][]
//may want to print periodic status to the screen to verify the rate of progress
void multiply_double(int size, double mat1[size][size], double mat2[size][size], double res[size][size])
{
    int i, j, k;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            res[i][j] = 0;
            for (k = 0; k < size; k++)
                res[i][j] += mat1[i][k] * mat2[k][j];
        }
    }
}

void multiply_int(int size, int mat1[size][size], int mat2[size][size], int res[size][size])
{
    int i, j, k;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            res[i][j] = 0;
            for (k = 0; k < size; k++)
                res[i][j] += mat1[i][k] * mat2[k][j];
        }
    }
}


void compute_flops_int(int size)
{
	int index, index2;
	int result;
	int loops = size;
	for (index=0;index<loops;index++)
	{
		for (index2=0;index2<MEGAFLOPS;index2++)
			result = index2+1;
	}
	if (DEBUG)
		printf("%d\n",result);
	
}
void* worker_flop_int(void* arg){
	long index, index2;
	long result;
	long loops = SIZE;
	long i, j, k, p, portion, row_start, row_end;
	p = *(long *)(arg); 
	portion = SIZE / num_threads;
	row_start = p * portion;
	row_end = (p+1) * portion;
	
	// printf("%ld %ld \n",row_start,row_end); //to check which rows work in which thread
	//row_start and row_end signify the different range from which individual threads will run
	for (index=row_start;index<row_end;index++)
	{
		for (index2=0;index2<MEGAFLOPS;index2++)
			result = index2+1;
	}
	if (DEBUG)
		printf("%ld\n",result);
}
void* worker_flop_double(void* arg){
	long index, index2;
	double result;
	long loops = SIZE;
	long i, j, k, p, portion, row_start, row_end;
	p = *(long *)(arg); 
	portion = SIZE / num_threads;
	row_start = p * portion;
	row_end = (p+1) * portion;
	// printf("%")
	for (index=row_start;index<row_end;index++)
	{
		for (index2=0;index2<MEGAFLOPS;index2++)
			result = index2+1;
	}
	if (DEBUG)
		printf("%lf\n",result);
}
void compute_flops_double(int size)
{
	int index, index2;
	double result;
	int loops = size;
	for (index=0;index<loops;index++)
	{
		for (index2=0;index2<MEGAFLOPS;index2++)
			result = index2+1.0;
	}
	if (DEBUG)
		printf("%lf\n",result);
	
}

//=================================================//
//MUST MODIFY TO SATISFY ASSIGNMENT ABOVE THIS LINE//


int compute_checksum_static_double(int size, double res[size][size]){
	if (DEBUG)
		printf("compute_checksum...\n");
	int checksum = 0;
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
                checksum += (int)res[i][j];
        }
    }
    return checksum;
}

int compute_checksum_static_int(int size, int res[size][size]){
	if (DEBUG)
	printf("compute_checksum...\n");
	int checksum = 0;
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
                checksum += res[i][j];
        }
    }
    return checksum;
}


int compute_checksum_dynamic_double(int size, double** res){
	if (DEBUG)
	printf("compute_checksum...\n");
	int checksum = 0;
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
                checksum += (int)res[i][j];
                //if (DEBUG)
                //printf("%d %lf\n", checksum, res[i][j]);
        }
    }
    return checksum;
}

int compute_checksum_dynamic_int(int size, int** res){
	if (DEBUG)
	printf("compute_checksum...\n");
	int checksum = 0;
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
                checksum += res[i][j];
        }
    }
    return checksum;
}



int main(int argc, char **argv)
{
	int checksum = 0;
	time_t t;
	srand((unsigned) time(&t));
    if (argc != 7) 
    {
        printf(USAGE);
		printf("mode=-1 type=-1 size=-1 threads=-1 time=-1 throughput=-1 checksum=-1\n");
        exit(1);
    } 
    else 
    {
    	
    	int seed = atoi(argv[1]);
		srand(seed);
    	
		int mode = -1;
        if(strcmp(argv[2],"flops") == 0)
        	mode = 0;
        else if(strcmp(argv[2],"matrix") == 0)
        	mode = 1;
        else
        	mode = -1;

		int type = -1;
        if(strcmp(argv[3],"single") == 0)
        	type = 0;
        else if(strcmp(argv[3],"double") == 0)
        	type = 1;
        else
        	type = -1;


        size = atoi(argv[4]);
		SIZE=size;
        num_threads = atoi(argv[5]);

        if(strcmp(argv[6],"true") == 0)
        	DEBUG = true;
        else if(strcmp(argv[6],"false") == 0)
        	DEBUG = false;
        else
        {
        	printf("error in DEBUG argument, must be either true or false; exiting...\n");
			printf("seed=%s mode=%s type=%s size=%d threads=%d time=0.0 throughput=0.0 checksum=-2\n",argv[1],argv[2],argv[3],size,num_threads);
			exit(-1);
		}	
		
		if (DEBUG)
    		printf(MSG, argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
		

		struct timeval start, end;

		if (mode == 0 && type == 0)
		{	
			gettimeofday(&start, NULL);
			/////// THREAD IMPLEMENTATION ///////
			long i,j,k;
			pthread_t * threads;
			threads = (pthread_t *) malloc( num_threads * sizeof(pthread_t) );
			for ( i = 0; i < num_threads; ++i ) {
				long *p;
				p = (long *) malloc( sizeof(long) );
				*p = i;
				pthread_create( &threads[i], NULL, worker_flop_int, (void *)p );
			}
			for ( i = 0; i < num_threads; ++i ) {
				pthread_join( threads[i], NULL );
			}
			/////// THREAD IMPLEMENTATION OVER ///////
			// compute_flops_int(size);
		    gettimeofday(&end, NULL);
		}
		else if (mode == 0 && type == 1)
		{	
			gettimeofday(&start, NULL);
			/////// THREAD IMPLEMENTATION ///////
			long i,j,k;
			pthread_t * threads;
			threads = (pthread_t *) malloc( num_threads * sizeof(pthread_t) );
			for ( i = 0; i < num_threads; ++i ) {
				long *p;
				p = (long *) malloc( sizeof(long) );
				*p = i;
				pthread_create( &threads[i], NULL, worker_flop_double, (void *)p );
			}
			for ( i = 0; i < num_threads; ++i ) {
				pthread_join( threads[i], NULL );
			}
			/////// THREAD IMPLEMENTATION OVER ///////
			// compute_flops_double(size);
		    gettimeofday(&end, NULL);
		}		
		else if (mode == 1 && type == 0)
		{	
		
			if (size != SIZE)
			{
				if (DEBUG)
					printf("you specified a size of %d, but there is a constant SIZE set to %d; your code is not correct, and must be updated to take the size parameter from the command line; exiting...", size, SIZE);
				else
					printf("seed=%s mode=%s type=%s size=%d threads=%d time=0.0 throughput=0.0 checksum=-3\n",argv[1],argv[2],argv[3],size,num_threads);
				exit(-1);
			}
			size_t len = 0;
			int i, j;//, k;
  
			len = sizeof(double *) * SIZE * SIZE * 3;
			if (DEBUG)
				printf("allocating %lf GB memory...\n",len*3.0/(1024*1024*1024)); 

//MUST MODIFY TO SATISFY ASSIGNMENT BELOW THIS LINE//
//=================================================//
	
			// int arr1[SIZE][SIZE];
			// int arr2[SIZE][SIZE];
			// int arr3[SIZE][SIZE];
			arr1 = allocate_int_mat( size );
			arr2 = allocate_int_mat( size );
			arr3 = allocate_int_mat( size );
			//set to false if dynamic memory allocation is used
			bool STATIC = false;
			pthread_t * threads;
			threads = (pthread_t *) malloc( num_threads * sizeof(pthread_t) );

//=================================================//
//MUST MODIFY TO SATISFY ASSIGNMENT ABOVE THIS LINE//


			for (i = 0; i < SIZE; i++) 
				for (j = 0; j < SIZE; j++) 
				{
					arr1[i][j] = (int)((rand()/rand())*10.0); 
					arr2[i][j] = (int)((rand()/rand())*10.0); 
					arr3[i][j] = 0.0; 
				}
		
			if (DEBUG)
  			{
    			for (i = 0; i < SIZE; i++) 
    			{
        			for (j = 0; j < SIZE; j++) 
            			printf("%d ", arr1[i][j]); 
        			printf("\n");
				}
				printf("\n");

    			for (i = 0; i < SIZE; i++) 
    			{
        			for (j = 0; j < SIZE; j++) 
            			printf("%d ", arr2[i][j]); 
        			printf("\n");
				}
				printf("\n");
			}			
		
		
		
			gettimeofday(&start, NULL);
// ---------------------------------------------- ////////
			for(int i=0;i<num_threads;i++){
				int *tid;
				tid = (int *) malloc( sizeof(int) );
				*tid = i;
				pthread_create( &threads[i], NULL, worker_int, (void *)tid );
			}
			for ( i = 0; i < num_threads; ++i ) {
				pthread_join( threads[i], NULL );
			}


// ----------------------------------------------- //////
			// multiply_int(size,arr1,arr2,arr3);
		    gettimeofday(&end, NULL);
		    
			if (STATIC == true)
				checksum = compute_checksum_static_int(size, arr3);
			else
				checksum = compute_checksum_dynamic_int(size, arr3);
		    if (DEBUG)
			{	    
				for (i = 0; i < size; i++) 
    			{
        			for (j = 0; j < size; j++) 
            			printf("%ld ", arr3[i][j]); 
        			printf("\n");
				}
				printf("\n");
			}	
		    
		}
		else if (mode == 1 && type == 1)
		{	
			//must change to take parameter from command line		
			if (size != SIZE)
			{
				if (DEBUG)
					printf("you specified a size of %d, but there is a constant SIZE set to %d; your code is not correct, and must be updated to take the size parameter from the command line; exiting...", size, SIZE);
				else
					printf("seed=%s mode=%s type=%s size=%d threads=%d time=0.0 throughput=0.0 checksum=-4\n",argv[1],argv[2],argv[3],size,num_threads);
					
				return -1;
			}
			size_t len = 0;
			int i, j;//, k;
  
			len = sizeof(double *) * SIZE * SIZE * 3;
			if (DEBUG)
				printf("allocating %lf GB memory...\n",len*3.0/(1024*1024*1024)); 

//MUST MODIFY TO SATISFY ASSIGNMENT BELOW THIS LINE//
//=================================================//
	
			// double arr1[SIZE][SIZE];
			// double arr2[SIZE][SIZE];
			// double arr3[SIZE][SIZE];
			arr4 = allocate_double_mat( size );
			arr5 = allocate_double_mat( size );
			arr6 = allocate_double_mat( size );
			//set to false if dynamic memory allocation is used
			bool STATIC = false;
			pthread_t * threads;
			threads = (pthread_t *) malloc( num_threads * sizeof(pthread_t) );
	//=================================================//
	//MUST MODIFY TO SATISFY ASSIGNMENT ABOVE THIS LINE//


			for (i = 0; i < SIZE; i++) 
				for (j = 0; j < SIZE; j++) 
				{
					arr4[i][j] = (double)rand()/rand(); // OR *(*(arr+i)+j) = ++count 
					arr5[i][j] = (double)rand()/rand(); // OR *(*(arr+i)+j) = ++count 
					arr6[i][j] = 0.0; 
				}
		
			if (DEBUG)
  			{
    			for (i = 0; i < SIZE; i++) 
    			{
        			for (j = 0; j < SIZE; j++) 
            			printf("%lf ", arr4[i][j]); 
        			printf("\n");
				}
				printf("\n");

    			for (i = 0; i < SIZE; i++) 
    			{
        			for (j = 0; j < SIZE; j++) 
            			printf("%lf ", arr5[i][j]); 
        			printf("\n");
				}
				printf("\n");
			}			
		
		
			gettimeofday(&start, NULL);
// ---------------------------------------------- ////////
			for(int i=0;i<num_threads;i++){
				int *tid;
				tid = (int *) malloc( sizeof(int) );
				*tid = i;
				pthread_create( &threads[i], NULL, worker_double, (void *)tid );
			}
			for ( i = 0; i < num_threads; ++i ) {
				pthread_join( threads[i], NULL );
			}


// ----------------------------------------------- //////
			// multiply_double(size,arr1,arr2,arr3);
		    gettimeofday(&end, NULL);

			if (STATIC == true)
				checksum = compute_checksum_static_double(size, arr6);
			else
				checksum = compute_checksum_dynamic_double(size, arr6);
	
			if (DEBUG)
			{	    
				for (i = 0; i < size; i++) 
    			{
        			for (j = 0; j < size; j++) 
            			printf("%lf ", arr6[i][j]); 
        			printf("\n");
				}
				printf("\n");
			}	    
	
		    
		    
		}
		else
		{
			if (DEBUG)
        	{
        		printf(USAGE);
				printf("unrecognized option, exiting...\n");
			}
			else
				printf("seed=%s mode=%s type=%s size=%d threads=%d time=0.0 throughput=0.0 checksum=-6\n",argv[1],argv[2],argv[3],size,num_threads);
				
        	exit(1);
		}

		double elapsed_time_sec = (((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)))/1000000.0;
		double num_giga_ops = 0;
		
		if (size*MEGAFLOPS < 0)
		{
			if (DEBUG)
				printf("error in size, check for overflow; exiting...\n");
			else
				printf("seed=%s mode=%s type=%s size=%d threads=%d time=0.0 throughput=0.0 checksum=-7\n",argv[1],argv[2],argv[3],size,num_threads);
			exit(1);
		}
		if (elapsed_time_sec < 0)
		{
			if (DEBUG)
				printf("error in elapsed time, check for proper timing; exiting...\n");
			else
				printf("seed=%s mode=%s type=%s size=%d threads=%d time=0.0 throughput=0.0 checksum=-8\n",argv[1],argv[2],argv[3],size,num_threads);
			exit(1);
		}
		if (elapsed_time_sec == 0)
		{
			if (DEBUG)
				printf("elapsed time is 0, check for proper timing or make sure to increase amount of work performed; exiting...\n");
			else
				printf("seed=%s mode=%s type=%s size=%d threads=%d time=0.0 throughput=0.0 checksum=-9\n",argv[1],argv[2],argv[3],size,num_threads);
			exit(1);
		}
			
		
		if (mode == 0)
			num_giga_ops = size*1000.0/MEGAFLOPS;
		else if (mode == 1)
			num_giga_ops = size*size*size*1.0/(MEGABYTES*1024.0);
		double throughput = num_giga_ops/elapsed_time_sec;
		printf("seed=%s mode=%s type=%s size=%d threads=%d time=%lf throughput=%lf checksum=%d\n",argv[1],argv[2],argv[3],size,num_threads,elapsed_time_sec,throughput,checksum);
 
    }

    return 0;
}