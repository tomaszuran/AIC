#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "serialize.h"
#include "neural_network.h"
#include "wave.h"

#define PI 3.141592653589793

int main(int argc, char **argv)
{
    Wave_t wave;
    WAVE_Get("test.wav", &wave);

    printf("Data size = %d\n", wave.wave_data.size);

    short * data = malloc(sizeof(short) * wave.wave_data.size);

    for(uint32_t i = 0; i < wave.wave_data.size; i+=2)
    {
        double t = (double)i / (wave.fmt.SampleRate * wave.fmt.NumChannels); 
        short value = 20000 * sin(2 * PI * 1000 * t);
        data[i] = value;
        data[i+1] = value;
    }

    WAVE_SetData(data, &wave);

    WAVE_Save("test2.wav", &wave);
    return 0;
}