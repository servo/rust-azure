/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_NVPR_GLXDEFS_H_
#define MOZILLA_GFX_NVPR_GLXDEFS_H_

#include "GLDefs.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

// GLX core
typedef struct __GLXcontextRec *GLXContext;
typedef XID GLXPixmap;
typedef XID GLXDrawable;
typedef struct __GLXFBConfigRec *GLXFBConfig;
typedef XID GLXFBConfigID;
typedef XID GLXContextID;
typedef XID GLXWindow;
typedef XID GLXPbuffer;

#define GLX_USE_GL 1
#define GLX_BUFFER_SIZE 2
#define GLX_LEVEL 3
#define GLX_RGBA 4
#define GLX_DOUBLEBUFFER 5
#define GLX_STEREO 6
#define GLX_AUX_BUFFERS 7
#define GLX_RED_SIZE 8
#define GLX_GREEN_SIZE 9
#define GLX_BLUE_SIZE 10
#define GLX_ALPHA_SIZE 11
#define GLX_DEPTH_SIZE 12
#define GLX_STENCIL_SIZE 13
#define GLX_ACCUM_RED_SIZE 14
#define GLX_ACCUM_GREEN_SIZE 15
#define GLX_ACCUM_BLUE_SIZE 16
#define GLX_ACCUM_ALPHA_SIZE 17
#define GLX_BAD_SCREEN 1
#define GLX_BAD_ATTRIBUTE 2
#define GLX_NO_EXTENSION 3
#define GLX_BAD_VISUAL 4
#define GLX_BAD_CONTEXT 5
#define GLX_BAD_VALUE 6
#define GLX_BAD_ENUM 7
#define GLX_VENDOR 1
#define GLX_VERSION 2
#define GLX_EXTENSIONS 3
#define GLX_CONFIG_CAVEAT 0x20
#define GLX_DONT_CARE 0xFFFFFFFF
#define GLX_X_VISUAL_TYPE 0x22
#define GLX_TRANSPARENT_TYPE 0x23
#define GLX_TRANSPARENT_INDEX_VALUE 0x24
#define GLX_TRANSPARENT_RED_VALUE 0x25
#define GLX_TRANSPARENT_GREEN_VALUE 0x26
#define GLX_TRANSPARENT_BLUE_VALUE 0x27
#define GLX_TRANSPARENT_ALPHA_VALUE 0x28
#define GLX_WINDOW_BIT 0x00000001
#define GLX_PIXMAP_BIT 0x00000002
#define GLX_PBUFFER_BIT 0x00000004
#define GLX_AUX_BUFFERS_BIT 0x00000010
#define GLX_FRONT_LEFT_BUFFER_BIT 0x00000001
#define GLX_FRONT_RIGHT_BUFFER_BIT 0x00000002
#define GLX_BACK_LEFT_BUFFER_BIT 0x00000004
#define GLX_BACK_RIGHT_BUFFER_BIT 0x00000008
#define GLX_DEPTH_BUFFER_BIT 0x00000020
#define GLX_STENCIL_BUFFER_BIT 0x00000040
#define GLX_ACCUM_BUFFER_BIT 0x00000080
#define GLX_NONE 0x8000
#define GLX_SLOW_CONFIG 0x8001
#define GLX_TRUE_COLOR 0x8002
#define GLX_DIRECT_COLOR 0x8003
#define GLX_PSEUDO_COLOR 0x8004
#define GLX_STATIC_COLOR 0x8005
#define GLX_GRAY_SCALE 0x8006
#define GLX_STATIC_GRAY 0x8007
#define GLX_TRANSPARENT_RGB 0x8008
#define GLX_TRANSPARENT_INDEX 0x8009
#define GLX_VISUAL_ID 0x800B
#define GLX_SCREEN 0x800C
#define GLX_NON_CONFORMANT_CONFIG 0x800D
#define GLX_DRAWABLE_TYPE 0x8010
#define GLX_RENDER_TYPE 0x8011
#define GLX_X_RENDERABLE 0x8012
#define GLX_FBCONFIG_ID 0x8013
#define GLX_RGBA_TYPE 0x8014
#define GLX_COLOR_INDEX_TYPE 0x8015
#define GLX_MAX_PBUFFER_WIDTH 0x8016
#define GLX_MAX_PBUFFER_HEIGHT 0x8017
#define GLX_MAX_PBUFFER_PIXELS 0x8018
#define GLX_PRESERVED_CONTENTS 0x801B
#define GLX_LARGEST_PBUFFER 0x801C
#define GLX_WIDTH 0x801D
#define GLX_HEIGHT 0x801E
#define GLX_EVENT_MASK 0x801F
#define GLX_DAMAGED 0x8020
#define GLX_SAVED 0x8021
#define GLX_WINDOW 0x8022
#define GLX_PBUFFER 0x8023
#define GLX_PBUFFER_HEIGHT 0x8040
#define GLX_PBUFFER_WIDTH 0x8041
#define GLX_RGBA_BIT 0x00000001
#define GLX_COLOR_INDEX_BIT 0x00000002
#define GLX_PBUFFER_CLOBBER_MASK 0x08000000
#define GLX_SAMPLE_BUFFERS 0x186a0
#define GLX_SAMPLES 0x186a1

