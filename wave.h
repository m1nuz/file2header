#pragma once

#include "al/al.h"

struct riff_header
{
    char    chunkID[4];
    long    chunkSize;   // size not including chunkSize or chunkID
    char    format[4];
};

struct wave_format
{
    char    subChunkID[4];
    long    subChunkSize;
    short   audioFormat;
    short   numChannels;
    long    sampleRate;
    long    byteRate;
    short   blockAlign;
    short   bitsPerSample;
};

struct wave_data
{
    char    subChunkID[4]; // should contain the word data
    long    subChunk2Size; // Stores the size of the data block
};

#ifdef __cplusplus
extern "C" {
#endif

void* load_wave(const char *_filename, ALenum* _format, ALsizei* _frequency, ALsizei* _size);

#ifdef __cplusplus
}
#endif

