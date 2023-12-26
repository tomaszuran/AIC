#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define INDEX(row, col, matrix) (col * matrix->rows + row)

void AIC_TimeSeedRand(void)
{
    srand(time(NULL));
}

uint8_t AIC_MatrixSet(uint32_t row, uint32_t col, Data_t value, Matrix_t *m)
{
    if (DEBUG)
    {
        if (row >= m->rows || col >= m->cols)
        {

            fprintf(stderr, "AIC_MatrixSet out of bounds error.\n");
            return 0;
        }
    }

    m->data[INDEX(row, col, m)] = value;

    return 1;
}
Data_t AIC_MatrixGet(uint32_t row, uint32_t col, Matrix_t *m)
{
    if (DEBUG)
    {
        if (col >= m->cols || row >= m->rows)
        {

            fprintf(stderr, "AIC_MatrixGet out of bounds error.\n");
            return 0;
        }
    }

    return m->data[INDEX(row, col, m)];
}

void AIC_MatrixPrint(Matrix_t *m, uint8_t verbose)
{
    printf("[Matrix %dx%d]\n", m->rows, m->cols);
    if (verbose == 0)
        return;
    if (verbose == 1)
    {
        printf("[");
        for (uint32_t row = 0; row < m->rows; row++)
        {
            printf("[");
            for (uint32_t col = 0; col < m->cols; col++)
            {
                if (col < (m->cols - 1))
                    printf("%f, ", AIC_MatrixGet(row, col, m));
                else
                    printf("%f", AIC_MatrixGet(row, col, m));
            }
            if (row < m->rows - 1)
                printf("],\n");
            else
                printf("]");
        }
        printf("]\n");
    }
    if (verbose == 2)
    {
        printf("[");
        uint32_t row = 0;
        printf("[");
        for (uint32_t col = 0; col < m->cols; col++)
        {
            if (col < (m->cols - 1))
                printf("%f, ", AIC_MatrixGet(row, col, m));
            else
                printf("%f", AIC_MatrixGet(row, col, m));
        }
        printf("],\n");

        printf("...\t...\n");
        row = m->rows - 1;

        for (uint32_t col = 0; col < m->cols; col++)
        {
            if (col < (m->cols - 1))
                printf("%f, ", AIC_MatrixGet(row, col, m));
            else
                printf("%f", AIC_MatrixGet(row, col, m));
        }
        printf("]");

        printf("]\n");
    }
}
void AIC_MatrixPrintf(Matrix_t *m, uint8_t verbose, const char *format)
{
    printf("[Matrix %dx%d]\n", m->rows, m->cols);
    if (verbose == 0)
        return;

    char *aux = malloc(strlen(format) + 5);

    strcpy(aux, format);
    strcat(aux, ", ");

    if (verbose == 1)
    {
        printf("[");
        for (uint32_t row = 0; row < m->rows; row++)
        {
            printf("[");
            for (uint32_t col = 0; col < m->cols; col++)
            {
                if (col < (m->cols - 1))
                    printf(aux, AIC_MatrixGet(row, col, m));
                else
                    printf(format, AIC_MatrixGet(row, col, m));
            }
            if (row < m->rows - 1)
                printf("],\n");
            else
                printf("]");
        }
        printf("]\n");
    }
    if (verbose == 2)
    {
        printf("[");
        uint32_t row = 0;
        printf("[");
        for (uint32_t col = 0; col < m->cols; col++)
        {
            if (col < (m->cols - 1))
                printf(aux, AIC_MatrixGet(row, col, m));
            else
                printf(format, AIC_MatrixGet(row, col, m));
        }
        printf("],\n");

        printf("...\t...\n");
        row = m->rows - 1;

        for (uint32_t col = 0; col < m->cols; col++)
        {
            if (col < (m->cols - 1))
                printf(aux, AIC_MatrixGet(row, col, m));
            else
                printf(format, AIC_MatrixGet(row, col, m));
        }
        printf("]");

        printf("]\n");
    }

    free(aux);
}

uint8_t AIC_MatrixCreate(uint32_t rows, uint32_t cols, Matrix_t *m)
{
    if (m->data)
        AIC_MatrixDestroy(m);

    m->rows = rows;
    m->cols = cols;

    m->data = calloc(rows * cols, sizeof(Data_t));

    if (DEBUG)
    {
        if (!m->data)
        {

            fprintf(stderr, "AIC_MatrixCreate calloc error.\n");
            return 0;
        }
    }
    return 1;
}
uint8_t AIC_MatrixCreateRand(uint32_t rows, uint32_t cols, Matrix_t *m)
{
    uint8_t r = AIC_MatrixCreate(rows, cols, m);

    for (uint32_t i = 0; i < (rows * cols); i++)
    {
        m->data[i] = (Data_t)rand() / RAND_MAX;
    }

    return r;
}
void AIC_MatrixDestroy(Matrix_t *m)
{
    free(m->data);
}

