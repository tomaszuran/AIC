#include "neural_network.h"

#include <stdlib.h>
#include <math.h>

void AIC_MLNN_Create(uint32_t n_layers, ML_NeuralNetwork_t *mlnn)
{
    mlnn->n_layers = n_layers;

    mlnn->layers = calloc(n_layers, sizeof(Layer_t));
}

void AIC_MLNN_Destroy(ML_NeuralNetwork_t *mlnn)
{
    for (uint32_t i = 0; i < mlnn->n_layers; i++)
    {
        AIC_LayerDestroy(&(mlnn->layers[i]));
    }

    free(mlnn->layers);
}

void AIC_MLNN_SetInputLayer(uint32_t inputs, uint32_t layer_size, ActivationFunction_t activation_function, ML_NeuralNetwork_t *mlnn)
{
    AIC_LayerCreate(inputs, layer_size, &(mlnn->layers[0]), activation_function);
}

void AIC_MLNN_SetHiddenLayer(uint32_t layer_number, uint32_t layer_size, ActivationFunction_t activation_function, ML_NeuralNetwork_t *mlnn)
{
    AIC_LayerCreate(mlnn->layers[0].output.cols, layer_size, &(mlnn->layers[layer_number]), activation_function);
}

void AIC_MLNN_Predict(Matrix_t *input, Matrix_t *prediction, ML_NeuralNetwork_t *mlnn)
{
    AIC_LayerGenerateOutput(input, &mlnn->layers[0]);

    for (uint32_t i = 1; i < mlnn->n_layers; i++)
        AIC_LayerGenerateOutput(&mlnn->layers[i - 1].output, &mlnn->layers[i]);

    AIC_MatrixCopy(prediction, &mlnn->layers[mlnn->n_layers - 1].output);
}

Data_t sigmoid(Data_t x)
{
    return 1 / (1 + exp(-x));
}

Data_t dSigmoid(Data_t x)
{
    return x * (1 - x); 
}