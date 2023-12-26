#include <stdio.h>
#include "matrix.h"
#include "serialize.h"

#include <stdlib.h>

int main(int argc, char **argv)
{
    Matrix_t prueba = {0};

    int load = 0;

    printf("1 for load, 0 for save: ");
    scanf("%d", &load);

    if (load)
    {
        FILE *fp = fopen("serial_matrix.data", "rb");

        AIC_FileMatrixLoad(fp, &prueba);

        fclose(fp);

        AIC_MatrixPrint(&prueba, 1);
    }
    else
    {
        AIC_TimeSeedRand();

        AIC_MatrixCreateRand(2, 3, &prueba);

        AIC_MatrixPrint(&prueba, 1);

        FILE *fp = fopen("serial_matrix.data", "wb");

        AIC_FileMatrixSave(fp, &prueba);

        fclose(fp);

    }

    

    return 0;
}