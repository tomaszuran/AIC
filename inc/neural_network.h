#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include "matrix.h"
#include "layer.h"

typedef struct MLNN_TypeDef
{
    uint32_t n_layers;
    Layer_t * layers;
} ML_NeuralNetwork_t;

void AIC_MLNN_Create(uint32_t n_layers, ML_NeuralNetwork_t * mlnn);
void AIC_MLNN_Destroy(ML_NeuralNetwork_t * mlnn);

void AIC_MLNN_SetInputLayer(uint32_t inputs, uint32_t layer_size, ActivationFunction_t activation_function, ML_NeuralNetwork_t *mlnn);
void AIC_MLNN_SetHiddenLayer(uint32_t layer_number, uint32_t layer_size, ActivationFunction_t activation_function, ML_NeuralNetwork_t *mlnn);

void AIC_MLNN_Predict(Matrix_t * input, Matrix_t * prediction, ML_NeuralNetwork_t * mlnn);

Data_t sigmoid(Data_t x);
Data_t dSigmoid(Data_t x);

#endif // NEURAL_NETWORK_H