#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

int SIZE;
double **one;
double **two;
double **prod;

int main(int argc, char *argv[])
{
	srand(time(NULL));
	double clockstart, clockend;
	clockstart = rtclock(); // start clocking

	// start: stuff I want to clock
	mmm_args margs;
	if (argv[1][0] == 'S' && argc == 3)
	{
		SIZE = atoi(argv[2]);

		margs.one = mmm_create(SIZE);
		margs.two = mmm_create(SIZE);
		margs.prod = mmm_create(SIZE);
		margs.SIZE = SIZE;
		mmm_rowInit(SIZE, margs.one);
		mmm_colInit(SIZE, margs.two);
		mmm_seq(&margs);

		mmm_freeup(&margs);
	}
	else if (argv[1][0] == 'P' && argc == 4)
	{
		SIZE = atoi(argv[3]);
		int numTh = atoi(argv[2]);
		double **one = mmm_create(SIZE);
		double **two = mmm_create(SIZE);
		double **prod = mmm_create(SIZE);
		double **ver = mmm_create(SIZE);
		mmm_rowInit(SIZE, one);
		mmm_colInit(SIZE, two);
		if (numTh >= SIZE)
		{
			numTh = SIZE - 1;
		}
		pthread_t tid[numTh];
		mmm_args margs[numTh];

		int i;

		for (i = 0; i < numTh; i++)
		{
			margs[i].one = one;
			margs[i].two = two;
			margs[i].prod = prod;
			margs[i].SIZE = SIZE;
			margs[i].ver = ver;
			margs[i].numTh = numTh;
			margs[i].tNum = i;

			pthread_create(&tid[i], NULL, mmm_par, &margs[i]);
		}
		for (i = 0; i < numTh; i++)
		{
			pthread_join(tid[i], NULL);
		}

		mmm_verify(&margs[0]);

		mmm_freeup(&margs[0]);
	}
	else
	{
		printf("error, incorrect args");
	}

	// end: stuff I want to clock

	clockend = rtclock(); // stop clocking
	printf("Time taken: %.6f sec\n", (clockend - clockstart));

	return 0;
}
