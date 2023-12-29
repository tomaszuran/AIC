#include <stdio.h>
#include "matrix.h"
#include "layer.h"
#include "serialize.h"
#include "neural_network.h"

#include <stdlib.h>
#include <math.h>

// https://towardsdatascience.com/simple-neural-network-implementation-in-c-663f51447547

int main(int argc, char **argv)
{
    AIC_TimeSeedRand();

    Matrix_t input = {0}, output = {0}, expected_output = {0};

    ML_NeuralNetwork_t nn = {0};

    AIC_MLNN_Create(2, &nn);

    AIC_MLNN_SetInputLayer(2, 2, sigmoid, &nn);
    AIC_MLNN_SetHiddenLayer(1, 1, sigmoid, &nn);

    AIC_MatrixCreate(1, 1, &expected_output);
    AIC_MatrixCreate(1, 2, &input);

    float lr = 1;
    for (int j = 0; j < 100; j++)
    {
        lr = 0.1;
        for (int i = 0; i < 1000; i++)
        {

            uint8_t input_case = rand() % 4;
            switch (input_case)
            {
            case 0:
                AIC_MatrixSet(0, 0, 0.0, &input);
                AIC_MatrixSet(0, 1, 0.0, &input);
                AIC_MatrixSet(0, 0, 0.0, &expected_output);
                break;
            case 1:
                AIC_MatrixSet(0, 0, 0.0, &input);
                AIC_MatrixSet(0, 1, 1.0, &input);
                AIC_MatrixSet(0, 0, 1.0, &expected_output);
                break;
            case 2:
                AIC_MatrixSet(0, 0, 1.0, &input);
                AIC_MatrixSet(0, 1, 0.0, &input);
                AIC_MatrixSet(0, 0, 1.0, &expected_output);
                break;
            case 3:
                AIC_MatrixSet(0, 0, 1.0, &input);
                AIC_MatrixSet(0, 1, 1.0, &input);
                AIC_MatrixSet(0, 0, 0.0, &expected_output);
                break;
            }

            AIC_MLNN_Fit(&input, &output, &expected_output, lr, &nn);
        }
    }

    for (int i = 0; i < 5; i++)
    {
        uint8_t input_case = rand() % 4;
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