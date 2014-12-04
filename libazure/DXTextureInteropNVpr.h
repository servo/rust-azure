/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_DXTEXTUREINTEROP_H_
#define MOZILLA_GFX_DXTEXTUREINTEROP_H_

#include "nvpr/GL.h"
#include "Windows.h"
#include <mozilla/RefPtr.h>

namespace mozilla {
namespace gfx {

class DXTextureInteropNVpr : public RefCounted<DXTextureInteropNVpr> {
public:
  static TemporaryRef<DXTextureInteropNVpr> Create(void* aDX, void* aDXTexture)
  {
    bool success;
    RefPtr<DXTextureInteropNVpr> interop =
      new DXTextureInteropNVpr(aDX, aDXTexture, success);
    return success ? interop.forget() : nullptr;
  }
  ~DXTextureInteropNVpr();

  GLuint Lock();
  void Unlock();

private:
  DXTextureInteropNVpr(void* aDX, void* aDXTexture, bool& aSuccess);
  HANDLE mDXInterop;
  GLuint mTextureId;
  HANDLE mTextureInterop;
};

}
}

#endif /* MOZILLA_GFX_DXTEXTUREINTEROP_H_ */
