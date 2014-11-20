/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "DrawTargetNVpr.h"
#include "GradientStopsNVpr.h"
#include "Logging.h"
#include "PathBuilderNVpr.h"
#include "PathNVpr.h"
#include "ScaledFontNVpr.h"
#include "SourceSurfaceNVpr.h"
#include "nvpr/Clip.h"
#include "nvpr/Paint.h"
#include "nvpr/ShadowShaders.h"
#include <sstream>
#include <vector>

#ifdef WIN32
#include "DXTextureInteropNVpr.h"
#endif

static const size_t sMaxScratchTextures = 2;

using namespace mozilla::gfx::nvpr;
using namespace std;

namespace mozilla {
namespace gfx {

template<int L>
inline static Log<L>&
operator <<(Log<L>& aStream, const DrawTargetNVpr& aDrawTarget)
{
  aStream << "DrawTargetNVpr(" << &aDrawTarget << ")";
  return aStream;
}

class DrawTargetNVpr::ScratchSurface : public SourceSurfaceNVpr {
public:
  ScratchSurface(WeakPtr<DrawTargetNVpr> aDrawTarget,
                 TemporaryRef<TextureObjectNVpr> aTexture)
    : SourceSurfaceNVpr(aTexture)
    , mDrawTarget(aDrawTarget)
  {}

