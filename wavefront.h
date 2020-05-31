#pragma once

#include <GL/glcorearb.h>

void* load_wavefront(const char *_filepath, float **_vertices, int *_vertices_num, unsigned short **_indices, int *_indices_num);
