#include "wave.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t WAVE_Create(uint16_t NumChannels, uint32_t SampleRate, uint32_t NumSamples, Wave_t * wave)
{
    wave->riff.ChunkID[0] = 'R';
    wave->riff.ChunkID[1] = 'I';
    wave->riff.ChunkID[2] = 'F';
    wave->riff.ChunkID[3] = 'F';
    
    wave->riff.Format[0] = 'W';
    wave->riff.Format[1] = 'A';
    wave->riff.Format[2] = 'V';
    wave->riff.Format[3] = 'E';

    // Falta riff chunksize que es (SubChunk2Size + 36)

    wave->fmt.SubChunk1ID[0] = 'f';
    wave->fmt.SubChunk1ID[1] = 'm';
    wave->fmt.SubChunk1ID[2] = 't';
    wave->fmt.SubChunk1ID[3] =  32;

    wave->fmt.SubChunk1Size = 16;
    wave->fmt.AudioFormat = 1;
    wave->fmt.NumChannels = NumChannels;
    wave->fmt.SampleRate = SampleRate;
    wave->fmt.ByteRate = SampleRate * 2 * NumChannels;
    wave->fmt.BlockAlign = NumChannels == 1 ? 2 : 4;
    wave->fmt.BitsPerSample = 16;
    
    wave->wave_data.SubChunk2ID[0] = 'd';
    wave->wave_data.SubChunk2ID[1] = 'a';
    wave->wave_data.SubChunk2ID[2] = 't';
    wave->wave_data.SubChunk2ID[3] = 'a';
    
    wave->wave_data.SubChunk2Size = NumSamples * wave->fmt.BlockAlign;
    wave->riff.ChunkSize = wave->wave_data.SubChunk2Size + 32;

    wave->wave_data.size = NumSamples;

    wave->wave_data.data = malloc(sizeof(short) * NumSamples);
    
    if(!wave->wave_data.data)
        return 0;

    return 1;
}


static uint8_t checkId(char *id)
{
    uint8_t check_fmt = 1;
    uint8_t check_data = 1;
    uint8_t check_riff = 1;

    char fmt[] = "fmt ";
    char data[] = "data";
    char riff[] = "RIFF";
    for (int i = 0; i < 4; i++)
    {
        if (id[i] != fmt[i])
            check_fmt = 0;
        if (id[i] != data[i])
            check_data = 0;
        if (id[i] != riff[i])
            check_riff = 0;
    }

    if (check_fmt)
        return 1;
    else if (check_data)
        return 2;
    else if (check_riff)
        return 3;
    else
        return 0;
}

void WAVE_Get(char *filename, Wave_t *wave)
{
    FILE *fp = fopen(filename, "rb");

    while (!feof(fp))
    {
        char id[5];
        uint32_t size;
        fread(id, sizeof(char), 4, fp);
        id[4] = 0;

        fread(&size, sizeof(uint32_t), 1, fp);

        uint8_t check_id = checkId(id);

        if (check_id == 1) // fmt
        {
            strncpy(wave->fmt.SubChunk1ID, id, 4); // fmt.SubChunk1ID = id;
            wave->fmt.SubChunk1Size = size;
            fread(&wave->fmt.AudioFormat, 1, size, fp);
        }
        else if (check_id == 2) // data
        {
            strncpy(wave->wave_data.SubChunk2ID, id, 4); // dat.SubChunk2ID = id;
            wave->wave_data.SubChunk2Size = size;
            wave->wave_data.size = size / sizeof(short);
            wave->wave_data.data = malloc(sizeof(short) * wave->wave_data.size);
            fread(wave->wave_data.data, sizeof(short), wave->wave_data.size, fp);
        }
        else if (check_id == 3) // riff
        {
            strncpy(wave->riff.ChunkID, id, 4); // RIFF.ChunkID = id;
            wave->riff.ChunkSize = size;
            fread(wave->riff.Format, sizeof(char), 4, fp);
        }
        else
        {
            //fprintf(stderr, "CheckID error\n");
        }
    }
}

void WAVE_Save(char * filename, Wave_t * wave)
{
    FILE *newfile = fopen(filename, "wb");
    fwrite(&wave->riff, sizeof(WAVE_Riff_t), 1, newfile);
    fwrite(&wave->fmt, sizeof(WAVE_Fmt_t), 1, newfile);
    fwrite(wave->wave_data.SubChunk2ID, sizeof(char), 4, newfile);
    fwrite(&wave->wave_data.SubChunk2Size, sizeof(uint32_t), 1, newfile);
    fwrite(wave->wave_data.data, sizeof(short), wave->wave_data.size, newfile);
    fclose(newfile);
}

void WAVE_SetData(short *data, Wave_t *wave)
{
    memcpy(wave->wave_data.data, data, wave->wave_data.size * 2);
}

uint32_t WAVE_GetNumSamples(Wave_t *wave)
{
    return wave->wave_data.size;
}