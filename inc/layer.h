#ifndef LAYER_H
#define LAYER_H

#include "matrix.h"

typedef Data_t (*ActivationFunction_t)(Data_t);

/*
typedef enum {
    SIGMOID,
    RELU
} ActivationFunctionNumber_t;
*/
typedef struct LayerTypeDef
{
    uint32_t n_in, layer_size;
    ActivationFunction_t activation_function;
    Matrix_t weights, bias, output;
} Layer_t;



void AIC_LayerCreate(uint32_t inputs, uint32_t layer_size, Layer_t * layer, ActivationFunction_t activation_function);
void AIC_LayerDestroy(Layer_t * layer);

void AIC_LayerGenerateOutput(Matrix_t * input, Layer_t * layer);

#endif // LAYER_H