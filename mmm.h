#ifndef MMM_H_
#define MMM_H_

// globals (anything here would be shared with all threads) */
// I would declare the pointers to the matrices here (i.e., double **A, **B, **C),
// as well as the size of the matrices, etc.

typedef struct
{
    int SIZE;
    int tNum;
    int *start;
    int *end;
    double **one;
    double **two;
    double **prod;
    double **ver;
} mmm_args;

void mmm_colInit(int SIZE, double **matrix);
void mmm_rowInit(int SIZE, double **matrix);
void mmm_reset(double **matrix, int SIZE);
void mmm_freeup();
double **mmm_create(int SIZE);
void mmm_seq(mmm_args *margs);
void *mmm_par(void *args);
void mmm_verify(mmm_args *margs);
void mmm_verSeq(mmm_args *margs);
extern double **one;
extern double **two;
extern double **prod;

#endif /* MMM_H_ */
