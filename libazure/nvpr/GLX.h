/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_NVPR_GLX_H_
#define MOZILLA_GFX_NVPR_GLX_H_

#include "GL.h"
#include "GLXDefs.h"

namespace mozilla {
namespace gfx {
namespace nvpr {

class GLX : public GL
{
public:
  GLX();
  virtual ~GLX();

  ::Display* Display() const { return mDisplay; }
  GLXPixmap Drawable() const { return mGLXPixmap; }
  GLXContext Context() const { return mContext; }

  enum GLXExtension {
    NV_copy_image,
    GLX_EXTENSION_COUNT
  };
  bool HasGLXExtension(GLXExtension aExtension) const
  {
    return mSupportedGLXExtensions[aExtension];
  }

#define FOR_ALL_GLX_ENTRY_POINTS(MACRO) \
  MACRO(ChooseFBConfig) \
  MACRO(GetVisualFromFBConfig) \
  MACRO(CreateGLXPixmap) \
  MACRO(DestroyGLXPixmap) \
  MACRO(CreateContext) \
  MACRO(DestroyContext) \
  MACRO(GetProcAddress) \
  MACRO(GetCurrentContext) \
  MACRO(MakeCurrent) \
  MACRO(QueryExtensionsString)

#define DECLARE_GLX_METHOD(NAME) \
  GLX##NAME NAME;

  FOR_ALL_GLX_ENTRY_POINTS(DECLARE_GLX_METHOD);

#undef DECLARE_GLX_METHOD

  GLXCopyImageSubDataNV CopyImageSubDataNV;

private:
  template<typename C, typename T> bool
    LoadProcAddress(T C::*aProc, const char* aName);

  void* mLibGL;
  ::Display* mDisplay;
  Pixmap mPixmap;
  GLXPixmap mGLXPixmap;
  GLXContext mContext;
  bool mSupportedGLXExtensions[GLX_EXTENSION_COUNT];
};

}
}
}

#endif /* MOZILLA_GFX_NVPR_GLX_H_ */
