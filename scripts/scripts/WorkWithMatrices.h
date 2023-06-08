#include <stdio.h>
#include <stdlib.h>

double randInRange(double min, double max)
{
    double random = (double)rand() / RAND_MAX;
    double range = max - min;
    return min + range * random;
}

double **randm(int rows, int cols)
{
    double **mT = (double **)malloc(sizeof(double *) * rows);
    int i, j;
    for (i = 0; i < rows; i++) {
        mT[i] = (double *)malloc(sizeof(double) * cols);
        for (j = 0; j < cols; j++)
            mT[i][j] = randInRange(0.0, 2.0);
    }
    return mT;
}

int **mulmr(int rows, int cols, double **mT, double n3, double n4)
{
    int **mA = (int **)malloc(sizeof(int *) * rows);
    double coefficient = 1.0 - n3 * 0.02 - n4 * 0.005 - 0.25;
    int i, j;
    for (i = 0; i < rows; i++) {
        mA[i] = (int *)malloc(sizeof(int) * cols);
        for (j = 0; j < cols; j++)
            mA[i][j] = (int)(mT[i][j] * coefficient);
    }
    return mA;
}

int **makeMSymmetric(int rows, int cols, int **mA)
{
    int **mS = (int **)malloc(sizeof(int *) * rows);
    int i, j;
    for (i = 0; i < rows; i++)
        mS[i] = (int *)calloc(cols, sizeof(int));
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (mA[i][j] == 1 || mA[j][i] == 1) mS[i][j] = mS[j][i] = 1;
        }
    }
    return mS;
}

void printDoubleM(int rows, int cols, double **m)
{
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%lf ", m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printIntM(int rows, int cols, int **m)
{
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void deleteDoubleM(int rows, double **m)
{
    int i;
    for (i = 0; i < rows; i++) free(m[i]);
    free(m);
}

void deleteIntM(int rows, int **m)
{
    int i;
    for (i = 0; i < rows; i++) free(m[i]);
    free(m);
}
