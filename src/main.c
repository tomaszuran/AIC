#include <stdio.h>
#include "matrix.h"

int main(int argc, char **argv)
{
    Matrix_t a = {0}, b = {0}, c = {0}, d = {0};
    AIC_TimeSeedRand();
    AIC_MatrixCreateRand(2, 10000, &a);
    AIC_MatrixCreateRand(10000, 3, &b);

    AIC_MatrixPrint(&a, 0);
    AIC_MatrixPrint(&b, 0);

    AIC_MatrixMultiplication(&a, &b, &c);

    AIC_MatrixPrint(&c, 1);

    AIC_MatrixTraspose(&c, &d);

    AIC_MatrixPrint(&d, 1);

    return 0;
}