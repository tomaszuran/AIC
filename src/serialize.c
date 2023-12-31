#include "serialize.h"
#include <stdlib.h>

void AIC_SerializeMatrixSave(uint8_t **data, Matrix_t *matrix)
{
    *data = malloc(sizeof(uint32_t) * 2 + sizeof(Data_t) * matrix->cols * matrix->rows);

    *((uint32_t *)*data) = matrix->cols;

    *((uint32_t *)*data + 1) = matrix->rows;

    for (uint32_t i = 0; i < (matrix->cols * matrix->rows * sizeof(Data_t)); ++i)
    {
        *(*data + 8 + i) = *((uint8_t *)matrix->data + i);
    }
}

void AIC_SerializeMatrixLoad(uint8_t *data, Matrix_t *matrix)
{
    matrix->cols = *((uint32_t *)data);

    matrix->rows = *((uint32_t *)data + 1);

    matrix->data = calloc(matrix->rows * matrix->cols, sizeof(Data_t));

    for (uint32_t i = 0; i < (matrix->cols * matrix->rows * sizeof(Data_t)); ++i)
    {
        *(((uint8_t *)matrix->data) + i) = *(data + 8 + i);
    }
}

void AIC_FileMatrixSave(FILE *fp, Matrix_t *matrix)
{
    fwrite(&matrix->cols, sizeof(uint32_t), 1, fp);
    fwrite(&matrix->rows, sizeof(uint32_t), 1, fp);
    fwrite(matrix->data, sizeof(Data_t), matrix->cols * matrix->rows, fp);
}
void AIC_FileMatrixLoad(FILE *fp, Matrix_t *matrix)
{
    fread(&matrix->cols, sizeof(uint32_t), 1, fp);
    fread(&matrix->rows, sizeof(uint32_t), 1, fp);

    matrix->data = calloc(matrix->rows * matrix->cols, sizeof(Data_t));

    fread(matrix->data, sizeof(Data_t), matrix->rows * matrix->cols, fp);
}

void AIC_FileLayerLoad(FILE *fp, Layer_t *layer)
{
    /*
        uint32_t n_in, layer_size;
        ActivationFunction_t activation_function;
        Matrix_t weights, bias, output, error;
    */
    fread(&layer->n_in, sizeof(uint32_t), 1, fp);
    fread(&layer->layer_size, sizeof(uint32_t), 1, fp);
    uint8_t af = 0;
    fread(&af, sizeof(uint8_t), 1, fp);
    switch (af)
    {
    case 1:
        layer->activation_function = sigmoid;
        break;
    }
    AIC_FileMatrixLoad(fp, &layer->weights);
    AIC_FileMatrixLoad(fp, &layer->bias);
    AIC_FileMatrixLoad(fp, &layer->output);
    AIC_FileMatrixLoad(fp, &layer->error);
}

void AIC_FileLayerSave(FILE *fp, Layer_t *layer)
{
    uint8_t af = 1;
    if (layer->activation_function == sigmoid)
        af = 1;
    fwrite(&layer->n_in, sizeof(uint32_t), 1, fp);
    fwrite(&layer->layer_size, sizeof(uint32_t), 1, fp);

    fwrite(&af, sizeof(uint8_t), 1, fp);
    AIC_FileMatrixSave(fp, &layer->weights);
    AIC_FileMatrixSave(fp, &layer->bias);
    AIC_FileMatrixSave(fp, &layer->output);
    AIC_FileMatrixSave(fp, &layer->error);
}

void AIC_FileMLNNSave(FILE *fp, ML_NeuralNetwork_t *mlnn)
{
    fwrite(&mlnn->n_layers, sizeof(uint32_t), 1, fp);

    for (uint32_t i = 0; i < mlnn->n_layers; i++)
        AIC_FileLayerSave(fp, &mlnn->layers[i]);
}

void AIC_FileMLNNLoad(FILE *fp, ML_NeuralNetwork_t *mlnn)
{
    fread(&mlnn->n_layers, sizeof(uint32_t), 1, fp);

    mlnn->layers = calloc(mlnn->n_layers, sizeof(Layer_t));

    for (uint32_t i = 0; i < mlnn->n_layers; i++)
        AIC_FileLayerLoad(fp, &mlnn->layers[i]);
}