#include <stdio.h>
#include "matrix.h"
#include "layer.h"
#include "serialize.h"
#include "neural_network.h"

#include <stdlib.h>

// https://towardsdatascience.com/simple-neural-network-implementation-in-c-663f51447547

int main(int argc, char **argv)
{
    AIC_TimeSeedRand();

    Matrix_t input = {0}, output = {0};

    ML_NeuralNetwork_t nn = {0};

    AIC_MLNN_Create(3, &nn);

    AIC_MLNN_SetInputLayer(7, 10, sigmoid, &nn);
    AIC_MLNN_SetHiddenLayer(1, 10, sigmoid, &nn);
    AIC_MLNN_SetHiddenLayer(2, 2, sigmoid, &nn);

    AIC_MatrixCreateRand(1, 7, &input);

    AIC_MLNN_Predict(&input, &output, &nn);

    AIC_MatrixPrint(&output, 1);

    return 0;
}