typedef XVisualInfo* (* GLXChooseVisual) (Display *dpy, int screen, int *attribList);
typedef GLXContext (* GLXCreateContext) (Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct);
typedef void (* GLXDestroyContext) (Display *dpy, GLXContext ctx);
typedef Bool (* GLXMakeCurrent) (Display *dpy, GLXDrawable drawable, GLXContext ctx);
typedef void (* GLXCopyContext) (Display *dpy, GLXContext src, GLXContext dst, unsigned long mask);
typedef void (* GLXSwapBuffers) (Display *dpy, GLXDrawable drawable);
typedef GLXPixmap (* GLXCreateGLXPixmap) (Display *dpy, XVisualInfo *visual, Pixmap pixmap);
typedef void (* GLXDestroyGLXPixmap) (Display *dpy, GLXPixmap pixmap);
typedef Bool (* GLXQueryExtension) (Display *dpy, int *errorb, int *event);
typedef Bool (* GLXQueryVersion) (Display *dpy, int *maj, int *min);
typedef Bool (* GLXIsDirect) (Display *dpy, GLXContext ctx);
typedef int (* GLXGetConfig) (Display *dpy, XVisualInfo *visual, int attrib, int *value);
typedef GLXContext (* GLXGetCurrentContext) (void);
typedef GLXDrawable (* GLXGetCurrentDrawable) (void);
typedef void (* GLXWaitGL) (void);
typedef void (* GLXWaitX) (void);
typedef void (* GLXUseXFont) (Font font, int first, int count, int list);
typedef const char * (* GLXQueryExtensionsString) (Display *dpy, int screen);
typedef const char * (* GLXQueryServerString) (Display *dpy, int screen, int name);
typedef const char * (* GLXGetClientString) (Display *dpy, int name);
typedef Display * (* GLXGetCurrentDisplay) (void);
typedef GLXFBConfig * (* GLXChooseFBConfig) (Display *dpy, int screen, const int *attribList, int *nitems);
typedef int (* GLXGetFBConfigAttrib) (Display *dpy, GLXFBConfig config, int attribute, int *value);
typedef GLXFBConfig * (* GLXGetFBConfigs) (Display *dpy, int screen, int *nelements);
typedef XVisualInfo * (* GLXGetVisualFromFBConfig) (Display *dpy, GLXFBConfig config);
typedef GLXWindow (* GLXCreateWindow) (Display *dpy, GLXFBConfig config, Window win, const int *attribList);
typedef void (* GLXDestroyWindow) (Display *dpy, GLXWindow window);
typedef GLXPixmap (* GLXCreatePixmap) (Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList);
typedef void (* GLXDestroyPixmap) (Display *dpy, GLXPixmap pixmap);
typedef GLXPbuffer (* GLXCreatePbuffer) (Display *dpy, GLXFBConfig config, const int *attribList);
typedef void (* GLXDestroyPbuffer) (Display *dpy, GLXPbuffer pbuf);
typedef void (* GLXQueryDrawable) (Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
typedef GLXContext (* GLXCreateNewContext) (Display *dpy, GLXFBConfig config, int renderType, GLXContext shareList, Bool direct);
typedef Bool (* GLXMakeContextCurrent) (Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
typedef GLXDrawable (* GLXGetCurrentReadDrawable) (void);
typedef int (* GLXQueryContext) (Display *dpy, GLXContext ctx, int attribute, int *value);
typedef void (* GLXSelectEvent) (Display *dpy, GLXDrawable drawable, unsigned long mask);
typedef void (* GLXGetSelectedEvent) (Display *dpy, GLXDrawable drawable, unsigned long *mask);
typedef void (* (* GLXGetProcAddress) (const GLubyte *procname))(void);


// GLX_NV_copy_image
typedef void (* GLXCopyImageSubDataNV) (Display *dpy, GLXContext srcCtx, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLXContext dstCtx, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);


#endif /* MOZILLA_GFX_NVPR_GLXDEFS_H_ */
