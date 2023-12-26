#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "matrix.h"

#include <stdio.h>

/*
 *    @brief A function to save the data pointer of the serialization data of matrix
 *
 *    @param data Data double pointer. Needs to be double because it allocates the memory needed.
 *    @param matrix Matrix to be saved
 * 
 *    @result Void
 */
void AIC_SerializeMatrixSave(uint8_t ** data, Matrix_t * matrix);

/*
 *    @brief A function to load a matrix from the serialization data.
 *
 *    @param data Data pointer generated with AIC_SerializeMatrixSave
 *    @param matrix Matrix to be loaded. This function allocates the memory needed for the matrix.
 * 
 *    @result Void
 */
void AIC_SerializeMatrixLoad(uint8_t * data, Matrix_t * matrix);

/*
 *    @brief A function to save the serialization data of a matrix in a file.
 *
 *    @param fp Previusly opened (in write binary mode) file pointer.
 *    @param matrix Matrix to be saved
 * 
 *    @result Void
 */
void AIC_FileMatrixSave(FILE * fp, Matrix_t * matrix);

/*
 *    @brief A function to load a matrix from a file.
 *
 *    @param fp Previusly opened (in read binary mode) file pointer.
 *    @param matrix Matrix to be loaded. This function allocates the memory needed for the matrix.
 * 
 *    @result Void
 */
void AIC_FileMatrixLoad(FILE * fp, Matrix_t * matrix);

#endif // SERIALIZE_H