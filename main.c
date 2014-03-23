#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "targa.h"
#include "wave.h"
#include "wavefront.h"

struct
{
    GLuint          iformat;
    GLenum          format;
    GLsizei         width;
    GLsizei         height;
    uint8_t         *data;
} tga;

struct
{
    ALenum          format;
    ALsizei         frequency;
    ALsizei         size;
    uint8_t         *data;
} wave;

struct
{
    float           *vertices;
    int             vertices_num;
    unsigned short  *indices;
    int             indices_num;
} obj;

void save_targa_to_header(const char *filename, const char *name)
{
    FILE* fp = fopen(filename, "w");

    fprintf(fp, "#pragma once\n");

    char s[10] = {};

    if(tga.format == GL_BGR)
        strcpy(s, "GL_BGR");

    if(tga.format == GL_BGRA)
        strcpy(s, "GL_BGR");

    fprintf(fp, "const GLenum %s_format = %s;\n", name, s);

    if(tga.iformat == GL_RGB8)
        strcpy(s, "GL_RGB8");

    if(tga.format == GL_RGBA8)
        strcpy(s, "GL_RGBA8");

    fprintf(fp, "const GLuint %s_internal_format = %s;\n", name, s);

    fprintf(fp, "const GLsizei %s_width = %d;\n", name, tga.width);
    fprintf(fp, "const GLsizei %s_height = %d;\n", name, tga.height);

    int bpp = 3;

    if(tga.iformat == GL_RGBA8)
        bpp = 4;

    fprintf(fp, "const unsigned char %s_data[] = {\n", name);
    for(unsigned i = 0; i < tga.width * tga.height * bpp; i++)
    {
        if((i != 0) && ((i % bpp) == 0))
            fprintf(fp, "\n");

        fprintf(fp, "%#x, ", tga.data[i]);
    }
    fprintf(fp, "};\n");

    fclose(fp);
}

void save_wave_to_header(const char *filename, const char *name)
{
    FILE* fp = fopen(filename, "w");

    fprintf(fp, "#pragma once\n");

    char s[40] = {};

    int depth = 0;

    switch(wave.format)
    {
    case AL_FORMAT_MONO8:
        strcpy(s, "AL_FORMAT_MONO8");
        depth = 1;
        break;
    case AL_FORMAT_MONO16:
        strcpy(s, "AL_FORMAT_MONO16");
        depth = 2;
        break;
    case AL_FORMAT_STEREO8:
        strcpy(s, "AL_FORMAT_STEREO8");
        depth = 1;
        break;
    case AL_FORMAT_STEREO16:
        strcpy(s, "AL_FORMAT_STEREO16");
        depth = 2;
        break;
    }

    fprintf(fp, "const ALenum %s_format = %s;\n", name, s);
    fprintf(fp, "const ALsizei %s_frequency = %d;\n", name, wave.frequency);
    fprintf(fp, "const ALsizei %s_size = %d;\n", name, wave.size);

    fprintf(fp, "const unsigned char %s_data[] = {\n", name);

    for(int i = 0; i < wave.size; i++)
    {
        if((i != 0) && (i % 10) == 0)
            fprintf(fp, "\n");

        fprintf(fp, "%#x, ", wave.data[i]);
    }

    fprintf(fp, "};\n");

    fclose(fp);
}

void save_obj_to_header(const char *filename, const char *name)
{
    FILE* fp = fopen(filename, "w");

    fprintf(fp, "#pragma once\n");

    fprintf(fp, "const float %s_vertices[] = {\n", name);

    for(int i = 0; i < obj.vertices_num * 3; i++)
    {
        if((i != 0) && (i % 3) == 0)
            fprintf(fp, "\n");

        fprintf(fp,"%4.6f, ", obj.vertices[i]);
    }

    fprintf(fp, "};\n");

    fprintf(fp, "const unsigned short %s_indices[] = {\n", name);

    for(int i = 0; i < obj.indices_num; i++)
    {
        if((i != 0) && (i % 3) == 0)
            fprintf(fp, "\n");

        fprintf(fp,"%4hd, ", obj.indices[i]);
    }

    fprintf(fp, "};\n");

    fprintf(fp, "const int %s_vertices_num = %d;\n", name, obj.vertices_num);
    fprintf(fp, "const int %s_indices_num = %d;\n", name, obj.indices_num);

    fclose(fp);
}

int main(int argc, char *argv[]) {

    if(argc < 3)
        return 0;

    char outname[260] = {};
    char name[100] = {};
    char filename[260];

    strcpy(filename, argv[1]);
    strcpy(name, argv[2]);
    strcpy(outname, name);
    strcat(outname, ".h");

    char *ext = strrchr(filename, '.') + 1;

    if(strcmp(ext, "tga") == 0)
    {
        tga.data = (uint8_t*)load_targa(filename, &tga.iformat, &tga.format, &tga.width, &tga.height);

        save_targa_to_header(outname, name);

        free(tga.data);
    }

    if((strcmp(ext, "wav") == 0) || (strcmp(ext, "wave") == 0))
    {
        wave.data = (uint8_t*)load_wave(filename, &wave.format, &wave.frequency, &wave.size);

        save_wave_to_header(outname, name);

        free(wave.data);
    }

    if((strcmp(ext, "obj") == 0) || (strcmp(ext, "wave") == 0))
    {
        load_wavefront(filename, &obj.vertices, &obj.vertices_num, &obj.indices, &obj.indices_num);

        save_obj_to_header(outname, name);

        free(obj.vertices);
        free(obj.indices);
    }
}
