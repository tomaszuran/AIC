#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

uint32_t AIC_MatrixGetIndex(uint32_t x, uint32_t y, Matrix_t *m)
{
    return y * m->cols + x;
}
uint8_t AIC_MatrixSet(uint32_t x, uint32_t y, DATA_TYPE value, Matrix_t *m)
{
    if (DEBUG)
    {
        if (x >= m->cols || y >= m->rows)
        {

            fprintf(stderr, "AIC_MatrixSet out of bounds error.");
            return 0;
        }
    }

    m->data[y * m->cols + x] = value;

    return 1;
}
DATA_TYPE AIC_MatrixGet(uint32_t x, uint32_t y, Matrix_t *m)
{
    if (DEBUG)
    {
        if (x >= m->cols || y >= m->rows)
        {

            fprintf(stderr, "AIC_MatrixGet out of bounds error.");
            return 0;
        }
    }

    return m->data[y * m->cols + x];
}

void AIC_MatrixPrint(Matrix_t *m, uint8_t verbose)
{
    printf("[Matrix %dx%d]\n", m->cols, m->rows);
    if (verbose == 0)
        return;
    if (verbose == 1)
    {
        printf("[");
        for (uint32_t y = 0; y < m->rows; y++)
        {
            printf("[");
            for (uint32_t x = 0; x < m->cols; x++)
            {
                if (x < (m->cols - 1))
                    printf("%f, ", AIC_MatrixGet(x, y, m));
                else
                    printf("%f", AIC_MatrixGet(x, y, m));
            }
            if (y < m->rows - 1)
                printf("],\n");
            else
                printf("]");
        }
        printf("]\n");
    }
    if (verbose == 2)
    {
        printf("[");
        uint32_t y = 0;
        printf("[");
        for (uint32_t x = 0; x < m->cols; x++)
        {
            if (x < (m->cols - 1))
                printf("%f, ", AIC_MatrixGet(x, y, m));
            else
                printf("%f", AIC_MatrixGet(x, y, m));
        }
        printf("],\n");

        printf("...\t...\n");
        y = m->rows - 1;

        for (uint32_t x = 0; x < m->cols; x++)
        {
            if (x < (m->cols - 1))
                printf("%f, ", AIC_MatrixGet(x, y, m));
            else
                printf("%f", AIC_MatrixGet(x, y, m));
        }
        printf("]");

        printf("]\n");
    }
}
void AIC_MatrixPrintf(Matrix_t *m, uint8_t verbose, const char *format)
{

    printf("[Matrix %dx%d]\n", m->cols, m->rows);
    if (verbose == 0)
        return;

    char *aux = malloc(strlen(format) + 5);

    strcpy(aux, format);
    strcat(aux, ", ");

    if (verbose == 1)
    {
        printf("[");
        for (uint32_t y = 0; y < m->rows; y++)
        {
            printf("[");
            for (uint32_t x = 0; x < m->cols; x++)
            {
                if (x < (m->cols - 1))
                    printf(aux, AIC_MatrixGet(x, y, m));
                else
                    printf(format, AIC_MatrixGet(x, y, m));
            }
            if (y < m->rows - 1)
                printf("],\n");
            else
                printf("]");
        }
        printf("]\n");
    }
    if (verbose == 2)
    {
        printf("[");
        uint32_t y = 0;
        printf("[");
        for (uint32_t x = 0; x < m->cols; x++)
        {
            if (x < (m->cols - 1))
                printf(aux, AIC_MatrixGet(x, y, m));
            else
                printf(format, AIC_MatrixGet(x, y, m));
        }
        printf("],\n");

        printf("...\t...\n");
        y = m->rows - 1;

        for (uint32_t x = 0; x < m->cols; x++)
        {
            if (x < (m->cols - 1))
                printf(aux, AIC_MatrixGet(x, y, m));
            else
                printf(format, AIC_MatrixGet(x, y, m));
        }
        printf("]");

        printf("]\n");
    }

    free(aux);
}

uint8_t AIC_MatrixCreate(uint32_t rows, uint32_t cols, Matrix_t *m)
{
    AIC_MatrixDestroy(m);

    m->rows = rows;
    m->cols = cols;

    m->data = calloc(rows * cols, sizeof(DATA_TYPE));

    if (DEBUG)
    {
        if (!m->data)
        {

            fprintf(stderr, "AIC_MatrixCreate calloc error.");
            return 0;
        }
    }
    return 1;
}
uint8_t AIC_MatrixCreateRand(uint32_t rows, uint32_t cols, Matrix_t *m)
{
    return AIC_MatrixCreate(rows, cols, m);
}
void AIC_MatrixDestroy(Matrix_t *m)
{
    free(m->data);
}

uint8_t AIC_MatrixTraspose(Matrix_t *m, Matrix_t *t)
{
    uint8_t r = AIC_MatrixCreate(m->cols, m->rows, t);

    for (uint32_t x = 0; x < t->cols; x++)
    {
        for (uint32_t y = 0; y < t->rows; y++)
            AIC_MatrixSet(x, y, AIC_MatrixGet(y, x, m), t);
    }

    return r;
}
uint8_t AIC_MatrixAdd(Matrix_t *a, Matrix_t *b, Matrix_t *c)
{
    if (DEBUG)
    {
        if (a->cols != b->cols && a->rows != b->rows)
        {
            fprintf(stderr, "AIC_MatrixAdd dimension error.");
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
            fprintf(stderr, "AIC_MatrixAddItself dimension error.");
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
            fprintf(stderr, "AIC_MatrixAdd dimension error.");
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
            fprintf(stderr, "AIC_MatrixAdd dimension error.");
            return 0;
        }
    }

    for (uint32_t i = 0; i < (a->cols * a->rows); i++)
        a->data[i] -= b->data[i];

    return 1;
}