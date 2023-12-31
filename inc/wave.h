#ifndef WAVE_H
#define WAVE_H

#include <stdint.h>

typedef struct WaveRiffTypeDef
{
    char ChunkID[4];
    uint32_t ChunkSize;
    char Format[4];
} WAVE_Riff_t;

typedef struct WaveFmtTypeDef
{
    char SubChunk1ID[4];
    uint32_t SubChunk1Size;
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
    // Extra params
} WAVE_Fmt_t;

typedef struct WaveDataTypeDef
{
    char SubChunk2ID[4];
    uint32_t SubChunk2Size;
    uint32_t size;
    short *data;
} WAVE_Data_t;

typedef struct WaveTypeDef
{
    WAVE_Riff_t riff;
    WAVE_Fmt_t fmt;
    WAVE_Data_t wave_data;
} Wave_t;

void WAVE_Create(WAVE_Riff_t riff, WAVE_Fmt_t fmt, int duration_ms, Wave_t *wave);
void WAVE_Get(char *filename, Wave_t *wave);
void WAVE_Save(char *filename, Wave_t *wave);
void WAVE_SetData(short *data, Wave_t *wave);

#endif // WAVE_H