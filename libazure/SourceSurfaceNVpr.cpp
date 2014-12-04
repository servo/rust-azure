/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SourceSurfaceNVpr.h"

using namespace mozilla::gfx::nvpr;
using namespace std;

namespace mozilla {
namespace gfx {

TextureObjectNVpr::TextureObjectNVpr(SurfaceFormat aFormat, const IntSize& aSize,
                                     bool& aSuccess)
  : mFormat(aFormat)
  , mSize(aSize)
  , mTextureId(0)
  , mWrapMode(GL_REPEAT)
  , mFilter(Filter::LINEAR)
  , mHasMipmaps(false)
{
  MOZ_ASSERT(mSize.width >= 0 && mSize.height >= 0);

  aSuccess = false;

  gl->MakeCurrent();

  if (max(mSize.width, mSize.height) > gl->MaxTextureSize()) {
    return;
  }

  gl->GenTextures(1, &mTextureId);

  GLenum internalFormat;
  switch (mFormat) {
    case SurfaceFormat::YUV:
    case SurfaceFormat::UNKNOWN:
    default:
      return;
    case SurfaceFormat::B8G8R8A8:
      internalFormat = GL_RGBA8;
      mGLFormat = GL_BGRA;
      mGLType = GL_UNSIGNED_BYTE;
      mBytesPerPixel = 4;
      break;
    case SurfaceFormat::B8G8R8X8:
      internalFormat = GL_RGB8;
      mGLFormat = GL_BGRA;
      mGLType = GL_UNSIGNED_BYTE;
      mBytesPerPixel = 4;
      break;
    case SurfaceFormat::R8G8B8A8:
      internalFormat = GL_RGBA8;
      mGLFormat = GL_RGBA;
      mGLType = GL_UNSIGNED_BYTE;
      mBytesPerPixel = 4;
      break;
    case SurfaceFormat::R8G8B8X8:
      internalFormat = GL_RGB8;
      mGLFormat = GL_RGBA;
      mGLType = GL_UNSIGNED_BYTE;
      mBytesPerPixel = 4;
      break;
    case SurfaceFormat::R5G6B5:
      internalFormat = GL_RGB565;
      mGLFormat = GL_RGB;
      mGLType = GL_UNSIGNED_SHORT_5_6_5;
      mBytesPerPixel = 2;
      break;
    case SurfaceFormat::A8:
      internalFormat = GL_ALPHA;
      mGLFormat = GL_ALPHA;
      mGLType = GL_UNSIGNED_BYTE;
      mBytesPerPixel = 1;
      break;
  }

  gl->TextureImage2DEXT(mTextureId, GL_TEXTURE_2D, 0, internalFormat, mSize.width,
                        mSize.height, 0, mGLFormat, mGLType, nullptr);

  // The initial value for MIN_FILTER is NEAREST_MIPMAP_LINEAR. We initialize it
  // to what 'Filter::LINEAR' expects.
  gl->TextureParameteriEXT(mTextureId, GL_TEXTURE_2D,
                           GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  if (gl->HasExtension(GL::EXT_texture_filter_anisotropic)) {
    gl->TextureParameteriEXT(mTextureId, GL_TEXTURE_2D,
                             GL_TEXTURE_MAX_ANISOTROPY_EXT, gl->MaxAnisotropy());
  }

  aSuccess = true;
}

TemporaryRef<TextureObjectNVpr>
TextureObjectNVpr::Create(DataSourceSurface* aData)
{
  return Create(aData->GetFormat(), aData->GetSize(), aData->GetData(),
                aData->Stride());
}

TemporaryRef<TextureObjectNVpr>
TextureObjectNVpr::Create(SurfaceFormat aFormat, const IntSize& aSize,
                          const GLvoid* aData, GLsizei aStride)
{
  bool success;
  RefPtr<TextureObjectNVpr> surface =
    new TextureObjectNVpr(aFormat, aSize, success);
  if (!success) {
    return nullptr;
  }

  if (aData) {
    surface->WritePixels(aData, aStride);
  }

  return surface.forget();
}

TextureObjectNVpr::~TextureObjectNVpr()
{
  gl->MakeCurrent();
  gl->DeleteTexture(mTextureId);
}

void
TextureObjectNVpr::SetWrapMode(ExtendMode aExtendMode)
{
  switch (aExtendMode) {
    default:
      MOZ_ASSERT(!"Invalid extend mode");
    case ExtendMode::CLAMP:
      SetWrapMode(GL_CLAMP_TO_EDGE);
      return;
    case ExtendMode::REPEAT:
      SetWrapMode(GL_REPEAT);
      return;
    case ExtendMode::REFLECT:
      SetWrapMode(GL_MIRRORED_REPEAT);
      return;
  }
}

void
TextureObjectNVpr::SetWrapMode(GLenum aWrapMode)
{
  if (mWrapMode == aWrapMode) {
    return;
  }

  gl->TextureParameteriEXT(mTextureId, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                           aWrapMode);
  gl->TextureParameteriEXT(mTextureId, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                           aWrapMode);

  mWrapMode = aWrapMode;
}

void
TextureObjectNVpr::SetFilter(Filter aFilter)
{
  if (mFilter != aFilter) {
    GLenum minFilter;
    GLenum magFilter;
    GLint anisotropy;
    switch (aFilter) {
      default:
        MOZ_ASSERT(!"Invalid filter");
      case Filter::GOOD:
        minFilter = GL_LINEAR_MIPMAP_LINEAR;
        magFilter = GL_LINEAR;
        anisotropy = gl->MaxAnisotropy();
        break;
      case Filter::LINEAR:
        minFilter = GL_LINEAR;
        magFilter = GL_LINEAR;
        anisotropy = 1;
        break;
      case Filter::POINT:
        minFilter = magFilter = GL_NEAREST;
        anisotropy = 1;
        break;
    }

    gl->TextureParameteriEXT(mTextureId, GL_TEXTURE_2D,
                             GL_TEXTURE_MIN_FILTER, minFilter);
    gl->TextureParameteriEXT(mTextureId, GL_TEXTURE_2D,
                             GL_TEXTURE_MAG_FILTER, magFilter);

    if (gl->HasExtension(GL::EXT_texture_filter_anisotropic)) {
      gl->TextureParameteriEXT(mTextureId, GL_TEXTURE_2D,
                               GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
    }

    mFilter = aFilter;
  }

  if (mFilter == Filter::GOOD && !mHasMipmaps) {
    gl->GenerateTextureMipmapEXT(mTextureId, GL_TEXTURE_2D);
    mHasMipmaps = true;
  }
}

void
TextureObjectNVpr::WritePixels(const GLvoid* aData, GLsizei aStride,
                               NotifyDataSurface aNotifyDataSurface)
{
  const GLsizei bytesPerRow = mSize.width * mBytesPerPixel;
  const GLubyte* pixelData = static_cast<const GLubyte*>(aData);
  vector<GLubyte> packBuffer;

  gl->MakeCurrent();

  if (aStride == 0 || aStride == bytesPerRow) {
    gl->PixelStorei(GL_PACK_ALIGNMENT, 1);
  } else if (aStride == bytesPerRow + 2 - (bytesPerRow % 2)) {
    gl->PixelStorei(GL_PACK_ALIGNMENT, 2);
  } else if (aStride == bytesPerRow + 4 - (bytesPerRow % 4)) {
    gl->PixelStorei(GL_PACK_ALIGNMENT, 4);
  } else if (aStride == bytesPerRow + 8 - (bytesPerRow % 8)) {
    gl->PixelStorei(GL_PACK_ALIGNMENT, 8);
  } else {
    packBuffer.resize(mSize.height * bytesPerRow);
    for (int i = 0; i < mSize.height; i++) {
      memcpy(&packBuffer[i * bytesPerRow], &pixelData[i * aStride], bytesPerRow);
    }
    pixelData = packBuffer.data();
    gl->PixelStorei(GL_PACK_ALIGNMENT, 1);
  }

  gl->TextureSubImage2DEXT(mTextureId, GL_TEXTURE_2D, 0, 0, 0, mSize.width,
                           mSize.height, mGLFormat, mGLType, pixelData);

  MarkChanged(aNotifyDataSurface);
}

void
TextureObjectNVpr::ReadPixels(GLvoid* aBuffer)
{
  gl->MakeCurrent();
  gl->PixelStorei(GL_UNPACK_ALIGNMENT, 1);
  gl->GetTextureImageEXT(mTextureId, GL_TEXTURE_2D, 0, mGLFormat, mGLType, aBuffer);
}

void
TextureObjectNVpr::MarkChanged(NotifyDataSurface aNotifyDataSurface)
{
  if (aNotifyDataSurface == NOTIFY_DATA_SURFACE && mDataSurface) {
    mDataSurface->OnTextureChanged();
  }

  mHasMipmaps = false;
}

TemporaryRef<DataSourceSurface>
TextureObjectNVpr::GetDataSurface()
{
  if (mDataSurface) {
    return mDataSurface.get();
  }

  RefPtr<DataSourceSurfaceNVpr> dataSurface = new DataSourceSurfaceNVpr(this);
  mDataSurface = dataSurface;

  return dataSurface.forget();
}

unsigned char*
DataSourceSurfaceNVpr::GetData()
{
  if (mShadowBuffer.empty()) {
    mShadowBuffer.resize(mTexture->Size().height * Stride());
    mTexture->ReadPixels(mShadowBuffer.data());
  }

  return mShadowBuffer.data();
}

int32_t
DataSourceSurfaceNVpr::Stride()
{
  return mTexture->BytesPerPixel() * mTexture->Size().width;
}

void
DataSourceSurfaceNVpr::MarkDirty()
{
  if (mShadowBuffer.empty()) {
    return;
  }

  mTexture->WritePixels(mShadowBuffer.data(), 0,
                        TextureObjectNVpr::DO_NOT_NOTIFY_DATA_SURFACE);
}

void
DataSourceSurfaceNVpr::OnTextureChanged()
{
  mShadowBuffer.clear();
}

}
}
