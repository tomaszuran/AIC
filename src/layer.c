#include "layer.h"

#include <stdio.h>

void AIC_LayerCreate(uint32_t inputs, uint32_t layer_size, Layer_t * layer, ActivationFunction_t activation_function)
{
    layer->n_in = inputs;
    layer->layer_size = layer_size;
    layer->activation_function = activation_function;

    // INPUT FORMAT: (1, inputs)

    AIC_MatrixCreateRand(inputs, layer_size, &layer->weights);
    AIC_MatrixCreateRand(1, layer_size, &layer->bias);
    AIC_MatrixCreate(1, layer_size, &layer->output);
    AIC_MatrixCreate(1, layer_size, &layer->error);
}

void AIC_LayerDestroy(Layer_t * layer)
{
    AIC_MatrixDestroy(&layer->weights);
    AIC_MatrixDestroy(&layer->bias);
    AIC_MatrixDestroy(&layer->output);
    AIC_MatrixDestroy(&layer->error);
}

void AIC_LayerGenerateOutput(Matrix_t * input, Layer_t * layer)
{
    if(DEBUG)
    {
        if(input->rows != 1 || input->cols != layer->n_in)
        {
            fprintf(stderr, "AIC_LayerGenerateOutput input dimension error.\n");
            return;
        }
    }
    AIC_MatrixMultiplication(input, &layer->weights, &layer->output);
    AIC_MatrixAddItself(&layer->output, &layer->bias);
    AIC_MatrixApplyFunction(&layer->output, layer->activation_function);
}