#include <stdio.h>
#include "matrix.h"
#include "layer.h"
#include "serialize.h"
#include "neural_network.h"

#include <stdlib.h>
#include <math.h>
#include "rng.h"
#include <time.h>

#define TRAIN_DATASET_SIZE 40

// https://towardsdatascience.com/simple-neural-network-implementation-in-c-663f51447547

int main(int argc, char **argv)
{
    ML_NeuralNetwork_t nn = {0};
    /*
    AIC_MLNN_Create(3, &nn);

    AIC_MLNN_SetInputLayer(2, 2, sigmoid, &nn);
    AIC_MLNN_SetHiddenLayer(1, 2, sigmoid, &nn);
    AIC_MLNN_SetHiddenLayer(2, 1, sigmoid, &nn);
    */
    MLNN_TrainDataset_t train_dataset;

    AIC_MLNN_TrainDatasetCreate(&train_dataset, 40);

    AIC_MatrixLoadFromCSV("train_inputs.csv", train_dataset.inputs);
    AIC_MatrixLoadFromCSV("train_outputs.csv", train_dataset.expected_outputs);

    FILE *fp = fopen("neural_network.data", "rb");
    AIC_FileMLNNLoad(fp, &nn);
    fclose(fp);

    AIC_MLNN_TrainDataset(train_dataset, 1, 0.1, 1, &nn);

    fp = fopen("neural_network.data", "wb");
    AIC_FileMLNNSave(fp, &nn);
    fclose(fp);
    return 0;
}