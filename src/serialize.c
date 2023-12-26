#include "serialize.h"
#include <stdlib.h>

void AIC_SerializeMatrixSave(uint8_t **data, Matrix_t *matrix)
{
    *data = malloc(sizeof(uint32_t) * 2 + sizeof(DATA_TYPE) * matrix->cols * matrix->rows);

    *((uint32_t *)*data) = matrix->cols;

    *((uint32_t *)*data + 1) = matrix->rows;

    for (uint32_t i = 0; i < (matrix->cols * matrix->rows * sizeof(DATA_TYPE)); ++i)
    {
        *(*data + 8 + i) = *((uint8_t *)matrix->data + i);
    }
}

void AIC_SerializeMatrixLoad(uint8_t *data, Matrix_t *matrix)
{
    matrix->cols = *((uint32_t *)data);

    matrix->rows = *((uint32_t *)data + 1);

    matrix->data = calloc(matrix->rows * matrix->cols, sizeof(DATA_TYPE));

    for (uint32_t i = 0; i < (matrix->cols * matrix->rows * sizeof(DATA_TYPE)); ++i)
    {
        *(((uint8_t *)matrix->data) + i) = *(data + 8 + i);
    }
}

void AIC_FileMatrixSave(FILE *fp, Matrix_t *matrix)
{
    fwrite(&matrix->cols, sizeof(uint32_t), 1, fp);
    fwrite(&matrix->rows, sizeof(uint32_t), 1, fp);
    fwrite(matrix->data, sizeof(DATA_TYPE), matrix->cols * matrix->rows, fp);
}
void AIC_FileMatrixLoad(FILE *fp, Matrix_t *matrix)
{
    fread(&matrix->cols, sizeof(uint32_t), 1, fp);
    fread(&matrix->rows, sizeof(uint32_t), 1, fp);
    
    matrix->data = calloc(matrix->rows * matrix->cols, sizeof(DATA_TYPE));

    fread(matrix->data, sizeof(DATA_TYPE), matrix->rows * matrix->cols, fp);
}