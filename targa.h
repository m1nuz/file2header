#pragma once

#include <stdint.h>

#include <GL/glcorearb.h>

enum TARGA_DATA_TYPE
{
    TARGA_DATA_NO = 0,
    TARGA_DATA_COLOR_MAPPED = 1,        // indexed
    TARGA_DATA_TRUE_COLOR = 2,          // RGB
    TARGA_DATA_BLACK_AND_WHITE = 3,     // grayscale
    TAGRA_DATA_RLE_COLOR_MAPPED = 9,
    TARGA_DATA_RLE_TRUE_COLOR = 10,
    TARGA_DATA_RLE_BLACK_AND_WITE = 11
};

#pragma pack(push, tga_header_align)
#pragma pack(1)
struct tga_header
{
    uint8_t     length;
    uint8_t     color_map;
    uint8_t     data_type;
    uint16_t    colormap_index;
    uint16_t    colormap_length;
    uint8_t     colormap_entry_size;
    uint16_t    x;
    uint16_t    y;
    uint16_t    width;
    uint16_t    height;
    uint8_t     bpp;
    uint8_t     decription;
};
#pragma pack(pop, tga_header_align)

#ifdef __cplusplus
extern "C" {
#endif

void* load_targa(const char *_filepath, GLuint *_iformat, GLenum *_format, GLsizei *_width, GLsizei *_height);

#ifdef __cplusplus
}
#endif
