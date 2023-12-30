#ifndef RNG_H
#define RNG_H

#include <stdio.h>
#include <stdint.h>
#include <intrin.h>
#include <time.h>

#define AIC_RNG_RANDMAX 1073741824

void AIC_RNG_Seed();
uint32_t AIC_RNG_rand(void);
double AIC_RNG_drand(void);

#endif