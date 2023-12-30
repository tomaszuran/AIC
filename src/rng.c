#include "rng.h"

uint8_t initialized = 0;

uint32_t AIC_RNG_rand(void)
{
    uint16_t x = rand();
    uint16_t y = rand();
    
    return (x << 15) | y;
}

void AIC_RNG_Seed()
{
    srand(time(NULL));
    if(initialized)
        return;
    initialized = 1;
}

double AIC_RNG_drand(void)
{
    return (double) AIC_RNG_rand() / AIC_RNG_RANDMAX;
}