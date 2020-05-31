#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "targa.h"
#include "wave.h"
#include "wavefront.h"

struct {
    GLuint iformat;
    GLenum format;
    GLsizei width;
    GLsizei height;
    uint8_t *data;
} tga;

struct {
    ALenum format;
    ALsizei frequency;
    ALsizei size;
    uint8_t *data;
} wave;

struct {
    float *vertices;
    int vertices_num;
    unsigned short *indices;
    int indices_num;
} obj;

void save_targa_to_header( const char *filename, const char *name ) {
    FILE *fp = fopen( filename, "w" );

    fprintf( fp, "#pragma once\n" );

    char s[10] = {};

    if ( tga.format == GL_BGR )
        strcpy( s, "GL_BGR" );

    if ( tga.format == GL_BGRA )
        strcpy( s, "GL_BGR" );

    fprintf( fp, "const GLenum %s_format = %s;\n", name, s );

    if ( tga.iformat == GL_RGB8 )
        strcpy( s, "GL_RGB8" );

    if ( tga.format == GL_RGBA8 )
        strcpy( s, "GL_RGBA8" );

    fprintf( fp, "const GLuint %s_internal_format = %s;\n", name, s );

    fprintf( fp, "const GLsizei %s_width = %d;\n", name, tga.width );
    fprintf( fp, "const GLsizei %s_height = %d;\n", name, tga.height );

    int bpp = 3;

    if ( tga.iformat == GL_RGBA8 )
        bpp = 4;

    fprintf( fp, "const unsigned char %s_data[] = {\n", name );
    for ( unsigned i = 0; i < tga.width * tga.height * bpp; i++ ) {
        if ( ( i != 0 ) && ( ( i % bpp ) == 0 ) )
            fprintf( fp, "\n" );

        fprintf( fp, "%#x, ", tga.data[i] );
    }
    fprintf( fp, "};\n" );

    fclose( fp );
}

void save_wave_to_header( const char *filename, const char *name ) {
    FILE *fp = fopen( filename, "w" );

    fprintf( fp, "#pragma once\n" );

    char s[40] = {};

    int depth = 0;

    switch ( wave.format ) {
    case AL_FORMAT_MONO8:
        strcpy( s, "AL_FORMAT_MONO8" );
        depth = 1;
        break;
    case AL_FORMAT_MONO16:
        strcpy( s, "AL_FORMAT_MONO16" );
        depth = 2;
        break;
    case AL_FORMAT_STEREO8:
        strcpy( s, "AL_FORMAT_STEREO8" );
        depth = 1;
        break;
    case AL_FORMAT_STEREO16:
        strcpy( s, "AL_FORMAT_STEREO16" );
        depth = 2;
        break;
    }

    fprintf( fp, "const ALenum %s_format = %s;\n", name, s );
    fprintf( fp, "const ALsizei %s_frequency = %d;\n", name, wave.frequency );
    fprintf( fp, "const ALsizei %s_size = %d;\n", name, wave.size );

    fprintf( fp, "const unsigned char %s_data[] = {\n", name );

    for ( int i = 0; i < wave.size; i++ ) {
        if ( ( i != 0 ) && ( i % 10 ) == 0 )
            fprintf( fp, "\n" );

        fprintf( fp, "%#x, ", wave.data[i] );
    }

    fprintf( fp, "};\n" );

    fclose( fp );
}

