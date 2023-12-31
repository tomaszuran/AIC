#include <stdio.h>
#include "matrix.h"
#include "layer.h"
#include "serialize.h"
#include "neural_network.h"

#include <stdlib.h>
#include <math.h>
#include "rng.h"
#include <time.h>

#define TRAIN_DATASET_SIZE 10000

// https://towardsdatascience.com/simple-neural-network-implementation-in-c-663f51447547

int main(int argc, char **argv)
{

    Matrix_t input = {0}, output = {0}, expected_output = {0};

    ML_NeuralNetwork_t nn = {0};

    AIC_MLNN_Create(2, &nn);

    AIC_MLNN_SetInputLayer(2, 2, sigmoid, &nn);
    AIC_MLNN_SetHiddenLayer(1, 1, sigmoid, &nn);

    AIC_MatrixCreate(1, 1, &expected_output);
    AIC_MatrixCreate(1, 2, &input);

    MLNN_TrainDataset_t train_dataset;

    AIC_MLNN_TrainDatasetCreate(&train_dataset, TRAIN_DATASET_SIZE);

    AIC_RNG_Seed();

    for (int i = 0; i < TRAIN_DATASET_SIZE; i++)
    {
        AIC_MatrixCreate(1, 1, &train_dataset.expected_outputs[i]);
        AIC_MatrixCreate(1, 2, &train_dataset.inputs[i]);
        
        uint8_t input_case = (uint8_t) (AIC_RNG_drand() * 4);
        switch (input_case)
        {
        case 0:
            AIC_MatrixSet(0, 0, 0.0, &train_dataset.inputs[i]);
            AIC_MatrixSet(0, 1, 0.0, &train_dataset.inputs[i]);
            AIC_MatrixSet(0, 0, 0.0, &train_dataset.expected_outputs[i]);
            break;
        case 1:
            AIC_MatrixSet(0, 0, 0.0, &train_dataset.inputs[i]);
            AIC_MatrixSet(0, 1, 1.0, &train_dataset.inputs[i]);
            AIC_MatrixSet(0, 0, 1.0, &train_dataset.expected_outputs[i]);
            break;
        case 2:
            AIC_MatrixSet(0, 0, 1.0, &train_dataset.inputs[i]);
            AIC_MatrixSet(0, 1, 0.0, &train_dataset.inputs[i]);
            AIC_MatrixSet(0, 0, 1.0, &train_dataset.expected_outputs[i]);
            break;
        case 3:
            AIC_MatrixSet(0, 0, 1.0, &train_dataset.inputs[i]);
            AIC_MatrixSet(0, 1, 1.0, &train_dataset.inputs[i]);
            AIC_MatrixSet(0, 0, 0.0, &train_dataset.expected_outputs[i]);
            break;
        }
    }

    AIC_MLNN_TrainDataset(train_dataset, 1, 0.1, 100, &nn);
    AIC_MLNN_TrainDataset(train_dataset, 1, 0.01, 100, &nn);

    for (int i = 0; i < 5; i++)
    {
        uint8_t input_case = (uint8_t) (AIC_RNG_drand() * 4);
        switch (input_case)
        {
        case 0:
            AIC_MatrixSet(0, 0, 0.0, &input);
            AIC_MatrixSet(0, 1, 0.0, &input);
            break;
        case 1:
            AIC_MatrixSet(0, 0, 0.0, &input);
            AIC_MatrixSet(0, 1, 1.0, &input);
            break;
        case 2:
            AIC_MatrixSet(0, 0, 1.0, &input);
            AIC_MatrixSet(0, 1, 0.0, &input);
            break;
        case 3:
            AIC_MatrixSet(0, 0, 1.0, &input);
            AIC_MatrixSet(0, 1, 1.0, &input);
            break;
        }

        AIC_MLNN_Predict(&input, &output, &nn);

        printf("Input:\n");
        AIC_MatrixPrint(&input, 1);
        printf("Output:\n");
        AIC_MatrixPrint(&output, 1);
    }

    return 0;
}