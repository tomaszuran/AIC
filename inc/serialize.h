#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "matrix.h"

uint8_t AIC_SerializeMatrixSave(uint8_t ** data, Matrix_t * matrix);
uint8_t AIC_SerializeMatrixLoad(uint8_t * data, Matrix_t * matrix);

#endif // SERIALIZE_H