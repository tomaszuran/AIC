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

/*
 *    @brief A function to seed the libc rng
 *
 *    @result Void
 */
void AIC_TimeSeedRand(void);

/*
 *    @brief A function to set a matrix value

 *    @param x Column number
 *    @param y Row number
 *    @param value Value to be set
 *    @param m Matrix to be modified 
 *
 *    @result Open succesfull operation '1', otherwise '0' (only in debug mode)
 */
uint8_t AIC_MatrixSet(uint32_t x, uint32_t y, DATA_TYPE value, Matrix_t *m);

/*
 *    @brief A function to get a matrix value

 *    @param x Column number
 *    @param y Row number
 *    @param m Matrix to be modified 
 *
 *    @result Open succesfull operation return the value stores in (x, y), otherwise '0'
 */
DATA_TYPE AIC_MatrixGet(uint32_t x, uint32_t y, Matrix_t *m);

/*
 *    @brief A function to print a matrix 
 *
 *    @param m Matrix to be printed
 *    @param verbose Verbose mode (0, 1, 2)
 *
 *
 *    @result Void
 */
void AIC_MatrixPrint(Matrix_t *m, uint8_t verbose);

/*
 *    @brief A function to print a matrix 
 *
 *    @param m Matrix to be printed
 *    @param verbose Verbose mode (0, 1, 2)
 *    @param format Formatted text for matrix value output 
 *
 *    @result Void
 */
void AIC_MatrixPrintf(Matrix_t *m, uint8_t verbose, const char *format);

/*
 *    @brief A function to allocate memory for a matrix with null initial value
 *
 *    @param cols Number of columns
 *    @param rows Number of rows
 *    @param m Matrix to be allocated 
 *
 *    @result Void
 */
uint8_t AIC_MatrixCreate(uint32_t cols, uint32_t rows, Matrix_t *m);

/*
 *    @brief A function to allocate memory for a matrix with rand (between 0 and 1) initial value
 *
 *    @param cols Number of columns
 *    @param rows Number of rows
 *    @param m Matrix to be allocated 
 *
 *    @result Open succesfull operation '1', otherwise '0' (only in debug mode)
 */
uint8_t AIC_MatrixCreateRand(uint32_t cols, uint32_t rows, Matrix_t *m);

/*
 *    @brief A function to allocate memory for a matrix with null initial value
 *
 *    @param cols Number of columns
 *    @param rows Number of rows
 *    @param m Matrix to be allocated 
 *
 *    @result Open succesfull operation '1', otherwise '0' (only in debug mode)
 */
void AIC_MatrixDestroy(Matrix_t *m);

/*
 *    @brief A function to deallocate a matrix from memory
 *
 *    @param m Matrix to be deallocated
 *
 *    @result Void
 */
uint8_t AIC_MatrixTraspose(Matrix_t *m, Matrix_t *t);

/*
 *    @brief A function to calculate a matrix addition
 *
 *    @param a Addend matrix
 *    @param b Addend matrix
 *    @param c Result matrix
 */
uint8_t AIC_MatrixAdd(Matrix_t *a, Matrix_t *b, Matrix_t *c);

/*
 *    @brief A function to add a matrix to an existing one
 *
 *    @param a Addend (and result) matrix
 *    @param b Addend matrix
 */
uint8_t AIC_MatrixAddItself(Matrix_t *a, Matrix_t *b);

/*
 *    @brief A function to calculate a matrix subtraction
 *
 *    @param a Minuend matrix
 *    @param b Subtrahend matrix
 *    @param c Result matrix
 */
uint8_t AIC_MatrixSub(Matrix_t *a, Matrix_t *b, Matrix_t *c);

/*
 *    @brief A function to subtract a matrix to an existing one
 *
 *    @param a Minuend (and result) matrix
 *    @param b Subtrahend matrix
 */
uint8_t AIC_MatrixSubItself(Matrix_t *a, Matrix_t *b);

#endif // MATRIX_H