/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GLX.h"
#include "Logging.h"
#include <dlfcn.h>
#include <iterator>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace mozilla {
namespace gfx {
namespace nvpr {

void
InitializeGLIfNeeded()
{
  if (gl) {
    return;
  }
  gl = new GLX();
}

template<typename C, typename T> bool
GLX::LoadProcAddress(T C::*aProc, const char* aName)
{
  void (*proc)() = GetProcAddress(reinterpret_cast<const GLubyte*>(aName));
  if (!proc) {
    gfxWarning() << "Failed to load GL function " << aName << ".";
    proc = nullptr;
    return false;
  }
  this->*aProc = reinterpret_cast<T>(proc);
  return true;
}

GLX::GLX()
  : mLibGL(nullptr)
  , mDisplay(nullptr)
  , mPixmap(0)
  , mGLXPixmap(0)
  , mContext(nullptr)
{
  memset(mSupportedGLXExtensions, 0, sizeof(mSupportedGLXExtensions));

  mLibGL = dlopen("libGL.so", RTLD_LAZY);
  if (!mLibGL) {
    return;
  }

#define LOAD_GLX_METHOD(NAME) \
  NAME = reinterpret_cast<decltype(NAME)>(dlsym(mLibGL, "glX"#NAME)); \
  if (!NAME) { \
    gfxWarning() << "Failed to find GLX function " #NAME "."; \
    return; \
  }

  FOR_ALL_GLX_ENTRY_POINTS(LOAD_GLX_METHOD);

#undef LOAD_GLX_METHOD

  mDisplay = XOpenDisplay(0);

  int nelements;
  int screen = DefaultScreen(mDisplay);
  GLXFBConfig *fbc = ChooseFBConfig(mDisplay, screen, 0, &nelements);
  XVisualInfo *vi = GetVisualFromFBConfig(mDisplay, fbc[0]);

  mPixmap = XCreatePixmap(mDisplay, RootWindow(mDisplay, vi->screen),
                              10, 10, vi->depth);
  mGLXPixmap = CreateGLXPixmap(mDisplay, vi, mPixmap);

  mContext = CreateContext(mDisplay, vi, 0, true);

  GL::MakeCurrent();

  stringstream extensions(QueryExtensionsString(mDisplay, screen));
  istream_iterator<string> iter(extensions);
  istream_iterator<string> end;

  for (; iter != end; iter++) {
    const string& extension = *iter;

    if (*iter == "GLX_NV_copy_image") {
      if (LoadProcAddress(&GLX::CopyImageSubDataNV, "glXCopyImageSubDataNV")) {
        mSupportedGLXExtensions[NV_copy_image] = true;
      }
      break;
    }
  }

#define LOAD_GL_METHOD(NAME) \
  if (!LoadProcAddress(&GLX::NAME, "gl"#NAME)) { \
    return; \
  }

  FOR_ALL_PUBLIC_GL_ENTRY_POINTS(LOAD_GL_METHOD);
  FOR_ALL_PRIVATE_GL_ENTRY_POINTS(LOAD_GL_METHOD);

#undef LOAD_GL_METHOD

  Initialize();
}

GLX::~GLX()
{
  if (mDisplay) {
    MakeCurrent(mDisplay, 0, 0);
  }

  if (mContext) {
    DestroyContext(mDisplay, mContext);
  }

  if (mGLXPixmap) {
    DestroyGLXPixmap(mDisplay, mGLXPixmap);
  }

  if (mPixmap) {
    XFreePixmap(mDisplay, mPixmap);
  }

  if (mDisplay) {
    XCloseDisplay(mDisplay);
  }

  if (mLibGL) {
    dlclose(mLibGL);
  }
}

bool
GL::IsCurrent() const
{
  const GLX* const glx = static_cast<const GLX*>(this);

  return glx->GetCurrentContext() == glx->Context();
}

void
GL::MakeCurrent() const
{
  const GLX* const glx = static_cast<const GLX*>(this);

  if (IsCurrent()) {
    return;
  }

  glx->MakeCurrent(glx->Display(), glx->Drawable(), glx->Context());
}

bool
GL::BlitTextureToForeignTexture(const IntSize& aSize, GLuint aSourceTextureId,
                                void* aForeignContext, GLuint aForeignTextureId)
{
  GLX* const glx = static_cast<GLX*>(this);

  if (!glx->HasGLXExtension(GLX::NV_copy_image)) {
    return false;
  }

  glx->CopyImageSubDataNV(glx->Display(), glx->Context(), aSourceTextureId,
                          GL_TEXTURE_2D, 0, 0, 0, 0,
                          static_cast<GLXContext>(aForeignContext),
                          aForeignTextureId, GL_TEXTURE_2D, 0, 0, 0, 0,
                          aSize.width, aSize.height, 1);

  return true;
}

}
}
}
