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

    Matrix_t input = {0}, output = {0}, expected_output = {0};

    ML_NeuralNetwork_t nn = {0};

    AIC_MLNN_Create(3, &nn);

    AIC_MLNN_SetInputLayer(3, 3, sigmoid, &nn);
    AIC_MLNN_SetHiddenLayer(1, 2, sigmoid, &nn);
    AIC_MLNN_SetHiddenLayer(2, 1, sigmoid, &nn);

    AIC_MatrixCreate(1, 1, &expected_output);

    for (int i = 0; i < 1000; i++)
    {
        AIC_MatrixCreateRand(1, 3, &input);
        Data_t sum = AIC_MatrixGetSum(&input);
        if (sum < 0.3)
            AIC_MatrixSet(0, 0, 1.0, &expected_output);
        if (sum >= 0.3 && sum < 0.6)
            AIC_MatrixSet(0, 0, 0.0, &expected_output);
        if (sum >= 0.6)
            AIC_MatrixSet(0, 0, 0.5, &expected_output);

        AIC_MLNN_Fit(&input, &expected_output, 0.1, &nn);
    }

    return 0;
}