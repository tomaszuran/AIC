#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "matrix.h"

#include <stdio.h>

void AIC_SerializeMatrixSave(uint8_t ** data, Matrix_t * matrix);
void AIC_SerializeMatrixLoad(uint8_t * data, Matrix_t * matrix);

void AIC_FileMatrixSave(FILE * fp, Matrix_t * matrix);
void AIC_FileMatrixLoad(FILE * fp, Matrix_t * matrix);

#endif // SERIALIZE_H