#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "omp.h"


int main() 
{

	int N = 8192;
	int *A[N];
	int NumberOfThreads = 4;
	int SizeOfChunk = 4;
	int i, j, temp = 0;

	// Fill up all the indices of A
	for (int j = 0; j < N; j++)
	{
		A[j] = malloc(N*sizeof(*A[j]));
		for (int i = 0; i < N; i++)
		{
			A[j][i] =(N * i) + j;
		}
	}

/*
	printf("\n Printing the intilialised array \n");
	for(int i = 0; i < N; i++)  
	{
		for(int j = 0; j < N; j++) 
		{
			printf(" %d", A[i][j]);
		}
		printf("\n");
	}
*/

	omp_set_num_threads(NumberOfThreads);
	double StartOpenMP = omp_get_wtime();

	// Loop through the top left quadrant of the array
	#pragma omp parallel shared(N, A,SizeOfChunk) private(temp, i, j)
	{
		#pragma omp for schedule(dynamic, SizeOfChunk)  nowait
		for (i = 0; i < N / 2 - 1; i++) 
		{
			for (j = i + 1; j < N / 2; j++) 
			{
				temp = A[i][j];
				A[i][j] = A[j][i];
				A[j][i] = temp;
			}
		}
	}

	// Loop through the bottom right quadrant of the array
	#pragma omp parallel shared(N, A,SizeOfChunk) private(temp, i, j)
	{
		#pragma omp for schedule(dynamic, SizeOfChunk)  nowait
		for (i = N / 2; i < N - 1; i++) 
		{
			for (j = i + 1; j < N; j++) 
			{
				temp = A[i][j];
				A[i][j] = A[j][i];
				A[j][i] = temp;
			}
		}
	}

	//Loop through the top right and bottom left quadrants of the array 
	#pragma omp parallel shared(N, A,SizeOfChunk) private(temp, i, j)
	{
		#pragma omp for schedule(dynamic, SizeOfChunk)  nowait
		for (i = N / 2; i < N; i++) 
		{
			for (j = 0; j < N / 2; j++) 
			{
				temp = A[i][j];
				A[i][j] = A[j][i];
				A[j][i] = temp;
			}
		}
	}

	double StopOpenMP = omp_get_wtime() - StartOpenMP;
/*
	printf("\n Printing the transposed array \n");
	for(int i = 0; i < N; i++)  
	{
		for(int j = 0; j < N; j++) 
		{
			printf(" %d", A[i][j]);
		}
		printf("\n");
	}

*/
	double StartNaive = omp_get_wtime();

	for(int i = 0; i < N; i++) 
	{
		for(int j = 0; j < i; j++) 
		{
			temp = A[i][j];
			A[i][j] = A[j][i];
			A[j][i] = temp;

		}
	}

	double StopNaive = omp_get_wtime() - StartNaive;
/*
        printf("\n Printing the re-transposed array \n");
	for(int i = 0; i < N; i++)  
	{
		for(int j = 0; j < N; j++) 
		{
			printf(" %d", A[i][j]);
		}
		printf("\n");
	}
*/
	// frees all the memory in the matrix A
    for(i = 0; i < N; i++) 
	{ 
		free(A[i]);
    }

	//Print Time taken for Parallel and Naive Approach
	printf("OpenMP Parallel Time: %lf\n", StopOpenMP);
    printf("Naive Time: %lf\n", StopNaive);
    return 0;
}
