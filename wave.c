#include <stdio.h>
#include <stdlib.h>

#include "wave.h"

void*
load_wave(const char *_filename, ALenum* _format, ALsizei* _frequency, ALsizei* _size)
{
    struct riff_header riff;
    struct wave_format format;
    struct wave_data   data;

    FILE *fp = fopen(_filename, "rb");

    if(!fp)
        return NULL;

    fread(&riff, sizeof(riff), 1, fp);

    if((riff.chunkID[0] != 'R' || riff.chunkID[1] != 'I' || riff.chunkID[2] != 'F' || riff.chunkID[3] != 'F') ||
       (riff.format[0] != 'W' || riff.format[1] != 'A' || riff.format[2] != 'V' || riff.format[3] != 'E'))
    {
        fprintf(stderr, "error: Invalid RIFF or WAVE header\n");
        return NULL;
    }

    fread(&format, sizeof(format), 1, fp);

    if(format.subChunkID[0] != 'f' || format.subChunkID[1] != 'm' || format.subChunkID[2] != 't' || format.subChunkID[3] != ' ')
    {
        fprintf(stderr, "error: Invalid WAVE format\n");
        return NULL;
    }

    if(format.subChunkSize > 16)
        fseek(fp, sizeof(short), SEEK_CUR);

    fread(&data, sizeof(data), 1, fp);

    if(data.subChunkID[0] != 'd' || data.subChunkID[1] != 'a' || data.subChunkID[2] != 't' || data.subChunkID[3] != 'a')
    {
        fprintf(stderr, "error: Invalid data header\n");
        return NULL;
    }

    void* ptr = malloc(data.subChunk2Size);

    if(!fread(ptr, data.subChunk2Size, 1, fp))
    {
        fprintf(stderr, "error: Can\'t read WAVE data\n");
        return NULL;
    }

    *_size = data.subChunk2Size;
    *_frequency = format.sampleRate;

    if(format.numChannels == 1)
    {
        if(format.bitsPerSample == 8)
            *_format = AL_FORMAT_MONO8;
        else if(format.bitsPerSample == 16)
            *_format = AL_FORMAT_MONO16;
    } else if (format.numChannels == 2)
    {
        if(format.bitsPerSample == 8)
            *_format = AL_FORMAT_STEREO8;
        else if(format.bitsPerSample == 16)
            *_format = AL_FORMAT_STEREO16;
    }

    fclose(fp);

    return ptr;
}
