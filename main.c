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
	}
	else if (argv[1][0] == 'P' && argc == 4)
	{
		SIZE = atoi(argv[3]);
		margs.one = mmm_create(SIZE);
		margs.two = mmm_create(SIZE);
		margs.prod = mmm_create(SIZE);
		margs.ver = mmm_create(SIZE);
		margs.SIZE = SIZE;
		mmm_rowInit(SIZE, margs.one);
		mmm_colInit(SIZE, margs.two);
		margs.start = (int *)malloc(margs.tNum * sizeof(int));
		margs.end = (int *)malloc(margs.tNum * sizeof(int));

		int numTh = atoi(argv[2]);
		if (numTh >= SIZE)
		{
			numTh = SIZE - 1;
		}
		int sec = SIZE / numTh;
		margs.end[0] = sec;
		pthread_t tid[numTh];
		int i;
		for (i = 0; i < numTh; i++)
		{
			margs.tNum = i;

			pthread_create(&tid[i], NULL, mmm_par, &margs);

			margs.start[i] = margs.end[i];
			margs.end[i] = sec * (i + 2);
		}

		for (i = 0; i < numTh; i++)
		{
			pthread_join(tid[i], NULL);
		}
		mmm_verify(&margs);
		free(margs.start);
		free(margs.end);
	}
	else
	{
		printf("error, incorrect args");
	}

	// end: stuff I want to clock

	clockend = rtclock(); // stop clocking
	printf("Time taken: %.6f sec\n", (clockend - clockstart));
	mmm_freeup(&margs);
	return 0;
}