uint8_t AIC_MatrixTraspose(Matrix_t *m, Matrix_t *t)
{
    uint8_t r = AIC_MatrixCreate(m->cols, m->rows, t);

    for (uint32_t col = 0; col < t->cols; col++)
    {
        for (uint32_t row = 0; row < t->rows; row++)
            AIC_MatrixSet(row, col, AIC_MatrixGet(col, row, m), t);
    }

    return r;
}
uint8_t AIC_MatrixAdd(Matrix_t *a, Matrix_t *b, Matrix_t *c)
{
    if (DEBUG)
    {
        if (a->cols != b->cols && a->rows != b->rows)
        {
            fprintf(stderr, "AIC_MatrixAdd dimension error.\n");
            return 0;
        }
    }

    uint8_t r = AIC_MatrixCreate(a->rows, a->cols, c);

    for (uint32_t i = 0; i < (a->cols * a->rows); i++)
        c->data[i] = a->data[i] + b->data[i];

    return r;
}
uint8_t AIC_MatrixAddItself(Matrix_t *a, Matrix_t *b)
{
    if (DEBUG)
    {
        if (a->cols != b->cols && a->rows != b->rows)
        {
            fprintf(stderr, "AIC_MatrixAddItself dimension error.\n");
            return 0;
        }
    }
    for (uint32_t i = 0; i < (a->cols * a->rows); i++)
        a->data[i] += b->data[i];

    return 1;
}
uint8_t AIC_MatrixSub(Matrix_t *a, Matrix_t *b, Matrix_t *c)
{
    if (DEBUG)
    {
        if (a->cols != b->cols && a->rows != b->rows)
        {
            fprintf(stderr, "AIC_MatrixSub dimension error.\n");
            return 0;
        }
    }

    uint8_t r = AIC_MatrixCreate(a->rows, a->cols, c);

    for (uint32_t i = 0; i < (a->cols * a->rows); i++)
        c->data[i] = a->data[i] - b->data[i];

    return r;
}
uint8_t AIC_MatrixSubItself(Matrix_t *a, Matrix_t *b)
{
    if (DEBUG)
    {
        if (a->cols != b->cols && a->rows != b->rows)
        {
            fprintf(stderr, "AIC_MatrixSubItself dimension error.\n");
            return 0;
        }
    }

    for (uint32_t i = 0; i < (a->cols * a->rows); i++)
        a->data[i] -= b->data[i];

    return 1;
}

uint8_t AIC_MatrixMultiplication(Matrix_t *a, Matrix_t *b, Matrix_t *c)
{
    if (DEBUG)
    {
        if (a->cols != b->rows)
        {
            fprintf(stderr, "AIC_MatrixMultiplication dimension error.\n");
            return 0;
        }
    }

    uint8_t r = AIC_MatrixCreate(a->rows, b->cols, c);

    for (uint32_t col = 0; col < c->cols; col++)
    {
        for (uint32_t row = 0; row < c->rows; row++)
        {
            Data_t total = 0;
            for (uint32_t k = 0; k < a->cols; k++)
            {
                total += AIC_MatrixGet(row, k, a) * AIC_MatrixGet(k, col, b);
            }
            AIC_MatrixSet(row, col, total, c);
        }
    }

    return r;
}

void AIC_MatrixAddScalar(Matrix_t *m, Data_t value)
{
    for (uint32_t i = 0; i < (m->cols * m->rows); i++)
        m->data[i] += value;
}

void AIC_MatrixMultiplyScalar(Matrix_t *m, Data_t value)
{
    for (uint32_t i = 0; i < (m->cols * m->rows); i++)
        m->data[i] *= value;
}

void AIC_MatrixCopy(Matrix_t *dest, Matrix_t *src)
{
    AIC_MatrixCreate(src->rows, src->cols, dest);

    for(uint32_t i = 0; i < src->rows * src->cols; i++)
        dest->data[i] = src->data[i];
}

void AIC_MatrixApplyFunction(Matrix_t *m, Data_t (*fun)(Data_t))
{
    for(uint32_t i = 0; i < m->rows * m->cols; i++)
        m->data[i] = (*fun)(m->data[i]);
}