void save_obj_to_header( const char *filename, const char *name ) {
    FILE *fp = fopen( filename, "w" );

    fprintf( fp, "#pragma once\n" );

    fprintf( fp, "const float %s_vertices[] = {\n", name );

    for ( int i = 0; i < obj.vertices_num * 3; i++ ) {
        if ( ( i != 0 ) && ( i % 3 ) == 0 )
            fprintf( fp, "\n" );

        fprintf( fp, "%4.6f, ", obj.vertices[i] );
    }

    fprintf( fp, "};\n" );

    fprintf( fp, "const unsigned short %s_indices[] = {\n", name );

    for ( int i = 0; i < obj.indices_num; i++ ) {
        if ( ( i != 0 ) && ( i % 3 ) == 0 )
            fprintf( fp, "\n" );

        fprintf( fp, "%4hd, ", obj.indices[i] );
    }

    fprintf( fp, "};\n" );

    fprintf( fp, "const int %s_vertices_num = %d;\n", name, obj.vertices_num );
    fprintf( fp, "const int %s_indices_num = %d;\n", name, obj.indices_num );

    fclose( fp );
}

_Noreturn static void print_usage_and_exit( const char *app ) {
    char *name = strrchr( app, '/' );
    if ( name ) {
        fprintf( stderr, "Usage:\n\t%s <-tga|-wave|-obj> [input file] <output file>\n", name + 1 );
    }
    exit( EXIT_FAILURE );
}

typedef enum { OPT_NONE, OPT_TGA, OPT_WAVE, OPT_OBJ } Options;

int main( int argc, char *argv[] ) {
    Options options = OPT_NONE;

    char outname[260] = {};
    char inname[260] = {};
    char varname[260] = {};

    int opt_index = 0;
    bool first_name = true;
    for ( opt_index = 1; opt_index < argc; opt_index++ ) {
        if ( argv[opt_index][0] == '-' ) {
            if ( strcmp( &argv[opt_index][1], "tga" ) == 0 ) {
                options = OPT_TGA;
            } else if ( strcmp( &argv[opt_index][1], "wave" ) == 0 ) {
                options = OPT_WAVE;
            } else if ( strcmp( &argv[opt_index][1], "obj" ) == 0 ) {
                options = OPT_OBJ;
            } else {
                print_usage_and_exit( argv[0] );
            }
        } else {
            if ( first_name ) {
                strcpy( inname, argv[opt_index] );
                first_name = false;
            } else {
                strcpy( outname, argv[opt_index] );
            }
        }
    }

    if ( inname[0] == 0 )
        print_usage_and_exit( argv[0] );

    if ( inname[0] != 0 && options == OPT_NONE ) {
        const char *ext = strrchr( inname, '.' );
        if ( !ext )
            exit( EXIT_FAILURE );

        if ( strcmp( ext, ".tga" ) == 0 ) {
            options = OPT_TGA;
        } else if ( strcmp( ext, ".wave" ) == 0 || strcmp( ext, ".wav" ) ) {
            options = OPT_WAVE;
        } else if ( strcmp( ext, ".obj" ) == 0 ) {
            options = OPT_OBJ;
        }
    }

    if ( options == OPT_NONE )
        print_usage_and_exit( argv[0] );

    if ( outname[0] == 0 ) {
        const char *name = strrchr( inname, '/' );
        if ( !name )
            exit( EXIT_FAILURE );
        const char *ext = strrchr( name, '.' );
        if ( !ext )
            exit( EXIT_FAILURE );

        strncpy( outname, name + 1, ( size_t )( ext - name - 1 ) );
        strcpy( varname, outname );
        strcat( outname, ".h" );
    }

    if ( options == OPT_TGA ) {
        tga.data = (uint8_t *)load_targa( inname, &tga.iformat, &tga.format, &tga.width, &tga.height );

        save_targa_to_header( outname, varname );

        free( tga.data );
    } else if ( options == OPT_WAVE ) {
        wave.data = (uint8_t *)load_wave( inname, &wave.format, &wave.frequency, &wave.size );

        save_wave_to_header( outname, varname );

        free( wave.data );
    } else {
        load_wavefront( inname, &obj.vertices, &obj.vertices_num, &obj.indices, &obj.indices_num );

        save_obj_to_header( outname, varname );

        free( obj.vertices );
        free( obj.indices );
    }

    return EXIT_SUCCESS;
}