  virtual ~ScratchSurface()
  {
    if (mDrawTarget) {
      mDrawTarget->OnScratchSurfaceDeleted(Texture());
    }
  }

private:
  WeakPtr<DrawTargetNVpr> mDrawTarget;
};

DrawTargetNVpr::DrawTargetNVpr(const IntSize& aSize, SurfaceFormat aFormat,
                               bool& aSuccess)
  : mSize(aSize)
  , mFormat(aFormat)
  , mColorBuffer(0)
  , mStencilBuffer(0)
  , mFramebuffer(0)
  , mScratchTextureCount(0)
  , mPoppedStencilClips(nullptr)
  , mTransformId(0)
  , mStencilClipBits(0)
{
  aSuccess = false;

  MOZ_ASSERT(mSize.width >= 0 && mSize.height >= 0);

  InitializeGLIfNeeded();
  if (!gl->IsValid()) {
    return;
  }

  gl->MakeCurrent();

  if (max(mSize.width, mSize.height) > gl->MaxRenderbufferSize()
      || max(mSize.width, mSize.height) > gl->MaxTextureSize()) {
    return;
  }

  GLenum colorBufferFormat;
  switch (mFormat) {
    case SurfaceFormat::YUV:
    case SurfaceFormat::UNKNOWN:
    default:
      mHasAlpha = false;
      return;
    case SurfaceFormat::B8G8R8A8:
    case SurfaceFormat::R8G8B8A8:
      colorBufferFormat = GL_RGBA8;
      mHasAlpha = true;
      break;
    case SurfaceFormat::B8G8R8X8:
    case SurfaceFormat::R8G8B8X8:
      colorBufferFormat = GL_RGB8;
      mHasAlpha = false;
      break;
    case SurfaceFormat::R5G6B5:
      colorBufferFormat = GL_RGB565;
      mHasAlpha = false;
      break;
    case SurfaceFormat::A8:
      colorBufferFormat = GL_ALPHA;
      mHasAlpha = true;
      break;
  }
  gl->GenRenderbuffers(1, &mColorBuffer);
  gl->NamedRenderbufferStorageMultisampleEXT(mColorBuffer, 16, colorBufferFormat,
                                             mSize.width, mSize.height);

  gl->GenRenderbuffers(1, &mStencilBuffer);
  gl->NamedRenderbufferStorageMultisampleEXT(mStencilBuffer, 16, GL_STENCIL_INDEX8,
                                             mSize.width, mSize.height);

  gl->GenFramebuffers(1, &mFramebuffer);
  gl->NamedFramebufferRenderbufferEXT(mFramebuffer, GL_COLOR_ATTACHMENT0,
                                      GL_RENDERBUFFER, mColorBuffer);
  gl->NamedFramebufferRenderbufferEXT(mFramebuffer, GL_STENCIL_ATTACHMENT,
                                      GL_RENDERBUFFER, mStencilBuffer);

  Validate(FRAMEBUFFER | COLOR_WRITE_MASK);

  gl->DisableScissorTest();
  gl->SetClearColor(Color());
  gl->Clear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  aSuccess = true;
}

DrawTargetNVpr::~DrawTargetNVpr()
{
  gl->MakeCurrent();
  gl->DeleteRenderbuffers(1, &mColorBuffer);
  gl->DeleteRenderbuffers(1, &mStencilBuffer);
  gl->DeleteFramebuffers(1, &mFramebuffer);
}

TemporaryRef<DrawTargetNVpr::ScratchSurface>
DrawTargetNVpr::GetScratchSurface()
{
  RefPtr<TextureObjectNVpr> texture;

  if (!mScratchTexturePool.empty()) {
    texture = mScratchTexturePool.front();
    mScratchTexturePool.front();
  } else {
    mScratchTextureCount++;
    texture = TextureObjectNVpr::Create(mFormat, mSize);
  }

  return new ScratchSurface(this, texture.forget());
}

void
DrawTargetNVpr::OnScratchSurfaceDeleted(TemporaryRef<TextureObjectNVpr> aBackingTexture)
{
  if (mScratchTextureCount > sMaxScratchTextures) {
    mScratchTextureCount--;
    return;
  }

  mScratchTexturePool.push_back(aBackingTexture);
}

TemporaryRef<SourceSurface>
DrawTargetNVpr::Snapshot()
{
  if (!mSnapshot) {
    mSnapshot = GetScratchSurface();

    gl->MakeCurrent();

    gl->SetFramebuffer(GL_READ_FRAMEBUFFER, mFramebuffer);
    gl->SetFramebufferToTexture(GL_DRAW_FRAMEBUFFER, GL_TEXTURE_2D, *mSnapshot);
    gl->DisableScissorTest();
    gl->SetColorWriteMask(GL::WRITE_COLOR_AND_ALPHA);

    gl->BlitFramebuffer(0, 0, mSize.width, mSize.height,
                        0, 0, mSize.width, mSize.height,
                        GL_COLOR_BUFFER_BIT, GL_NEAREST);

    mSnapshot->MarkChanged();
  }

  return mSnapshot;
}

bool
DrawTargetNVpr::BlitToForeignTexture(void* aForeignContext,
                                     GLuint aForeignTextureId)
{
  Snapshot();
  return gl->BlitTextureToForeignTexture(mSize, *mSnapshot,
                                         aForeignContext, aForeignTextureId);
}

#ifdef WIN32

TemporaryRef<DXTextureInteropNVpr>
DrawTargetNVpr::OpenDXTextureInterop(void* aDX, void* aDXTexture)
{
  return DXTextureInteropNVpr::Create(aDX, aDXTexture);
}

void
DrawTargetNVpr::BlitToDXTexture(DXTextureInteropNVpr* aDXTexture)
{
  gl->MakeCurrent();

  GLuint dxTextureId = aDXTexture->Lock();

  gl->SetFramebuffer(GL_READ_FRAMEBUFFER, mFramebuffer);
  gl->SetFramebufferToTexture(GL_DRAW_FRAMEBUFFER, GL_TEXTURE_2D, dxTextureId);
  gl->DisableScissorTest();
  gl->SetColorWriteMask(GL::WRITE_COLOR_AND_ALPHA);

  gl->BlitFramebuffer(0, 0, mSize.width, mSize.height,
                      0, 0, mSize.width, mSize.height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);

  aDXTexture->Unlock();
}

#endif

void
DrawTargetNVpr::Flush()
{
  gl->MakeCurrent();
  gl->Flush();
}

void
DrawTargetNVpr::DrawSurface(SourceSurface* aSurface,
                            const Rect& aDestRect,
                            const Rect& aSourceRect,
                            const DrawSurfaceOptions& aSurfOptions,
                            const DrawOptions& aOptions)
{
  MOZ_ASSERT(aSurface->GetType() == SURFACE_NVPR_TEXTURE);

  SourceSurfaceNVpr* const surface = static_cast<SourceSurfaceNVpr*>(aSurface);

  gl->MakeCurrent();

  Validate();

  Paint paint;
  if (aSurfOptions.mSamplingBounds == SamplingBounds::UNBOUNDED) {
    paint.SetToSurface(surface, aSurfOptions.mFilter);
  } else {
    paint.SetToClampedSurface(surface, aSurfOptions.mFilter, aSourceRect);
  }
  paint.mGlobalAlpha = aOptions.mAlpha;
  ApplyPaint(paint);

  ApplyDrawOptions(aOptions.mCompositionOp, aOptions.mAntialiasMode);

  if (mStencilClipBits) {
    gl->EnableStencilTest(GL::PASS_IF_ALL_SET, mStencilClipBits,
                          GL::LEAVE_UNCHANGED);
  } else {
    gl->DisableStencilTest();
  }

  Rect textureRect = aSourceRect;
  textureRect.ScaleInverse(surface->GetSize().width, surface->GetSize().height);
  gl->EnableTexCoordArrayToRect(PaintShader::PAINT_UNIT, textureRect);
  gl->DisableTexCoordArray(PaintShader::MASK_UNIT);
  gl->SetVertexArrayToRect(aDestRect);

  gl->DrawArrays(GL_QUADS, 0, 4);
  gl->BlendBarrier();

  MarkChanged();
}

void
DrawTargetNVpr::DrawSurfaceWithShadow(SourceSurface* aSurface,
                                      const Point& aDest,
                                      const Color& aColor,
                                      const Point& aOffset,
                                      Float aSigma,
                                      CompositionOp aOperator)
{
  MOZ_ASSERT(aSurface->GetType() == SURFACE_NVPR_TEXTURE);

  SourceSurfaceNVpr* const surface = static_cast<SourceSurfaceNVpr*>(aSurface);
  RefPtr<ScratchSurface> horizontalConvolution = GetScratchSurface();
  ShadowShaders& shadowShaders =
    gl->GetUserObject<ShadowShaders>(&nvpr::UserData::mShadowShaders);
  const Rect shadowRect(aDest + aOffset, Size(surface->GetSize()));

  gl->MakeCurrent();

  // We take advantage of the fact that a Gaussian blur is separable by drawing
  // the shadow in two passes: Once with a 1-dimensional kernel in the horizontal
  // direction, and again with that same kernel in the vertical direction.
  Rect horizontalConvolutionRect;
  GLuint horizontalConvolutionShader;
  GLuint shadowShader;
  shadowShaders.ConfigureShaders(mSize, shadowRect, aColor, aSigma,
                                 mHasAlpha ? ShadowShaders::ALPHA : ShadowShaders::RED,
                                 &horizontalConvolutionRect,
                                 &horizontalConvolutionShader, &shadowShader);

  // Step 1: Draw the horizontal convolution into a scratch texture.
  gl->SetSize(mSize);
  gl->SetFramebufferToTexture(GL_FRAMEBUFFER, GL_TEXTURE_2D,
                              *horizontalConvolution);
  gl->SetTransformToIdentity();
  gl->DisableScissorTest();
  gl->DisableClipPlanes();
  gl->DisableStencilTest();
  gl->SetBlendMode(CompositionOp::OP_SOURCE);
  gl->SetColorWriteMask(mHasAlpha ? GL::WRITE_ALPHA : GL::WRITE_RED);
  gl->DisableTexCoordArray(GL::UNIT_1);

  gl->SetShaderProgram(horizontalConvolutionShader);

  surface->SetWrapMode(GL_CLAMP_TO_BORDER);
  surface->SetFilter(horizontalConvolutionRect.width == shadowRect.width
                     ? Filter::LINEAR : Filter::GOOD);
  gl->SetTexture(GL::UNIT_0, GL_TEXTURE_2D, *surface);

  gl->EnableTexCoordArrayToUnitRect(GL::UNIT_0);
  gl->SetVertexArrayToRect(horizontalConvolutionRect);

  gl->DrawArrays(GL_QUADS, 0, 4);

  horizontalConvolution->MarkChanged();

  // Step 2: Use the horizontal convolution to draw the shadow.
  Validate(FRAMEBUFFER | CLIPPING | COLOR_WRITE_MASK);
  ApplyDrawOptions(aOperator, AntialiasMode::DEFAULT);

  gl->SetShaderProgram(shadowShader);

  if (mStencilClipBits) {
    gl->EnableStencilTest(GL::PASS_IF_ALL_SET, mStencilClipBits,
                          GL::LEAVE_UNCHANGED);
  }

  horizontalConvolution->SetWrapMode(GL_CLAMP_TO_EDGE);
  horizontalConvolution->SetFilter(Filter::LINEAR);
  gl->SetTexture(GL::UNIT_0, GL_TEXTURE_2D, *horizontalConvolution);

  horizontalConvolutionRect.ScaleInverse(mSize.width, mSize.height);
  gl->EnableTexCoordArrayToRect(GL::UNIT_0, horizontalConvolutionRect);
  gl->SetVertexArrayToRect(shadowRect);

  gl->DrawArrays(GL_QUADS, 0, 4);
  gl->BlendBarrier();

  // Step 3: Draw the surface on top of its shadow.
  Paint paint;
  paint.SetToSurface(surface, Filter::LINEAR);
  ApplyPaint(paint);

  gl->EnableTexCoordArrayToUnitRect(GL::UNIT_0);
  gl->SetVertexArrayToRect(aDest, Size(surface->GetSize()));

  gl->DrawArrays(GL_QUADS, 0, 4);
  gl->BlendBarrier();

  MarkChanged();
}

void
DrawTargetNVpr::ClearRect(const Rect& aRect)
{
  FillRect(aRect, ColorPattern(Color()), DrawOptions(1, CompositionOp::OP_SOURCE));
}

void
DrawTargetNVpr::CopySurface(SourceSurface* aSurface,
                            const IntRect& aSourceRect,
                            const IntPoint& aDestination)
{
  MOZ_ASSERT(aSurface->GetType() == SURFACE_NVPR_TEXTURE);

  SourceSurfaceNVpr* const surface = static_cast<SourceSurfaceNVpr*>(aSurface);

  gl->MakeCurrent();

  // TODO: Consider using NV_draw_texture instead.

  gl->SetFramebufferToTexture(GL_READ_FRAMEBUFFER, GL_TEXTURE_2D, *surface);
  gl->SetFramebuffer(GL_DRAW_FRAMEBUFFER, mFramebuffer);
  gl->DisableScissorTest();
  gl->SetColorWriteMask(GL::WRITE_COLOR_AND_ALPHA);

  gl->BlitFramebuffer(aSourceRect.x, aSourceRect.y, aSourceRect.XMost(),
                      aSourceRect.YMost(), aDestination.x, aDestination.y,
                      aDestination.x + aSourceRect.width, aDestination.y
                      + aSourceRect.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void
DrawTargetNVpr::FillRect(const Rect& aRect,
                         const Pattern& aPattern,
                         const DrawOptions& aOptions)
{
  gl->MakeCurrent();

  if (aPattern.GetType() == PatternType::COLOR) {
    const Color& color = static_cast<const ColorPattern&>(aPattern).mColor;
    const bool needsBlending = aOptions.mCompositionOp != CompositionOp::OP_SOURCE
                               && (aOptions.mCompositionOp != CompositionOp::OP_OVER
                                   || aOptions.mAlpha != 1 || color.a != 1);
    const bool hasComplexClips = mTopPlanesClip || (mTopStencilClip
                                   && (!mPoppedStencilClips
                                       || mPoppedStencilClips->GetPrevious()));

    IntRect scissorRect;
    if (!needsBlending && !hasComplexClips && GetTransform().IsRectilinear()
        && GetTransform().TransformBounds(aRect).ToIntRect(&scissorRect)) {

      Validate(FRAMEBUFFER | COLOR_WRITE_MASK);

      if (mTopScissorClip) {
        scissorRect.IntersectRect(scissorRect, mTopScissorClip->ScissorRect());
      }
      gl->EnableScissorTest(scissorRect);
      gl->SetClearColor(color, aOptions.mAlpha);
      gl->Clear(GL_COLOR_BUFFER_BIT);

      MarkChanged();
      return;
    }
  }

  Validate();

  Paint paint;
  paint.SetToPattern(aPattern);
  paint.mGlobalAlpha = aOptions.mAlpha;
  ApplyPaint(paint);

  ApplyDrawOptions(aOptions.mCompositionOp, aOptions.mAntialiasMode);

  if (mStencilClipBits) {
    gl->EnableStencilTest(GL::PASS_IF_ALL_SET, mStencilClipBits,
                          GL::LEAVE_UNCHANGED);
  } else {
    gl->DisableStencilTest();
  }

  gl->Rectf(aRect.x, aRect.y, aRect.x + aRect.width, aRect.y + aRect.height);
  gl->BlendBarrier();

  MarkChanged();
}

void
DrawTargetNVpr::StrokeRect(const Rect& aRect,
                           const Pattern& aPattern,
                           const StrokeOptions& aStrokeOptions,
                           const DrawOptions& aOptions)
{
  PathBuilderNVpr pathBuilder(FillRule::FILL_WINDING);
  pathBuilder.MoveTo(aRect.BottomRight());
  pathBuilder.LineTo(aRect.TopRight());
  pathBuilder.LineTo(aRect.TopLeft());
  pathBuilder.LineTo(aRect.BottomLeft());
  pathBuilder.Close();
  RefPtr<Path> path = pathBuilder.Finish();

  Stroke(path.get(), aPattern, aStrokeOptions, aOptions);
}

void
DrawTargetNVpr::StrokeLine(const Point& aStart,
                           const Point& aEnd,
                           const Pattern& aPattern,
                           const StrokeOptions& aStrokeOptions,
                           const DrawOptions& aOptions)
{
  PathBuilderNVpr pathBuilder(FillRule::FILL_WINDING);
  pathBuilder.MoveTo(aStart);
  pathBuilder.LineTo(aEnd);
  RefPtr<Path> path = pathBuilder.Finish();

  Stroke(path.get(), aPattern, aStrokeOptions, aOptions);
}

void
DrawTargetNVpr::Stroke(const Path* aPath,
                       const Pattern& aPattern,
                       const StrokeOptions& aStrokeOptions,
                       const DrawOptions& aOptions)
{
  MOZ_ASSERT(aPath->GetBackendType() == BACKEND_NVPR);

  const PathNVpr* const path = static_cast<const PathNVpr*>(aPath);

  gl->MakeCurrent();

  Validate();

  Paint paint;
  paint.SetToPattern(aPattern);
  paint.mGlobalAlpha = aOptions.mAlpha;
  ApplyPaint(paint);

  ApplyDrawOptions(aOptions.mCompositionOp, aOptions.mAntialiasMode);

  gl->ConfigurePathStencilTest(mStencilClipBits);
  path->ApplyStrokeOptions(aStrokeOptions);
  gl->StencilStrokePathNV(*path, 0x1, 0x1);

  gl->EnableStencilTest(GL::PASS_IF_NOT_ZERO, 1, GL::CLEAR_PASSING_VALUES, 1);
  gl->CoverStrokePathNV(*path, GL_BOUNDING_BOX_NV);

  MarkChanged();
}

void
DrawTargetNVpr::Fill(const Path* aPath,
                     const Pattern& aPattern,
                     const DrawOptions& aOptions)
{
  MOZ_ASSERT(aPath->GetBackendType() == BACKEND_NVPR);

  const PathNVpr* const path = static_cast<const PathNVpr*>(aPath);
  const GLubyte countingMask =
      path->GetFillRule() == FillRule::FILL_WINDING ? (~mStencilClipBits & 0xff) : 0x1;

  gl->MakeCurrent();

  Validate();

  Paint paint;
  paint.SetToPattern(aPattern);
  paint.mGlobalAlpha = aOptions.mAlpha;
  ApplyPaint(paint);

  ApplyDrawOptions(aOptions.mCompositionOp, aOptions.mAntialiasMode);

  gl->ConfigurePathStencilTest(mStencilClipBits);
  gl->StencilFillPathNV(*path, GL_COUNT_UP_NV, countingMask);

  gl->EnableStencilTest(GL::PASS_IF_NOT_ZERO, countingMask,
                        GL::CLEAR_PASSING_VALUES, countingMask);
  gl->CoverFillPathNV(*path, GL_BOUNDING_BOX_NV);

  MarkChanged();
}

void
DrawTargetNVpr::FillGlyphs(ScaledFont* aFont,
                           const GlyphBuffer& aBuffer,
                           const Pattern& aPattern,
                           const DrawOptions& aOptions,
                           const GlyphRenderingOptions* aRenderOptions)
{
  MOZ_ASSERT(aFont->GetType() == FONT_NVPR);

  if (!aBuffer.mNumGlyphs) {
    return;
  }

  const ScaledFontNVpr* const font = static_cast<const ScaledFontNVpr*>(aFont);
  const GLubyte countingMask = (~mStencilClipBits & 0xff);

  gl->MakeCurrent();

  Validate();

  Paint paint;
  paint.SetToPattern(aPattern);
  paint.mGlobalAlpha = aOptions.mAlpha;
  ApplyPaint(paint);

  ApplyDrawOptions(aOptions.mCompositionOp, aOptions.mAntialiasMode);

  gl->ScaleTransform(font->Size(), -font->Size());

  struct Position {GLfloat x, y;};
  vector<GLuint> characters(aBuffer.mNumGlyphs);
  vector<Position> positions(aBuffer.mNumGlyphs);

  for (size_t i = 0; i < aBuffer.mNumGlyphs; i++) {
    // TODO: How can we know the real mapping index -> unicode?
    characters[i] = aBuffer.mGlyphs[i].mIndex + 29;
    positions[i].x = aBuffer.mGlyphs[i].mPosition.x * font->InverseSize();
    positions[i].y = aBuffer.mGlyphs[i].mPosition.y * -font->InverseSize();
  }

  gl->ConfigurePathStencilTest(mStencilClipBits);
  gl->StencilFillPathInstancedNV(aBuffer.mNumGlyphs, GL_UNSIGNED_INT,
                                 &characters.front(), *font, GL_COUNT_UP_NV,
                                 countingMask, GL_TRANSLATE_2D_NV,
                                 &positions[0].x);

  gl->EnableStencilTest(GL::PASS_IF_NOT_ZERO, countingMask,
                        GL::CLEAR_PASSING_VALUES, countingMask);
  gl->CoverFillPathInstancedNV(aBuffer.mNumGlyphs, GL_UNSIGNED_INT,
                               &characters.front(), *font,
                               GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV,
                               GL_TRANSLATE_2D_NV, &positions[0].x);

  MarkChanged();
}

void
DrawTargetNVpr::Mask(const Pattern& aSource,
                     const Pattern& aMask,
                     const DrawOptions& aOptions)
{
  gl->MakeCurrent();

  Validate();

  Paint paint;
  paint.SetToPattern(aSource);
  paint.mMask.SetToPattern(aMask);
  paint.mGlobalAlpha = aOptions.mAlpha;
  ApplyPaint(paint);

  ApplyDrawOptions(aOptions.mCompositionOp, aOptions.mAntialiasMode);

  if (mStencilClipBits) {
    gl->EnableStencilTest(GL::PASS_IF_ALL_SET, mStencilClipBits,
                          GL::LEAVE_UNCHANGED);
  } else {
    gl->DisableStencilTest();
  }

  Matrix inverse = GetTransform();
  inverse.Invert();
  Point topLeft = inverse * Point(0, 0);
  Point bottomRight = inverse * Point(mSize.width, mSize.height);

  gl->Rectf(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
  gl->BlendBarrier();

  MarkChanged();
}

void
DrawTargetNVpr::MaskSurface(const Pattern& aSource,
                            SourceSurface* aMask,
                            Point aOffset,
                            const DrawOptions& aOptions)
{
  MOZ_ASSERT(aMask->GetType() == SURFACE_NVPR_TEXTURE);

  SourceSurfaceNVpr* const mask = static_cast<SourceSurfaceNVpr*>(aMask);
  const Rect maskRect(aOffset, Size(mask->GetSize()));

  gl->MakeCurrent();

  Validate();

  Paint paint;
  paint.SetToPattern(aSource);
  paint.mMask.SetToSurface(mask);
  paint.mGlobalAlpha = aOptions.mAlpha;
  ApplyPaint(paint);

  ApplyDrawOptions(aOptions.mCompositionOp, aOptions.mAntialiasMode);

  if (mStencilClipBits) {
    gl->EnableStencilTest(GL::PASS_IF_ALL_SET, mStencilClipBits,
                          GL::LEAVE_UNCHANGED);
  } else {
    gl->DisableStencilTest();
  }

  gl->DisableTexCoordArray(PaintShader::PAINT_UNIT);
  gl->EnableTexCoordArrayToUnitRect(PaintShader::MASK_UNIT);
  gl->SetVertexArrayToRect(maskRect);

  gl->DrawArrays(GL_QUADS, 0, 4);
  gl->BlendBarrier();

  MarkChanged();
}

void
DrawTargetNVpr::PushClip(const Path* aPath)
{
  MOZ_ASSERT(aPath->GetBackendType() == BACKEND_NVPR);

  const PathNVpr* const path = static_cast<const PathNVpr*>(aPath);

  if (!path->Polygon().IsEmpty()) {
    if (RefPtr<PlanesClip> planesClip =
          PlanesClip::Create(this, mTopPlanesClip, GetTransform(),
                             ConvexPolygon(path->Polygon()))) {
      mTopPlanesClip = planesClip.forget();
      mClipTypeStack.push(PLANES_CLIP_TYPE);
      return;
    }
  }

  Validate(FRAMEBUFFER | CLIPPING);

  mTopStencilClip = StencilClip::Create(this, mTopStencilClip.forget(),
                                        GetTransform(), mTransformId,
                                        path->Clone());

  mTopStencilClip->ApplyToStencilBuffer();

  mClipTypeStack.push(STENCIL_CLIP_TYPE);
}

void
DrawTargetNVpr::PushClipRect(const Rect& aRect)
{
  if (RefPtr<ScissorClip> scissorClip =
        ScissorClip::Create(this, mTopScissorClip, GetTransform(), aRect)) {
    mTopScissorClip = scissorClip.forget();
    mClipTypeStack.push(SCISSOR_CLIP_TYPE);
    return;
  }

  if (RefPtr<PlanesClip> planesClip =
        PlanesClip::Create(this, mTopPlanesClip, GetTransform(),
                           ConvexPolygon(aRect))) {
    mTopPlanesClip = planesClip.forget();
    mClipTypeStack.push(PLANES_CLIP_TYPE);
    return;
  }

  if (!mUnitSquarePath) {
    PathBuilderNVpr pathBuilder(FillRule::FILL_WINDING);
    pathBuilder.MoveTo(Point(0, 0));
    pathBuilder.LineTo(Point(1, 0));
    pathBuilder.LineTo(Point(1, 1));
    pathBuilder.LineTo(Point(0, 1));
    RefPtr<Path> path = pathBuilder.Finish();

    mUnitSquarePath = static_cast<PathNVpr*>(path.get());
  }

  Matrix transform = GetTransform();
  transform.PreTranslate(aRect.x, aRect.y);
  transform.PreScale(aRect.width, aRect.height);

  Validate(FRAMEBUFFER | CLIPPING);

  mTopStencilClip = StencilClip::Create(this, mTopStencilClip.forget(),
                                        transform, gl->GetUniqueId(),
                                        mUnitSquarePath->Clone());

  mTopStencilClip->ApplyToStencilBuffer();

  mClipTypeStack.push(STENCIL_CLIP_TYPE);
}

void
DrawTargetNVpr::PopClip()
{
  switch (mClipTypeStack.top()) {
    case SCISSOR_CLIP_TYPE:
      mTopScissorClip = mTopScissorClip->Pop();
      break;
    case PLANES_CLIP_TYPE:
      mTopPlanesClip = mTopPlanesClip->Pop();
      break;
    case STENCIL_CLIP_TYPE:
      mPoppedStencilClips = !mPoppedStencilClips ? mTopStencilClip.get()
                                                 : mPoppedStencilClips->GetPrevious();
      break;
    default:
      MOZ_ASSERT(!"Invalid clip type.");
  }

  mClipTypeStack.pop();
}

TemporaryRef<SourceSurface>
DrawTargetNVpr::CreateSourceSurfaceFromData(unsigned char* aData,
                                            const IntSize& aSize,
                                            int32_t aStride,
                                            SurfaceFormat aFormat) const
{
 RefPtr<TextureObjectNVpr> texture =
   TextureObjectNVpr::Create(aFormat, aSize, aData, aStride);

  return texture ? new SourceSurfaceNVpr(texture.forget()) : nullptr;
}

TemporaryRef<SourceSurface>
DrawTargetNVpr::OptimizeSourceSurface(SourceSurface* aSurface) const
{
  if (aSurface->GetType() == SurfaceType::NVPR_TEXTURE) {
    return aSurface;
  }

  RefPtr<DataSourceSurface> data = aSurface->GetDataSurface();
  RefPtr<TextureObjectNVpr> texture = TextureObjectNVpr::Create(data);

  return texture ? new SourceSurfaceNVpr(texture.forget()) : nullptr;
}

TemporaryRef<SourceSurface>
DrawTargetNVpr::CreateSourceSurfaceFromNativeSurface(const NativeSurface& aSurface) const
{
  gfxWarning() << *this << ": CreateSourceSurfaceFromNativeSurface not implemented";
  return 0;
}

TemporaryRef<DrawTarget>
DrawTargetNVpr::CreateSimilarDrawTarget(const IntSize& aSize,
                                        SurfaceFormat aFormat) const
{
  return Create(aSize, aFormat);
}

TemporaryRef<PathBuilder>
DrawTargetNVpr::CreatePathBuilder(FillRule aFillRule) const
{
  return new PathBuilderNVpr(aFillRule);
}

TemporaryRef<GradientStops>
DrawTargetNVpr::CreateGradientStops(GradientStop* rawStops, uint32_t aNumStops,
                                    ExtendMode aExtendMode) const
{
  return GradientStopsNVpr::create(rawStops, aNumStops, aExtendMode);
}

void*
DrawTargetNVpr::GetNativeSurface(NativeSurfaceType aType)
{
  gfxWarning() << *this << ": GetNativeSurface not implemented";
  return 0;
}

void
DrawTargetNVpr::SetTransform(const Matrix& aTransform)
{
  DrawTarget::SetTransform(aTransform);
  mTransformId = gl->GetUniqueId();
}

void
DrawTargetNVpr::Validate(ValidationFlags aFlags)
{
  MOZ_ASSERT(gl->IsCurrent());

  if (aFlags & FRAMEBUFFER) {
    gl->SetSize(mSize);
    gl->SetFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
  }

  if (aFlags & CLIPPING) {
    if (mTopScissorClip) {
      gl->EnableScissorTest(mTopScissorClip->ScissorRect());
    } else {
      gl->DisableScissorTest();
    }

    if (mTopPlanesClip) {
      if (gl->ClipPolygonId() != mTopPlanesClip->PolygonId()) {
        gl->SetTransformToIdentity();
        gl->EnableClipPlanes(mTopPlanesClip->Polygon(),
                             mTopPlanesClip->PolygonId());
      }
    } else {
      gl->DisableClipPlanes();
    }

    if (mPoppedStencilClips) {
      mPoppedStencilClips->RestoreStencilBuffer();
      mTopStencilClip = mPoppedStencilClips->Pop();
      mPoppedStencilClips = nullptr;
    }
  }

  if (aFlags & TRANSFORM) {
    gl->SetTransform(GetTransform(), mTransformId);
  }

  if (aFlags & COLOR_WRITE_MASK) {
    gl->SetColorWriteMask(GL::WRITE_COLOR_AND_ALPHA);
  }
}

void
DrawTargetNVpr::ApplyPaint(const Paint& aPaint)
{
  MOZ_ASSERT(gl->IsCurrent());

  struct PaintShaders : public nvpr::UserData::Object {
    RefPtr<PaintShader> mShaders[PaintConfig::MODE_COUNT]
                                [PaintConfig::MODE_COUNT][2];
  };

  PaintShaders& shaders =
    gl->GetUserObject<PaintShaders>(&nvpr::UserData::mPaintShaders);

  RefPtr<PaintShader>& shader = shaders.mShaders[aPaint.mPaintMode]
                                                [aPaint.mMask.mPaintMode]
                                                [aPaint.mGlobalAlpha != 1];
  if (!shader) {
    shader = PaintShader::Create(aPaint);
  }

  gl->SetTexGen(PaintShader::PAINT_UNIT, aPaint.mTexGenComponents,
                aPaint.mTexGenCoefficients);
  gl->SetTexGen(PaintShader::MASK_UNIT, aPaint.mMask.mTexGenComponents,
                aPaint.mMask.mTexGenCoefficients);

  shader->ApplyFragmentUniforms(aPaint);

  gl->SetShaderProgram(*shader);
}

void
DrawTargetNVpr::ApplyDrawOptions(CompositionOp aCompositionOp,
                                 AntialiasMode aAntialiasMode)
{
  MOZ_ASSERT(gl->IsCurrent());

  gl->SetBlendMode(aCompositionOp);

  if (aAntialiasMode == AntialiasMode::NONE) {
    gl->DisableMultisample();
  } else {
    gl->EnableMultisample();
  }
}

void
DrawTargetNVpr::MarkChanged()
{
  mSnapshot = nullptr;
}

GLubyte
DrawTargetNVpr::ReserveStencilClipBit()
{
  // Don't reserve more than two bit planes for clipping.
  if (mStencilClipBits >= 0xc0) {
    return 0;
  }

  mStencilClipBits = 0x80 | (mStencilClipBits >> 1);

  return (~mStencilClipBits & 0xff) + 1;
}

void
DrawTargetNVpr::ReleaseStencilClipBits(GLubyte aBits)
{
  mStencilClipBits &= (~aBits & 0xff);

  // The clip bits need to be a consecutive run of most-significant bits (in
  // other words, they need to be released in reverse order).
  MOZ_ASSERT((~mStencilClipBits & 0xff) & ((~mStencilClipBits & 0xff) - 1));
}

}
}
