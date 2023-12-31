#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include "matrix.h"
#include "layer.h"

typedef struct MLNN_TypeDef
{
    uint32_t n_layers;
    Layer_t *layers;
} ML_NeuralNetwork_t;

typedef struct MLNN_TrainDatasetTypeDef
{
    uint32_t quantity;
    Matrix_t *inputs;
    Matrix_t *expected_outputs;
} MLNN_TrainDataset_t;

void AIC_MLNN_Create(uint32_t n_layers, ML_NeuralNetwork_t *mlnn);
void AIC_MLNN_Destroy(ML_NeuralNetwork_t *mlnn);

void AIC_MLNN_Print(ML_NeuralNetwork_t *mlnn);

void AIC_MLNN_SetInputLayer(uint32_t inputs, uint32_t layer_size, ActivationFunction_t activation_function, ML_NeuralNetwork_t *mlnn);
void AIC_MLNN_SetHiddenLayer(uint32_t layer_number, uint32_t layer_size, ActivationFunction_t activation_function, ML_NeuralNetwork_t *mlnn);

void AIC_MLNN_Predict(Matrix_t *input, Matrix_t *prediction, ML_NeuralNetwork_t *mlnn);
void AIC_MLNN_TrainDataset(MLNN_TrainDataset_t dataset, uint8_t shuffle_data, Data_t learning_rate, uint32_t epochs, ML_NeuralNetwork_t *mlnn);
void AIC_MLNN_PredictSet(Matrix_t *input, Matrix_t *prediction, uint32_t quantity, ML_NeuralNetwork_t *mlnn);

void AIC_MLNN_TrainDatasetCreate(MLNN_TrainDataset_t * dataset, uint32_t size);

Data_t sigmoid(Data_t x);
Data_t dSigmoid(Data_t x);

#endif // NEURAL_NETWORK_H