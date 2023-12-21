#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>

#define DATA_TYPE float

#define DEBUG 1

typedef struct MatrixTypeDef
{
    DATA_TYPE *data;
    uint32_t rows, cols;
} Matrix_t;

void AIC_TimeSeedRand(void);

uint32_t AIC_MatrixGetIndex(uint32_t x, uint32_t y, Matrix_t *m);
uint8_t AIC_MatrixSet(uint32_t x, uint32_t y, DATA_TYPE value, Matrix_t *m);
DATA_TYPE AIC_MatrixGet(uint32_t x, uint32_t y, Matrix_t *m);

void AIC_MatrixPrint(Matrix_t *m, uint8_t verbose);
void AIC_MatrixPrintf(Matrix_t *m, uint8_t verbose, const char *format);

uint8_t AIC_MatrixCreate(uint32_t cols, uint32_t rows, Matrix_t *m);
uint8_t AIC_MatrixCreateRand(uint32_t cols, uint32_t rows, Matrix_t *m);
void AIC_MatrixDestroy(Matrix_t * m);

uint8_t AIC_MatrixTraspose(Matrix_t *m, Matrix_t *t);
uint8_t AIC_MatrixAdd(Matrix_t *a, Matrix_t *b, Matrix_t *c);
uint8_t AIC_MatrixAddItself(Matrix_t *a, Matrix_t *b);
uint8_t AIC_MatrixSub(Matrix_t *a, Matrix_t *b, Matrix_t *c);
uint8_t AIC_MatrixSubItself(Matrix_t *a, Matrix_t *b);

#endif // MATRIX_H