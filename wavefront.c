#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "wavefront.h"

struct v3_t
{
    float x, y, z;
};

void*
load_wavefront(const char *_filepath, float **_vertices, int *_vertices_num, unsigned short **_indices, int *_indices_num)
{
    assert(_filepath);

    FILE *fp = fopen(_filepath, "rb");

    if(!fp)
        return NULL;

    char line[512] = {0};

    int vertices_num = 100;
    int vertices_count = 0;
    struct v3_t *vertices = malloc(sizeof(struct v3_t) * vertices_num);

    int faces_num = 1000;
    int faces_count = 0;
    unsigned short *faces = malloc(sizeof(unsigned short) * faces_num);
    unsigned short *fs = faces;

    while(!feof(fp))
    {
        memset(line, 0, sizeof(line));
        fgets(line, sizeof(line), fp);

        if(line[0] == '#' || (strcmp(line, " ") == 0) || line[0] == '\n')
            continue;

        if(line[0] == 'v' && line[1] == ' ')
        {
            char *p = strtok(line, " \n");

            int index = vertices_count;

            p = strtok(0, " \n");
            vertices[index].x = atof(p);

            p = strtok(0, " \n");
            vertices[index].y = atof(p);

            p = strtok(0, " \n");
            vertices[index].z = atof(p);

            vertices_count++;

            if(vertices_count >= vertices_num)
            {
                vertices_num *= 2;
                vertices = realloc(vertices, sizeof(struct v3_t) * vertices_num);
            }

            //printf("v %f %f %f\n", vertices[index].x, vertices[index].y, vertices[index].z);
        }

        if(line[0] == 'f')
        {
            char *p = strtok(line, " \n");

            p = strtok(0, " \n");
            fs[0] = atoi(p);

            p = strtok(0, " \n");
            fs[1] = atoi(p);

            p = strtok(0, " \n");
            fs[2] = atoi(p);

            //printf("f %d %d %d\n", fs[0], fs[1], fs[2]);

            fs += 3;
            faces_count += 3;

            if(faces_count >= faces_num)
            {
                faces_num *= 2;
                faces = realloc(faces, sizeof(unsigned short) * faces_num);
                fs = &faces[faces_count + 1];
            }
        }
    }

    *_vertices = malloc(sizeof(float) * 3 * faces_count);

    for(int i = 0; i < faces_count; i++)
    {
        struct v3_t vx = vertices[faces[i] - 1];

        (*_vertices)[i * 3 + 0] = vx.x;
        (*_vertices)[i * 3 + 1] = vx.y;
        (*_vertices)[i * 3 + 2] = vx.z;

        //printf("v[%d] %f %f %f\n", i, vx.x, vx.y, vx.z);
    }

    *_indices = malloc(sizeof(unsigned short) * faces_count);

    for(int i = 0; i < faces_count; i++)
        (*_indices)[i] = i;

    *_vertices_num = faces_count;
    *_indices_num = faces_count;

    free(faces);
    free(vertices);

    fclose(fp);

    return NULL;
}
