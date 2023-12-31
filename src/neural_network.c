#include "neural_network.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "rng.h"

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

void AIC_MLNN_PredictSet(Matrix_t *input, Matrix_t *prediction, uint32_t quantity, ML_NeuralNetwork_t *mlnn)
{
    for (uint32_t i = 0; i < quantity; i++)
        AIC_MLNN_Predict(&input[i], &prediction[i], mlnn);
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

static Data_t AIC_MLNN_GenerateErrors(Matrix_t *real_output, Matrix_t *expected_output, ML_NeuralNetwork_t *mlnn)
{
    Matrix_t deltaError = {0}, deltaOutput = {0}, t_weights = {0}, derivative = {0};

    // Error = MSE
    AIC_MatrixSub(expected_output, real_output, &deltaError);
    AIC_MatrixApplyFunction(&deltaError, squared);
    Data_t error = AIC_MatrixGetSum(&deltaError);

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
        AIC_MatrixTraspose(&(mlnn->layers[i + 1].weights), &t_weights);
        AIC_MatrixMultiplication(&(mlnn->layers[i + 1].error), &t_weights, &deltaError);

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

    return error;
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

static Data_t AIC_MLNN_Fit(Matrix_t *input, Matrix_t *prediction, Matrix_t *expected_output, Data_t learning_rate, ML_NeuralNetwork_t *mlnn)
{
    AIC_MLNN_Predict(input, prediction, mlnn);

    Data_t error = AIC_MLNN_GenerateErrors(prediction, expected_output, mlnn);

    AIC_MLNN_UpdateInputLayer(input, learning_rate, mlnn, 0);

    for (uint32_t i = 1; i < mlnn->n_layers; i++)
    {
        AIC_MLNN_UpdateHiddenLayer(learning_rate, mlnn, i);
    }

    return error;
}

void AIC_MLNN_Print(ML_NeuralNetwork_t *mlnn)
{
    for (uint32_t i = 0; i < mlnn->n_layers; i++)
    {
        printf("Layer %d - Weights:\n", i);
        AIC_MatrixPrint(&mlnn->layers[i].weights, 1);
        printf("Layer %d - Bias:\n", i);
        AIC_MatrixPrint(&mlnn->layers[i].bias, 1);
    }
}

static void shuffle(uint32_t *array, size_t n)
{
    AIC_RNG_Seed();

    if (n > 1)
    {
        size_t i;
        for (i = n - 1; i > 0; i--)
        {
            size_t j = (unsigned int)(AIC_RNG_drand() * (i + 1));
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

void AIC_MLNN_TrainDataset(MLNN_TrainDataset_t dataset, uint8_t shuffle_data, Data_t learning_rate, uint32_t epochs, ML_NeuralNetwork_t *mlnn)
{
    Matrix_t dummy_output = {0};

    clock_t start, end;
    double cpu_time_used;
    Data_t error = 0;

    uint32_t indices[dataset.quantity];

    for (uint32_t i = 0; i < dataset.quantity; i++)
        indices[i] = i;

    for (uint32_t j = 0; j < epochs; j++)
    {
        start = clock();
        printf("Epoch %d/%d\t", j + 1, epochs);
        fflush(stdout);
        if (shuffle_data)
            shuffle(indices, dataset.quantity);
        error = 0;
        for (uint32_t i = 0; i < dataset.quantity; i++)
        {
            error += AIC_MLNN_Fit(&dataset.inputs[indices[i]], &dummy_output, &dataset.expected_outputs[indices[i]], learning_rate, mlnn);
        }
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Done in %lfsec\tAverage error = %f\n", cpu_time_used, error / dataset.quantity);
    }

    AIC_MatrixDestroy(&dummy_output);
}

void AIC_MLNN_TrainDatasetCreate(MLNN_TrainDataset_t *dataset, uint32_t size)
{
    dataset->quantity = size;

    dataset->inputs = calloc(size, sizeof(Matrix_t));
    dataset->expected_outputs = calloc(size, sizeof(Matrix_t));
}

void AIC_MLNN_TrainDatasetDestroy(MLNN_TrainDataset_t *dataset)
{
    dataset->quantity = 0;

    free(dataset->inputs);
    free(dataset->expected_outputs);
}