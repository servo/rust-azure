/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "DXTextureInteropNVpr.h"
#include "nvpr/WGL.h"

using namespace mozilla::gfx::nvpr;
using namespace std;

namespace mozilla {
namespace gfx {

DXTextureInteropNVpr::DXTextureInteropNVpr(void* aDX, void* aDXTexture,
                                           bool& aSuccess)
  : mDXInterop(nullptr)
  , mTextureInterop(nullptr)
  , mTextureId(0)
{
  WGL* const wgl = static_cast<WGL*>(gl);
  gl->MakeCurrent();

  aSuccess = false;

  if (!wgl->HasWGLExtension(WGL::NV_DX_interop2)) {
    return;
  }

  mDXInterop = wgl->DXOpenDeviceNV(aDX);
  if (!mDXInterop) {
    return;
  }

  gl->GenTextures(1, &mTextureId);
  mTextureInterop = wgl->DXRegisterObjectNV(mDXInterop, aDXTexture,
                                            mTextureId, GL_TEXTURE_2D,
                                            WGL_ACCESS_WRITE_DISCARD_NV);
  if (mTextureInterop) {
    return;
  }

  aSuccess = true;
}

DXTextureInteropNVpr::~DXTextureInteropNVpr()
{
  WGL* const wgl = static_cast<WGL*>(gl);
  gl->MakeCurrent();

  if (mTextureInterop) {
    MOZ_ASSERT(wgl->HasWGLExtension(WGL::NV_DX_interop2));
    wgl->DXUnregisterObjectNV(mDXInterop, mTextureInterop);
  }

  if (mTextureId) {
    gl->DeleteTexture(mTextureId);
  }

  if (mDXInterop) {
    MOZ_ASSERT(wgl->HasWGLExtension(WGL::NV_DX_interop2));
    wgl->DXCloseDeviceNV(mDXInterop);
  }
}

GLuint
DXTextureInteropNVpr::Lock()
{
  MOZ_ASSERT(gl->IsCurrent());

  WGL* const wgl = static_cast<WGL*>(gl);
  MOZ_ASSERT(wgl->HasWGLExtension(WGL::NV_DX_interop2));

  wgl->DXLockObjectsNV(mDXInterop, 1, &mTextureInterop);

  return mTextureId;
}

void
DXTextureInteropNVpr::Unlock()
{
  MOZ_ASSERT(gl->IsCurrent());

  WGL* const wgl = static_cast<WGL*>(gl);
  MOZ_ASSERT(wgl->HasWGLExtension(WGL::NV_DX_interop2));

  wgl->DXUnlockObjectsNV(mDXInterop, 1, &mTextureInterop);
}

}
}
