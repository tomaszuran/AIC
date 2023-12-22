#include <stdio.h>
#include "matrix.h"

int main(int argc, char **argv)
{
    Matrix_t m = {0}, t = {0}, a = {0}, b = {0};
    AIC_TimeSeedRand();
    AIC_MatrixCreateRand(5, 3, &m);
    AIC_MatrixCreate(5, 3, &a);

    AIC_MatrixPrintf(&a, 1, "%1.2f");
    AIC_MatrixPrintf(&m, 1, "%1.2f");

    AIC_MatrixAddItself(&a, &m);

    AIC_MatrixPrintf(&a, 1, "%1.2f");
    AIC_MatrixPrintf(&m, 1, "%1.2f");

    AIC_MatrixTraspose(&m, &t);

    return 0;
}