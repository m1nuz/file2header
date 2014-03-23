#ifndef WGLEXTENSIONS_H
#define WGLEXTENSIONS_H

#define WIN32_MEAN_AND_LEAN
#include <windows.h>
#include <gl3/gl3core.h>

#define WGL_GET_ENTRYPOINT(name) wglGetProcAddress(name)

#ifdef __cplusplus
extern "C"
{
#endif

    //
    // WGL_ARB_create_context
    // http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
    //
    #define WGL_CONTEXT_DEBUG_BIT_ARB      0x00000001
    #define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
    #define WGL_CONTEXT_MAJOR_VERSION_ARB  0x2091
    #define WGL_CONTEXT_MINOR_VERSION_ARB  0x2092
    #define WGL_CONTEXT_LAYER_PLANE_ARB    0x2093
    #define WGL_CONTEXT_FLAGS_ARB          0x2094
    #define ERROR_INVALID_VERSION_ARB      0x2095

    //
    // WGL_ARB_create_context_profile
    // http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
    //
    #define WGL_CONTEXT_PROFILE_MASK_ARB   0x9126
    #define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
    #define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
    #define ERROR_INVALID_PROFILE_ARB      0x2096

    extern HGLRC wglCreateContextAttribsARB(HDC hDC, HGLRC hShareContext, const int *attribList);

#ifdef __cplusplus
}
#endif

#endif // WGLEXTENSIONS_H
