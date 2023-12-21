#include <stdio.h>
#include "matrix.h"

int main(int argc, char **argv)
{
    Matrix_t m, t;
    
    AIC_MatrixCreate(3, 5, &m);

    AIC_MatrixSet(2, 1, 5, &m);

    AIC_MatrixPrintf(&m, 1, "%1.0f");

    AIC_MatrixTraspose(&m, &t);

    AIC_MatrixPrintf(&t, 1, "%1.0f");
    return 0;
}