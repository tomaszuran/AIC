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
    WAVE_Create(2, 44100, 44100 * 5, &wave);

    //WAVE_Get("2.wav", &wave);

    short * data = malloc(sizeof(short) * wave.wave_data.size);

    for(uint32_t i = 0; i < wave.wave_data.size; i+=2)
    {
        double t = (double)i / (wave.fmt.SampleRate * wave.fmt.NumChannels); 
        short value1 = 5000 * sin(2 * PI * 1000 * t);
        short value2 = 5000 * sin(2 * PI * 440 * t);
        data[i] = value1;
        data[i+1] = value2;
    }

    WAVE_SetData(data, &wave);

    WAVE_Save("test3.wav", &wave);
    return 0;
}