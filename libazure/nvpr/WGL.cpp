/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "WGL.h"
#include "Logging.h"
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
  gl = new WGL();
}

template<typename C, typename T> bool
WGL::LoadProcAddress(T C::*aProc, const char* aName)
{
  PROC proc = GetProcAddress(aName);
  if (!proc) {
    // The GL 1.1 functions have to be loaded directly from the dll.
    proc = ::GetProcAddress(mGLLibrary, aName);
  }
  if (!proc) {
    gfxWarning() << "Failed to load GL function " << aName << ".";
    this->*aProc = nullptr;
    return false;
  }
  this->*aProc = reinterpret_cast<T>(proc);
  return true;
}

WGL::WGL()
  : mGLLibrary(0)
  , mHWnd(0)
  , mDC(0)
  , mContext(0)
{
  memset(mSupportedWGLExtensions, 0, sizeof(mSupportedWGLExtensions));

  mGLLibrary = ::LoadLibrary("opengl32.dll");
  if (!mGLLibrary) {
    return;
  }

#define LOAD_WGL_METHOD(NAME) \
  NAME = reinterpret_cast<decltype(NAME)>(::GetProcAddress(mGLLibrary, "wgl"#NAME)); \
  if (!NAME) { \
    gfxWarning() << "Failed to find WGL function " #NAME "."; \
    return; \
  }

  FOR_ALL_WGL_ENTRY_POINTS(LOAD_WGL_METHOD);

#undef LOAD_WGL_METHOD

  HINSTANCE inst = (HINSTANCE)GetModuleHandle(NULL);

  WNDCLASS wc;
  memset(&wc, 0, sizeof(wc));
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hInstance = inst;
  wc.lpfnWndProc = (WNDPROC) DefWindowProc;
  wc.lpszClassName = TEXT("DummyWindow");
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

  if (!RegisterClass(&wc)) {
    return;
  }

  if (!(mHWnd = CreateWindow(TEXT("DummyWindow"),
                             TEXT("Dummy OGL Window"),
                             WS_OVERLAPPEDWINDOW,
                             0, 0, 1, 1,
                             NULL, NULL,
                             inst, NULL))) {
    return;
  }

  mDC = ::GetDC(mHWnd);

  PIXELFORMATDESCRIPTOR pfd;

  memset(&pfd, 0, sizeof(pfd));
  pfd.nSize = sizeof(pfd);
  pfd.dwFlags = PFD_SUPPORT_OPENGL;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 0;
  pfd.cStencilBits = 0;
  pfd.iLayerType = PFD_MAIN_PLANE;

  // get the best available match of pixel format for the device context
  int iPixelFormat = ChoosePixelFormat(mDC, &pfd);

  // make that the pixel format of the device context
  SetPixelFormat(mDC, iPixelFormat, &pfd);

  mContext = CreateContext(mDC);

  DWORD lastError = ::GetLastError();

  GL::MakeCurrent();

  if (!LoadProcAddress(&WGL::GetExtensionsStringARB, "wglGetExtensionsStringARB")) {
    return;
  }

  stringstream extensions(GetExtensionsStringARB(mDC));
  istream_iterator<string> iter(extensions);
  istream_iterator<string> end;

  for (; iter != end; iter++) {
    const string& extension = *iter;

    if (*iter == "WGL_NV_copy_image") {
      if (LoadProcAddress(&WGL::CopyImageSubDataNV, "wglCopyImageSubDataNV")) {
        mSupportedWGLExtensions[NV_copy_image] = true;
      }
      continue;
    }

    if (*iter == "WGL_NV_DX_interop2") {
      if (LoadProcAddress(&WGL::DXOpenDeviceNV, "wglDXOpenDeviceNV")
          && LoadProcAddress(&WGL::DXCloseDeviceNV, "wglDXCloseDeviceNV")
          && LoadProcAddress(&WGL::DXRegisterObjectNV, "wglDXRegisterObjectNV")
          && LoadProcAddress(&WGL::DXUnregisterObjectNV, "wglDXUnregisterObjectNV")
          && LoadProcAddress(&WGL::DXLockObjectsNV, "wglDXLockObjectsNV")
          && LoadProcAddress(&WGL::DXUnlockObjectsNV, "wglDXUnlockObjectsNV")) {
        mSupportedWGLExtensions[NV_DX_interop2] = true;
      }
      continue;
    }
  }

#define LOAD_GL_METHOD(NAME) \
  if (!LoadProcAddress(&WGL::NAME, "gl"#NAME)) { \
    return; \
  }

  FOR_ALL_PUBLIC_GL_ENTRY_POINTS(LOAD_GL_METHOD);
  FOR_ALL_PRIVATE_GL_ENTRY_POINTS(LOAD_GL_METHOD);

#undef LOAD_GL_METHOD

  Initialize();
}

WGL::~WGL()
{
  if (mDC) {
    MakeCurrent(mDC, nullptr);
  }

  if (mContext) {
    DeleteContext(mContext);
  }

  if (mDC) {
    ::ReleaseDC(mHWnd, mDC);
  }

  if (mHWnd) {
    DestroyWindow(mHWnd);
  }

  if (mGLLibrary) {
    ::FreeLibrary(mGLLibrary);
  }
}

bool GL::IsCurrent() const
{
  const WGL* const wgl = static_cast<const WGL*>(this);

  return wgl->GetCurrentContext() == wgl->Context();
}

void GL::MakeCurrent() const
{
  const WGL* const wgl = static_cast<const WGL*>(this);

  if (wgl->IsCurrent()) {
    return;
  }

  wgl->MakeCurrent(wgl->DC(), wgl->Context());
}

bool
GL::BlitTextureToForeignTexture(const IntSize& aSize, GLuint aSourceTextureId,
                                void* aForeignContext, GLuint aForeignTextureId)
{
  WGL* const wgl = static_cast<WGL*>(this);

  if (!wgl->HasWGLExtension(WGL::NV_copy_image)) {
    return false;
  }

  wgl->CopyImageSubDataNV(wgl->Context(), aSourceTextureId, GL_TEXTURE_2D, 0, 0, 0, 0,
                          static_cast<HGLRC>(aForeignContext), aForeignTextureId,
                          GL_TEXTURE_2D, 0, 0, 0, 0, aSize.width, aSize.height, 1);

  return true;
}

}
}
}
