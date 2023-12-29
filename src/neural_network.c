#include "neural_network.h"

#include <stdlib.h>
#include <stdio.h>
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
    AIC_LayerCreate(mlnn->layers[layer_number - 1].output.cols, layer_size, &(mlnn->layers[layer_number]), activation_function);
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

Data_t squared(Data_t x)
{
    return x * x;
}

Data_t loss_abs(Data_t x)
{
    return x >= 0 ? x : -x;
}

static void AIC_MLNN_GenerateErrors(Matrix_t *real_output, Matrix_t *expected_output, ML_NeuralNetwork_t *mlnn)
{
    Matrix_t deltaError = {0}, deltaOutput = {0}, t_weights = {0}, derivative = {0};

    //Error = MSE
    AIC_MatrixSub(expected_output, real_output, &deltaError);
    AIC_MatrixApplyFunction(&deltaError, squared);
    printf("Error: %f\n", AIC_MatrixGetSum(&deltaError));

    // deltaOutput = 2 * (expected - output) * dSigmoid(output)

    // deltaError = 2 * (expected - output)
    AIC_MatrixSub(expected_output, real_output, &deltaError);
    AIC_MatrixMultiplyScalar(&deltaError, 2.0);

    // derivative = dSigmoid(output)
    AIC_MatrixCopy(&derivative, real_output);
    AIC_MatrixApplyFunction(&derivative, dSigmoid);

    // deltaOutput = deltaError * dSigmoid(output)
    AIC_MatrixMultiplyElements(&deltaError, &derivative, &deltaOutput);

    // layer error = deltaOutput
    AIC_MatrixCopy(&(mlnn->layers[mlnn->n_layers - 1].error), &deltaOutput);

    for (int32_t i = mlnn->n_layers - 2; i >= 0; i--)
    {
        // deltaError[i] = deltaOutput[i+1] x weights[i+1]
        AIC_MatrixTraspose(&(mlnn->layers[i+1].weights), &t_weights);
        AIC_MatrixMultiplication(&(mlnn->layers[i+1].error), &t_weights, &deltaError);

        // derivative = dSigmoid(output[i])
        AIC_MatrixCopy(&derivative, &(mlnn->layers[i].output));
        AIC_MatrixApplyFunction(&derivative, dSigmoid);

        // deltaOutput[i] = deltaError[i] * dSigmoid(output[i])
        AIC_MatrixMultiplyElements(&deltaError, &derivative, &deltaOutput);

        // layer error = deltaOutput
        AIC_MatrixCopy(&(mlnn->layers[i].error), &deltaOutput);
    }

    AIC_MatrixDestroy(&deltaError);
    AIC_MatrixDestroy(&deltaOutput);
    AIC_MatrixDestroy(&t_weights);
    AIC_MatrixDestroy(&derivative);
}

static void AIC_MLNN_UpdateInputLayer(Matrix_t *input, Data_t learning_rate, ML_NeuralNetwork_t *mlnn, uint32_t layer)
{
    Matrix_t deltaBias = {0};

    // Update biases
    AIC_MatrixCopy(&deltaBias, &mlnn->layers[layer].error);
    AIC_MatrixMultiplyScalar(&deltaBias, learning_rate);
    AIC_MatrixAddItself(&mlnn->layers[layer].bias, &deltaBias);

    // Update weights
    Matrix_t t_input = {0}, deltaWeights = {0};
    AIC_MatrixTraspose(input, &t_input);
    AIC_MatrixMultiplication(&t_input, &mlnn->layers[layer].error, &deltaWeights);
    AIC_MatrixMultiplyScalar(&deltaWeights, learning_rate);
    AIC_MatrixAddItself(&mlnn->layers[layer].weights, &deltaWeights);

    AIC_MatrixDestroy(&deltaWeights);
    AIC_MatrixDestroy(&deltaBias);
    AIC_MatrixDestroy(&t_input);
}

static void AIC_MLNN_UpdateHiddenLayer(Data_t learning_rate, ML_NeuralNetwork_t *mlnn, uint32_t layer)
{
    Matrix_t deltaBias = {0};

    // Update biases
    AIC_MatrixCopy(&deltaBias, &mlnn->layers[layer].error);
    AIC_MatrixMultiplyScalar(&deltaBias, learning_rate);
    AIC_MatrixAddItself(&mlnn->layers[layer].bias, &deltaBias);

    // Update weights
    Matrix_t t_input = {0}, deltaWeights = {0};
    AIC_MatrixTraspose(&mlnn->layers[layer - 1].output, &t_input);
    AIC_MatrixMultiplication(&t_input, &mlnn->layers[layer].error, &deltaWeights);
    AIC_MatrixMultiplyScalar(&deltaWeights, learning_rate);
    AIC_MatrixAddItself(&mlnn->layers[layer].weights, &deltaWeights);


    AIC_MatrixDestroy(&deltaWeights);
    AIC_MatrixDestroy(&deltaBias);
    AIC_MatrixDestroy(&t_input);
}

void AIC_MLNN_Fit(Matrix_t *input, Matrix_t *prediction, Matrix_t *expected_output, Data_t learning_rate, ML_NeuralNetwork_t *mlnn)
{
    AIC_MLNN_Predict(input, prediction, mlnn);

    AIC_MLNN_GenerateErrors(prediction, expected_output, mlnn);

    AIC_MLNN_UpdateInputLayer(input, learning_rate, mlnn, 0);

    for (uint32_t i = 1; i < mlnn->n_layers; i++)
    {
        AIC_MLNN_UpdateHiddenLayer(learning_rate, mlnn, i);
    }
}

void AIC_MLNN_Print(ML_NeuralNetwork_t *mlnn)
{
    for(uint32_t i = 0; i < mlnn->n_layers; i++)
    {
        printf("Layer %d - Weights:\n", i);
        AIC_MatrixPrint(&mlnn->layers[i].weights, 1);
        printf("Layer %d - Bias:\n", i);
        AIC_MatrixPrint(&mlnn->layers[i].bias, 1);
        
    }
}