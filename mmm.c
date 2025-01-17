#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "mmm.h"

/**
 * Allocate and initialize the matrices on the heap. Populate
 * the input matrices with random integers from 0 to 99
 */

void mmm_colInit(int SIZE, double **matrix)
{

	int j;
	for (int i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			matrix[i][j] = rand() % 100;
		}
	}
}
void mmm_rowInit(int SIZE, double **matrix)
{
	int j;
	for (int i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			matrix[j][i] = rand() % 100;
		}
	}
}

/**
 * Reset a given matrix to zeroes
 * @param matrix pointer to a 2D array
 */
void mmm_reset(double **matrix, int SIZE)
{
	for (int i = 0; i < SIZE; i++)
	{
		memset(matrix[i], 0, SIZE * sizeof(double *));
	}
}

/**
 * Free up memory allocated to all matrices
 */
void mmm_freeup(mmm_args *margs)
{

	free(margs->one);
	free(margs->two);
	free(margs->prod);
	free(margs->ver);
}
/**
 * mallocs a matrix
 **/
double **mmm_create(int SIZE)
{
	double **matrix = (double **)malloc(SIZE * sizeof(double *));
	for (int i = 0; i < SIZE; i++)
	{
		matrix[i] = (double *)malloc(SIZE * sizeof(double));
	}
	return matrix;
}
/**
 * Sequential MMM
 */
void mmm_seq(mmm_args *margs)
{
	for (int i = 0; i < margs->SIZE; i++)
	{ // row
		for (int j = 0; j < margs->SIZE; j++)
		{ // col
			int temp = 0;
			for (int k = 0; k < margs->SIZE; k++)
			{
				temp += (margs->one[i][k] * margs->two[j][k]); // remember two is rotated
			}
			margs->prod[i][j] = temp;
		}
	}
}

/**
 * Verifies the correctness between the matrices generated by
 * the sequential run and the parallel run.
 *
 * @return the largest error between two corresponding elements
 * in the result matrices
 */
void mmm_verify(mmm_args *margs)
{
	// mmm_args *margs = args;
	int size = margs->SIZE;
	for (int i = 0; i < size; i++)
	{ // row
		for (int j = 0; j < size; j++)
		{ // col
			int temp = 0;
			for (int k = 0; k < size; k++)
			{
				temp += (margs->one[i][k] * margs->two[j][k]); // remember two is rotated
			}
			margs->ver[i][j] = temp;
		}
	}

	int greatestDiff = 0;
	for (int i = 0; i < size; i++)
	{ // row
		for (int j = 0; j < size; j++)
		{ // col
			int diff = 0;

			diff = margs->prod[i][j] - margs->ver[i][j];
			if (diff > greatestDiff)
			{
				greatestDiff = diff;
			}
		}
	}
	printf("Verifying... largest error between parallel and sequential matrix: %d\n", greatestDiff);
	return;
}

/**
 * Parallel MMM
 */
void *mmm_par(void *args)
{

	mmm_args *margs = (mmm_args *)args;

	int i, j, k;

	for (i = margs->tNum; i < margs->SIZE; i += margs->numTh)
	{ // row
		for (j = 0; j < margs->SIZE; j++)
		{ // col
			int temp = 0;
			for (k = 0; k < margs->SIZE; k++)
			{
				temp += (margs->one[i][k] * margs->two[j][k]); // remember two is rotated
			}
			margs->prod[i][j] = temp;
		}
	}

	return NULL;
}
