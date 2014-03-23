#pragma once

#include <GL/glx.h>
#include <GL/glxext.h>

#define GLX_GET_ENTRYPOINT(name) glXGetProcAddress((GLubyte*)name)

#ifdef __cplusplus
extern "C"
{
#endif

extern GLXContext glXCreateContextAttribsARB (Display *dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list);

#ifdef __cplusplus
}
#endif
