#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include "rng.h"

#define DEBUG 1

typedef float Data_t;


/*
 *    @brief Matrix data structure.
 *
 *    The data memory is not allocated here. It needs to be allocated via AIC_MatrixCreate or AIC_MatrixCreateRand
 */
typedef struct MatrixTypeDef
{
    Data_t *data;
    uint32_t rows, cols;
} Matrix_t;

/*
 *    @brief A function to set a matrix value

 *    @param row Row number
 *    @param col Column number
 *    @param value Value to be set
 *    @param m Matrix to be modified 
 *
 *    @result Open succesfull operation '1', otherwise '0' (only in debug mode)
 */
uint8_t AIC_MatrixSet(uint32_t row, uint32_t col, Data_t value, Matrix_t *m);

/*
 *    @brief A function to get a matrix value

 *    @param row Row number
 *    @param col Column number
 *    @param m Matrix to be modified 
 *
 *    @result Open succesfull operation return the value stores in (x, y), otherwise '0'
 */
Data_t AIC_MatrixGet(uint32_t row, uint32_t col, Matrix_t *m);

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
uint8_t AIC_MatrixCreate(uint32_t rows, uint32_t cols, Matrix_t *m);

/*
 *    @brief A function to allocate memory for a matrix with rand (between 0 and 1) initial value
 *
 *    @param cols Number of columns
 *    @param rows Number of rows
 *    @param m Matrix to be allocated 
 *
 *    @result Open succesfull operation '1', otherwise '0' (only in debug mode)
 */
uint8_t AIC_MatrixCreateRand(uint32_t rows, uint32_t cols, Matrix_t *m);

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
 * 
 *    @result Open succesfull operation '1', otherwise '0' (only in debug mode)
 */
uint8_t AIC_MatrixAdd(Matrix_t *a, Matrix_t *b, Matrix_t *c);

/*
 *    @brief A function to add a matrix to an existing one
 *
 *    @param a Addend (and result) matrix
 *    @param b Addend matrix
 * 
 *    @result Open succesfull operation '1', otherwise '0' (only in debug mode)
 */
uint8_t AIC_MatrixAddItself(Matrix_t *a, Matrix_t *b);

/*
 *    @brief A function to calculate a matrix subtraction
 *
 *    @param a Minuend matrix
 *    @param b Subtrahend matrix
 *    @param c Result matrix
 * 
 *    @result Open succesfull operation '1', otherwise '0' (only in debug mode)
 */
uint8_t AIC_MatrixSub(Matrix_t *a, Matrix_t *b, Matrix_t *c);

/*
 *    @brief A function to subtract a matrix to an existing one
 *
 *    @param a Minuend (and result) matrix
 *    @param b Subtrahend matrix
 * 
 *    @result Open succesfull operation '1', otherwise '0' (only in debug mode)
 */
uint8_t AIC_MatrixSubItself(Matrix_t *a, Matrix_t *b);

/*
 *    @brief A function to calculate a matrix multiplication. A x B = C
 *
 *    @param a Multiplicated matrix
 *    @param b Multiplicated matrix
 *    @param c Result matrix
 * 
 *    @result Open succesfull operation '1', otherwise '0' (only in debug mode)
 */
uint8_t AIC_MatrixMultiplication(Matrix_t *a, Matrix_t *b, Matrix_t *c);

/*
 *    @brief A function to add a scalar number to every item in the matrix.
 *
 *    @param m Matrix to be modified
 *    @param value Scalar number
 * 
 *    @result Void
 */
void AIC_MatrixAddScalar(Matrix_t *m, Data_t value);

/*
 *    @brief A function to multiplicate every item in the matrix a times the same item in the matrix b (and stores it in a)
 *
 *    @param a Original matrix
 *    @param b Multiplicated matrix
 * 
 *    @result Void
 */
void AIC_MatrixMultiplyItself(Matrix_t *a, Matrix_t *b);


/*
 *    @brief A function to multiplicate every item in the matrix a times the same item in the matrix b (and stores it in a)
 *
 *    @param a Multiplicated matrix
 *    @param b Multiplicated matrix
 *    @param c Result matrix
 * 
 *    @result Open succesfull operation '1', otherwise '0' (only in debug mode)
 */
uint8_t AIC_MatrixMultiplyElements(Matrix_t *a, Matrix_t *b, Matrix_t *c);

/*
 *    @brief A function to multiply a scalar number to every item in the matrix.
 *
 *    @param m Matrix to be modified
 *    @param value Scalar number
 * 
 *    @result Void
 */
void AIC_MatrixMultiplyScalar(Matrix_t *m, Data_t value);

/*
 *    @brief A function to copy a matrix
 *
 *    @param dest Coppied matrix
 *    @param src Original matrix
 * 
 *    @result Void
 */
void AIC_MatrixCopy(Matrix_t *dest, Matrix_t *src);

/*
 *    @brief A function apply a function to every element of the matrix
 *
 *    @param m Matrix to be moddified
 *    @param fun Function (pointer) to be applied. The prototipe must be Data_t fun(Data_t)
 * 
 *    @result Void
 */
void AIC_MatrixApplyFunction(Matrix_t *m, Data_t (*fun)(Data_t));

/*
 *    @brief A function to get the total sum of the elements of the matrix
 *
 *    @param m Matrix
 * 
 *    @result Sum of the elements
 */
Data_t AIC_MatrixGetSum(Matrix_t *m);


uint32_t AIC_MatrixLoadFromCSV(char *filename, Matrix_t *matrix);
uint8_t AIC_MatrixSaveToCSV(char *filename, Matrix_t *matrix, uint32_t quantity);

#endif // MATRIX_H