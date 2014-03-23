#include "targa.h"

#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void* load_targa(const char *_filepath, GLuint *_iformat, GLenum *_format, GLsizei *_width, GLsizei *_height)
{
    assert(_filepath);
    int i = 0, j = 0;

    FILE *fp = fopen(_filepath, "rb");

    if(!fp)
        return NULL;

    fseek(fp, 0, SEEK_END);
    int lenght = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    struct tga_header header = {0};
    fread(&header, sizeof(header), 1, fp);

    const uint8_t bytesperpixel = header.bpp / 8;
    uint8_t *data = (uint8_t*)malloc(header.width * header.height * (bytesperpixel + 1)); // todo : why +1??? it works
    uint8_t *pdata = data;

    if(header.data_type == TARGA_DATA_RLE_TRUE_COLOR)
    {
        uint8_t block = 0;

        for(i = 0; (i < header.width * header.height) && !feof(fp); i++)
        {
            fread(&block, 1, 1, fp);

            uint8_t count = (block & 0x7f) + 1;

            if(block & 0x80)
            {
                uint8_t bytes[4] = {};
                fread(bytes, bytesperpixel, 1, fp);

                for(j = 0; j < count; j++)
                {
                    memcpy(pdata, bytes, bytesperpixel);
                    pdata += bytesperpixel;
                }
            }
            else
            {
                fread(pdata, bytesperpixel * count, 1, fp);
                pdata += bytesperpixel * count;
            }
        }
    }
    else
    {
        if(!fread(data, lenght - sizeof(header), 1, fp))
        {
            fclose(fp);
        }
    }

    fclose(fp);

    switch(header.bpp)
    {
    case 8:
        *_iformat = GL_RGB8;
        *_format = GL_BGR;
        break;
    case 24:
        *_iformat = GL_RGB8;
        *_format = GL_BGR;
        break;
    case 32:
        *_iformat = GL_RGBA8;
        *_format = GL_BGRA;
        break;
    }

    *_width = header.width;
    *_height = header.height;

    return data;
}
