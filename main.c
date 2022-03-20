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

	if (argv[1][0] == 'S' && argc == 3)
	{
		SIZE = atoi(argv[2]);
		mmm_args args;
		args.one = mmm_create(SIZE);
		args.two = mmm_create(SIZE);
		args.prod = mmm_create(SIZE);
		args.SIZE = SIZE;
		mmm_rowInit(SIZE, args.one);
		mmm_colInit(SIZE, args.two);
		mmm_seq(&args);
	}
	else if (argv[1][0] == 'P' && argc == 4)
	{
		SIZE = atoi(argv[3]);
		mmm_args margs;
		margs.one = mmm_create(SIZE);
		margs.two = mmm_create(SIZE);
		margs.prod = mmm_create(SIZE);
		margs.SIZE = SIZE;
		mmm_rowInit(SIZE, margs.one);
		mmm_colInit(SIZE, margs.two);

		int numTh = atoi(argv[2]);
		if (numTh >= SIZE)
		{
			numTh = SIZE - 1;
		}
		int sec = SIZE / numTh;
		margs.end = sec;
		pthread_t tid[numTh];

		for (int i = 0; i < numTh; i++)
		{
			margs.tNum = i;

			pthread_create(&tid[i], NULL, mmm_par, &margs);

			margs.start = margs.end;
			margs.end = sec * (i + 2);
		}
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
