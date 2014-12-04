/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_SOURCESURFACENVPR_H_
#define MOZILLA_GFX_SOURCESURFACENVPR_H_

#include "2D.h"
#include "nvpr/GL.h"
#include <mozilla/RefPtr.h>
#include <mozilla/WeakPtr.h>
#include <vector>

namespace mozilla {
namespace gfx {

class DataSourceSurfaceNVpr;

class TextureObjectNVpr : public RefCounted<TextureObjectNVpr>
{
  friend class DataSourceSurfaceNVpr;
  enum NotifyDataSurface { NOTIFY_DATA_SURFACE, DO_NOT_NOTIFY_DATA_SURFACE };

public:
  static TemporaryRef<TextureObjectNVpr>
  Create(DataSourceSurface* aData);

  static TemporaryRef<TextureObjectNVpr>
  Create(SurfaceFormat aFormat, const IntSize& aSize,
         const GLvoid* aData = nullptr, GLsizei aStride = 0);

  ~TextureObjectNVpr();

  SurfaceFormat Format() const { return mFormat; }
  IntSize Size() const { return mSize; }
  GLsizei BytesPerPixel() const { return mBytesPerPixel; }
  operator GLuint() const { return mTextureId; }

  void SetWrapMode(ExtendMode aExtendMode);
  void SetWrapMode(GLenum aWrapMode);
  void SetFilter(Filter aFilter);

  TemporaryRef<DataSourceSurface> GetDataSurface();

  // Use this method when the texture contents are changed without the
  // DataSurface (i.e. by using direct OpenGL calls).
  void MarkChanged() { MarkChanged(NOTIFY_DATA_SURFACE); }

private:
  TextureObjectNVpr(SurfaceFormat aFormat, const IntSize& aSize,
                    bool& aSuccess);

  void WritePixels(const GLvoid* aData, GLsizei aStride,
                   NotifyDataSurface aNotifyDataSurface = NOTIFY_DATA_SURFACE);
  void ReadPixels(GLvoid* aBuffer);

  void MarkChanged(NotifyDataSurface aNotifyDataSurface);

  const SurfaceFormat mFormat;
  const IntSize mSize;
  GLenum mGLFormat;
  GLenum mGLType;
  GLsizei mBytesPerPixel;
  GLuint mTextureId;
  GLenum mWrapMode;
  Filter mFilter;
  bool mHasMipmaps;
  WeakPtr<DataSourceSurfaceNVpr> mDataSurface;
};

class SourceSurfaceNVpr : public SourceSurface {
public:
  SourceSurfaceNVpr(TemporaryRef<TextureObjectNVpr> aTexture)
    : mTexture(aTexture)
  {}

  TextureObjectNVpr* Texture() const { return mTexture; }
  operator GLuint() const { return *mTexture; }

  void SetWrapMode(ExtendMode aExtendMode) { mTexture->SetWrapMode(aExtendMode); }
  void SetWrapMode(GLenum aWrapMode) { mTexture->SetWrapMode(aWrapMode); }
  void SetFilter(Filter aFilter) { mTexture->SetFilter(aFilter); }

  virtual SurfaceType GetType() const { return SurfaceType::NVPR_TEXTURE; }
  virtual SurfaceFormat GetFormat() const { return mTexture->Format(); }
  virtual IntSize GetSize() const { return mTexture->Size(); }

  void MarkChanged() { mTexture->MarkChanged(); }

  virtual TemporaryRef<DataSourceSurface> GetDataSurface() {
    return mTexture->GetDataSurface();
  }

private:
  RefPtr<TextureObjectNVpr> mTexture;
};

class DataSourceSurfaceNVpr
  : public DataSourceSurface
  , public SupportsWeakPtr<DataSourceSurfaceNVpr>
{
public:
  DataSourceSurfaceNVpr(TemporaryRef<TextureObjectNVpr> aTexture)
    : mTexture(aTexture)
  {}

  virtual IntSize GetSize() const { return mTexture->Size(); }
  virtual SurfaceFormat GetFormat() const { return mTexture->Format(); }

  virtual unsigned char* GetData();
  virtual int32_t Stride();

  virtual void MarkDirty();

  void OnTextureChanged();

private:
  RefPtr<TextureObjectNVpr> mTexture;
  std::vector<GLubyte> mShadowBuffer;
};

}
}

#endif /* MOZILLA_GFX_SOURCESURFACENVPR_H_ */
