/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GL.h"
#include "ConvexPolygon.h"
#include "Line.h"
#include "Logging.h"
#include <iterator>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#ifdef WIN32
#define STDCALL __stdcall
#else
#define STDCALL
#endif

namespace mozilla {
namespace gfx {

static void STDCALL GLDebugCallback(GLenum aSource, GLenum aType, GLuint aId,
                                    GLenum aSeverity, GLsizei aLength,
                                    const GLchar* aMessage,
                                    const void* aUserParam)
{
  if (aSeverity == GL_DEBUG_SEVERITY_LOW) {
    return;
  }

  gfxWarning() << "OpenGL Debug Callback:";
  gfxWarning() << "  Source: 0x" << hex << aSource;
  gfxWarning() << "  Type: 0x" << hex << aType;
  gfxWarning() << "  Id: " << aId;
  gfxWarning() << "  Severity: 0x" << hex << aSeverity;
  gfxWarning() << "  Message: " << aMessage;
  gfxWarning() << "";
}

static void RectToClientArray(GLfloat* aClientArray,
                              const Point& aPosition, const Size& aSize)
{
  aClientArray[0] = aPosition.x;
  aClientArray[1] = aPosition.y;
  aClientArray[2] = aPosition.x + aSize.width;
  aClientArray[3] = aPosition.y;
  aClientArray[4] = aPosition.x + aSize.width;
  aClientArray[5] = aPosition.y + aSize.height;
  aClientArray[6] = aPosition.x;
  aClientArray[7] = aPosition.y + aSize.height;
}


namespace nvpr {

GL* gl = nullptr;

GL::GL()
  : mIsValid(false)
  , mNextUniqueId(1)
  , mReadFramebuffer(0)
  , mDrawFramebuffer(0)
  , mTransformId(0)
  , mColorWriteMask(WRITE_COLOR_AND_ALPHA)
  , mNumClipPlanes(0)
  , mClipPolygonId(0)
  , mColor(1, 1, 1, 1)
  , mScissorTestEnabled(false)
  , mStencilTestEnabled(false)
  , mStencilTest(ALWAYS_PASS)
  , mStencilComparand(0)
  , mStencilTestMask(~0)
  , mStencilOp(LEAVE_UNCHANGED)
  , mStencilWriteMask(~0)
  , mPathStencilFuncBits(0)
  , mBlendMode(CompositionOp::OP_SOURCE)
  , mMultisampleEnabled(true)
  , mShaderProgram(0)
{
  memset(mTexGenComponents, 0, sizeof(mTexGenComponents));
  memset(mTexGenTransformIds, 0, sizeof(mTexGenTransformIds));
  memset(mTexGenCoefficients, 0, sizeof(mTexGenCoefficients));
  memset(mActiveTextureTargets, 0, sizeof(mActiveTextureTargets));
  memset(mBoundTextures, 0, sizeof(mBoundTextures));
  memset(mTexCoordArraysEnabled, 0, sizeof(mTexCoordArraysEnabled));
}

void GL::Initialize()
{
  MOZ_ASSERT(IsCurrent());

  GLint majorVersion;
  GetIntegerv(GL_MAJOR_VERSION, &majorVersion);
  if (majorVersion < 4) {
    return;
  }

  if (majorVersion == 4) {
    GLint minorVersion;
    GetIntegerv(GL_MINOR_VERSION, &minorVersion);
    if (minorVersion < 3) {
      return;
    }
  }

  memset(mSupportedExtensions, 0, sizeof(mSupportedExtensions));
  stringstream extensions(reinterpret_cast<const char*>(GetString(GL_EXTENSIONS)));
  istream_iterator<string> iter(extensions);
  istream_iterator<string> end;

  for (; iter != end; iter++) {
    const string& extension = *iter;

    if (*iter == "GL_EXT_texture_filter_anisotropic") {
      mSupportedExtensions[EXT_texture_filter_anisotropic] = true;
      continue;
    }

    if (*iter == "GL_EXT_direct_state_access") {
      mSupportedExtensions[EXT_direct_state_access] = true;
      continue;
    }

    if (*iter == "GL_ARB_texture_storage") {
      mSupportedExtensions[ARB_texture_storage] = true;
      continue;
    }

    if (*iter == "GL_NV_path_rendering") {
      mSupportedExtensions[NV_path_rendering] = true;
      continue;
    }

    if (*iter == "GL_NV_blend_equation_advanced") {
      mSupportedExtensions[NV_blend_equation_advanced] = true;
      continue;
    }

    if (*iter == "GL_NV_blend_equation_advanced_coherent") {
      mSupportedExtensions[NV_blend_equation_advanced_coherent] = true;
      continue;
    }
  }

  if (!HasExtension(GL::EXT_direct_state_access)
      || !HasExtension(GL::ARB_texture_storage)
      || !HasExtension(GL::NV_path_rendering)
      || !HasExtension(GL::NV_blend_equation_advanced)) {
    return;
  }

  GetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &mMaxRenderbufferSize);
  GetIntegerv(GL_MAX_TEXTURE_SIZE, &mMaxTextureSize);
  GetIntegerv(GL_MAX_CLIP_PLANES, &mMaxClipPlanes);

  if (HasExtension(EXT_texture_filter_anisotropic)) {
    GetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &mMaxAnisotropy);
  } else {
    mMaxAnisotropy = 1;
  }

  GenFramebuffers(1, &mTextureFramebuffer1D);
  GenFramebuffers(1, &mTextureFramebuffer2D);

  EnableClientState(GL_VERTEX_ARRAY);

  Enable(GL_BLEND);

  DebugMessageCallback(GLDebugCallback, nullptr);
  DebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
  Enable(GL_DEBUG_OUTPUT);

  mIsValid = true;
}

GL::~GL()
{
  // No need to delete the GL objects. They automatically went away when the
  // context was destroyed.
}

void
GL::SetSize(const IntSize& aSize)
{
  MOZ_ASSERT(IsCurrent());

  if (mSize == aSize) {
    return;
  }

  Viewport(0, 0, aSize.width, aSize.height);
  MatrixLoadIdentityEXT(GL_PROJECTION);
  MatrixOrthoEXT(GL_PROJECTION, 0, aSize.width, 0, aSize.height, -1, 1);
  mSize = aSize;
}

void
GL::SetFramebuffer(GLenum aFramebufferTarget, GLuint aFramebuffer)
{
  MOZ_ASSERT(IsCurrent());

  bool clearTextureFramebuffer1D;
  bool clearTextureFramebuffer2D;

  switch (aFramebufferTarget) {
    case GL_FRAMEBUFFER:
      if (mReadFramebuffer == aFramebuffer && mDrawFramebuffer == aFramebuffer) {
        return;
      }

      BindFramebuffer(GL_FRAMEBUFFER, aFramebuffer);

      clearTextureFramebuffer1D = (aFramebuffer != mTextureFramebuffer1D)
                                  && (mDrawFramebuffer == mTextureFramebuffer1D
                                      || mReadFramebuffer == mTextureFramebuffer1D);
      clearTextureFramebuffer2D = (aFramebuffer != mTextureFramebuffer2D)
                                  && (mDrawFramebuffer == mTextureFramebuffer2D
                                      || mReadFramebuffer == mTextureFramebuffer2D);
      mReadFramebuffer = mDrawFramebuffer = aFramebuffer;
      break;

    case GL_READ_FRAMEBUFFER:
      if (mReadFramebuffer == aFramebuffer) {
        return;
      }

      BindFramebuffer(GL_READ_FRAMEBUFFER, aFramebuffer);

      clearTextureFramebuffer1D = (mReadFramebuffer == mTextureFramebuffer1D);
      clearTextureFramebuffer2D = (mReadFramebuffer == mTextureFramebuffer2D);
      mReadFramebuffer = aFramebuffer;
      break;

    case GL_DRAW_FRAMEBUFFER:
      if (mDrawFramebuffer == aFramebuffer) {
        return;
      }

      BindFramebuffer(GL_DRAW_FRAMEBUFFER, aFramebuffer);

      clearTextureFramebuffer1D = (mDrawFramebuffer == mTextureFramebuffer1D);
      clearTextureFramebuffer2D = (mDrawFramebuffer == mTextureFramebuffer2D);
      mDrawFramebuffer = aFramebuffer;
      break;

    default:
      MOZ_ASSERT(!"Invalid framebuffer target.");
      break;
  }

  if (clearTextureFramebuffer1D) {
    NamedFramebufferTexture1DEXT(mTextureFramebuffer1D, GL_COLOR_ATTACHMENT0,
                                 GL_TEXTURE_1D, 0, 0);
  }

  if (clearTextureFramebuffer2D) {
    NamedFramebufferTexture2DEXT(mTextureFramebuffer2D, GL_COLOR_ATTACHMENT0,
                                 GL_TEXTURE_2D, 0, 0);
  }
}

void
GL::SetFramebufferToTexture(GLenum aFramebufferTarget,
                            GLenum aTextureTarget, GLuint aTextureId)
{
  MOZ_ASSERT(IsCurrent());

  switch (aTextureTarget) {
    case GL_TEXTURE_1D:
      NamedFramebufferTexture1DEXT(mTextureFramebuffer1D, GL_COLOR_ATTACHMENT0,
                                   GL_TEXTURE_1D, aTextureId, 0);
      SetFramebuffer(aFramebufferTarget, mTextureFramebuffer1D);
      break;

    case GL_TEXTURE_2D:
      NamedFramebufferTexture2DEXT(mTextureFramebuffer2D, GL_COLOR_ATTACHMENT0,
                                   GL_TEXTURE_2D, aTextureId, 0);
      SetFramebuffer(aFramebufferTarget, mTextureFramebuffer2D);
      break;

    default:
      MOZ_ASSERT(!"Invalid texture target.");
      break;
  }
}

void
GL::SetTransform(const Matrix& aTransform, UniqueId aTransformId)
{
  MOZ_ASSERT(IsCurrent());

  if (mTransformId == aTransformId) {
    return;
  }

  const GLfloat matrix[] = {
    aTransform._11,  aTransform._12,  0,  0,
    aTransform._21,  aTransform._22,  0,  0,
    0,               0,               1,  0,
    aTransform._31,  aTransform._32,  0,  1
  };

  MatrixLoadfEXT(GL_MODELVIEW, matrix);

  mTransformId = aTransformId;
}

void
GL::ScaleTransform(GLfloat x, GLfloat y)
{
  MOZ_ASSERT(IsCurrent());

  MatrixScalefEXT(GL_MODELVIEW, x, y, 1);
  mTransformId = GetUniqueId();
}

void
GL::SetTransformToIdentity()
{
  MOZ_ASSERT(IsCurrent());

  if (mTransformId == 0) {
    return;
  }

  MatrixLoadIdentityEXT(GL_MODELVIEW);
  mTransformId = 0;
}

void
GL::SetColorWriteMask(ColorWriteMask aColorWriteMask)
{
  MOZ_ASSERT(IsCurrent());

  if (mColorWriteMask == aColorWriteMask) {
    return;
  }

  ColorMask(aColorWriteMask & WRITE_RED, aColorWriteMask & WRITE_GREEN,
            aColorWriteMask & WRITE_BLUE, aColorWriteMask & WRITE_ALPHA);
  mColorWriteMask = aColorWriteMask;
}

void
GL::SetClearColor(const Color& aColor)
{
  MOZ_ASSERT(IsCurrent());

  if (!memcmp(&mClearColor, &aColor, sizeof(Color))) {
    return;
  }

  if (aColor.a == 1) {
    ClearColor(aColor.r, aColor.g, aColor.b, 1);
  } else {
    const float a = aColor.a;
    ClearColor(a * aColor.r, a * aColor.g, a * aColor.b, a);
  }

  mClearColor = aColor;
}

void
GL::SetClearColor(const Color& aColor, GLfloat aAlpha)
{
  SetClearColor(Color(aColor.r, aColor.g, aColor.b, aAlpha * aColor.a));
}

void
GL::EnableScissorTest(const IntRect& aScissorRect)
{
  MOZ_ASSERT(IsCurrent());

  if (!mScissorTestEnabled) {
    Enable(GL_SCISSOR_TEST);
    mScissorTestEnabled = true;
  }

  if (!mScissorRect.IsEqualInterior(aScissorRect)) {
    Scissor(aScissorRect.x, aScissorRect.y, aScissorRect.width, aScissorRect.height);
    mScissorRect = aScissorRect;
  }
}

void
GL::DisableScissorTest()
{
  MOZ_ASSERT(IsCurrent());

  if (!mScissorTestEnabled) {
    return;
  }

  Disable(GL_SCISSOR_TEST);
  mScissorTestEnabled = false;
}

void
GL::EnableClipPlanes(const ConvexPolygon& aPolygon, UniqueId aPolygonId)
{
  MOZ_ASSERT(IsCurrent());
  MOZ_ASSERT(aPolygon.NumSides() <= mMaxClipPlanes);

  if (mClipPolygonId == aPolygonId) {
    return;
  }

  if (aPolygon.IsEmpty()) {
    if (!mNumClipPlanes) {
      Enable(GL_CLIP_PLANE0);
    } else {
      for (size_t i = 1; i < mNumClipPlanes; i++) {
        Disable(GL_CLIP_PLANE0 + i);
      }
    }

    mNumClipPlanes = 1;

    // We specify a single clip plane equation that fails for all vertices.
    const double planeEquation[] = {0, 0, 0, -1};
    ClipPlane(GL_CLIP_PLANE0, planeEquation);

    mClipPolygonId = aPolygonId;

    return;
  }

  for (size_t i = mNumClipPlanes; i < aPolygon.NumSides(); i++) {
    Enable(GL_CLIP_PLANE0 + i);
  }
  for (size_t i = aPolygon.NumSides(); i < mNumClipPlanes; i++) {
    Disable(GL_CLIP_PLANE0 + i);
  }

  mNumClipPlanes = aPolygon.NumSides();

  for (size_t i = 0; i < aPolygon.NumSides(); i++) {
    const Line& line = aPolygon.Sides()[i];
    const double planeEquation[] = {line.A, line.B, 0, -line.C};
    ClipPlane(GL_CLIP_PLANE0 + i, planeEquation);
  }

  mClipPolygonId = aPolygonId;
}

void
GL::DisableClipPlanes()
{
  MOZ_ASSERT(IsCurrent());

  for (size_t i = 0; i < mNumClipPlanes; i++) {
    Disable(GL_CLIP_PLANE0 + i);
  }

  mNumClipPlanes = 0;
  mClipPolygonId = 0;
}

void
GL::EnableStencilTest(UnaryStencilTest aTest, GLuint aTestMask,
                      StencilOperation aOp, GLuint aWriteMask)
{
  switch (aTest) {
    case PASS_IF_NOT_ZERO:
      EnableStencilTest(PASS_IF_NOT_EQUAL, 0, aTestMask, aOp, aWriteMask);
      return;
    case PASS_IF_ALL_SET:
      EnableStencilTest(PASS_IF_EQUAL, aTestMask, aTestMask, aOp, aWriteMask);
      return;
  }
}

void
GL::EnableStencilTest(BinaryStencilTest aTest,
                      GLint aComparand, GLuint aTestMask,
                      StencilOperation aOp, GLuint aWriteMask)
{
  MOZ_ASSERT(IsCurrent());

  if (!mStencilTestEnabled) {
    Enable(GL_STENCIL_TEST);
    mStencilTestEnabled = true;
  }

  if (mStencilTest != aTest || mStencilComparand != aComparand
      || mStencilTestMask != aTestMask) {
    GLenum func;
    switch (aTest) {
      default:
        MOZ_ASSERT(!"Invalid stencil test");
      case ALWAYS_PASS:
        func = GL_ALWAYS;
        break;
      case PASS_IF_EQUAL:
        func = GL_EQUAL;
        break;
      case PASS_IF_NOT_EQUAL:
        func = GL_NOTEQUAL;
        break;
    }

    StencilFunc(func, aComparand, aTestMask);

    mStencilTest = aTest;
    mStencilComparand = aComparand;
    mStencilTestMask = aTestMask;
  }

  if (mStencilOp != aOp) {
    switch (aOp) {
      case LEAVE_UNCHANGED:
        StencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        break;
      case CLEAR_PASSING_VALUES:
        StencilOp(GL_KEEP, GL_ZERO, GL_ZERO);
        break;
      case REPLACE_PASSING_WITH_COMPARAND:
        StencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
        break;
      case REPLACE_PASSING_CLEAR_FAILING:
        StencilOp(GL_ZERO, GL_REPLACE, GL_REPLACE);
        break;
    }

    mStencilOp = aOp;
  }

  if (mStencilWriteMask != aWriteMask) {
    StencilMask(aWriteMask);
    mStencilWriteMask = aWriteMask;
  }
}

void
GL::DisableStencilTest()
{
  MOZ_ASSERT(IsCurrent());

  if (!mStencilTestEnabled) {
    return;
  }

  Disable(GL_STENCIL_TEST);
  mStencilTestEnabled = false;
}

void
GL::ConfigurePathStencilTest(GLubyte aClipBits)
{
  MOZ_ASSERT(IsCurrent());

  if (mPathStencilFuncBits == aClipBits) {
    return;
  }

  if (!aClipBits) {
    PathStencilFuncNV(GL_ALWAYS, 0, 0);
  } else {
    PathStencilFuncNV(GL_EQUAL, aClipBits, aClipBits);
  }

  mPathStencilFuncBits = aClipBits;
}

void
GL::SetShaderProgram(GLuint aShaderProgram)
{
  MOZ_ASSERT(IsCurrent());

  if (mShaderProgram == aShaderProgram) {
    return;
  }

  UseProgram(aShaderProgram);
  mShaderProgram = aShaderProgram;
}

void
GL::DeleteShaderProgram(GLuint aShaderProgram)
{
  MOZ_ASSERT(IsCurrent());

  if (mShaderProgram == aShaderProgram) {
    SetShaderProgram(0);
  }

  DeleteProgram(aShaderProgram);
}

void
GL::SetBlendMode(CompositionOp aBlendMode)
{
  if (mBlendMode == aBlendMode) {
    return;
  }

  switch (aBlendMode) {
    default: MOZ_ASSERT(!"Invalid blend mode");
    case CompositionOp::OP_OVER: BlendEquation(GL_SRC_OVER_NV); break;
    case CompositionOp::OP_ADD: BlendEquation(GL_PLUS_NV); break;
    case CompositionOp::OP_ATOP: BlendEquation(GL_SRC_ATOP_NV); break;
    case CompositionOp::OP_OUT: BlendEquation(GL_SRC_OUT_NV); break;
    case CompositionOp::OP_IN: BlendEquation(GL_SRC_IN_NV); break;
    case CompositionOp::OP_SOURCE: BlendEquation(GL_SRC_NV); break;
    case CompositionOp::OP_DEST_IN: BlendEquation(GL_DST_IN_NV); break;
    case CompositionOp::OP_DEST_OUT: BlendEquation(GL_DST_OUT_NV); break;
    case CompositionOp::OP_DEST_OVER: BlendEquation(GL_DST_OVER_NV); break;
    case CompositionOp::OP_DEST_ATOP: BlendEquation(GL_DST_ATOP_NV); break;
    case CompositionOp::OP_XOR: BlendEquation(GL_XOR); break;
    case CompositionOp::OP_MULTIPLY: BlendEquation(GL_MULTIPLY_NV); break;
    case CompositionOp::OP_SCREEN: BlendEquation(GL_SCREEN_NV); break;
    case CompositionOp::OP_OVERLAY: BlendEquation(GL_OVERLAY_NV); break;
    case CompositionOp::OP_DARKEN: BlendEquation(GL_DARKEN_NV); break;
    case CompositionOp::OP_LIGHTEN: BlendEquation(GL_LIGHTEN_NV); break;
    case CompositionOp::OP_COLOR_DODGE: BlendEquation(GL_COLORDODGE_NV); break;
    case CompositionOp::OP_COLOR_BURN: BlendEquation(GL_COLORBURN_NV); break;
    case CompositionOp::OP_HARD_LIGHT: BlendEquation(GL_HARDLIGHT_NV); break;
    case CompositionOp::OP_SOFT_LIGHT: BlendEquation(GL_SOFTLIGHT_NV); break;
    case CompositionOp::OP_DIFFERENCE: BlendEquation(GL_DIFFERENCE_NV); break;
    case CompositionOp::OP_EXCLUSION: BlendEquation(GL_EXCLUSION_NV); break;
    case CompositionOp::OP_HUE: BlendEquation(GL_HSL_HUE_NV); break;
    case CompositionOp::OP_SATURATION: BlendEquation(GL_HSL_SATURATION_NV); break;
    case CompositionOp::OP_COLOR: BlendEquation(GL_HSL_COLOR_NV); break;
    case CompositionOp::OP_LUMINOSITY: BlendEquation(GL_HSL_LUMINOSITY_NV); break;
  }

  mBlendMode = aBlendMode;
}

void
GL::BlendBarrier()
{
  if (HasExtension(NV_blend_equation_advanced_coherent)) {
    return;
  }

  BlendBarrierNV();
}

void
GL::EnableMultisample()
{
  if (mMultisampleEnabled) {
    return;
  }

  Enable(GL_MULTISAMPLE);
  mMultisampleEnabled = true;
}

void
GL::DisableMultisample()
{
  if (!mMultisampleEnabled) {
    return;
  }

  Disable(GL_MULTISAMPLE);
  mMultisampleEnabled = false;
}

void
GL::SetTexture(TextureUnit aTextureUnit, GLenum aTextureTarget,
               GLuint aTextureId)
{
  MOZ_ASSERT(IsCurrent());

  if (mActiveTextureTargets[aTextureUnit] == aTextureTarget
      && mBoundTextures[aTextureUnit] == aTextureId) {
    return;
  }

  if (mActiveTextureTargets[aTextureUnit]
      && mActiveTextureTargets[aTextureUnit] != aTextureTarget) {
    BindMultiTextureEXT(GL_TEXTURE0 + aTextureUnit,
                        mActiveTextureTargets[aTextureUnit], 0);
  }

  BindMultiTextureEXT(GL_TEXTURE0 + aTextureUnit, aTextureTarget, aTextureId);
  mActiveTextureTargets[aTextureUnit] = aTextureTarget;
  mBoundTextures[aTextureUnit] = aTextureId;
}

void
GL::DeleteTexture(GLuint aTextureId)
{
  MOZ_ASSERT(IsCurrent());

  DeleteTextures(1, &aTextureId);

  for (size_t i = 0; i < TEXTURE_UNIT_COUNT; i++) {
    if (mBoundTextures[i] == aTextureId) {
      mActiveTextureTargets[i] = 0;
      mBoundTextures[i] = 0;
    }
  }
}

void
GL::SetTexGen(TextureUnit aTextureUnit, TexGenComponents aComponents,
              const GLfloat* aCoefficients)
{
  MOZ_ASSERT(IsCurrent());

  if (mTexGenComponents[aTextureUnit] == aComponents) {
    if (!aComponents) {
      return;
    }
    if (mTexGenTransformIds[aTextureUnit] == mTransformId
        && !memcmp(mTexGenCoefficients[aTextureUnit], aCoefficients,
                   aComponents * 3 * sizeof(GLfloat))) {
      return;
    }
  }

  for (int i = mTexGenComponents[aTextureUnit]; i < aComponents; i++) {
    Enablei(GL_TEXTURE_GEN_S + i, aTextureUnit);
  }
  for (int i = aComponents; i < mTexGenComponents[aTextureUnit]; i++) {
    Disablei(GL_TEXTURE_GEN_S + i, aTextureUnit);
  }
  for (int i = 0; i < aComponents; i++) {
    const GLfloat plane[] = {aCoefficients[3 * i], aCoefficients[3 * i + 1],
                             0, aCoefficients[3 * i + 2]};
    MultiTexGenfvEXT(GL_TEXTURE0 + aTextureUnit, GL_S + i, GL_EYE_PLANE, plane);
  }

  PathTexGenNV(GL_TEXTURE0 + aTextureUnit, GL_EYE_LINEAR,
               aComponents, aCoefficients);

  mTexGenComponents[aTextureUnit] = aComponents;
  mTexGenTransformIds[aTextureUnit] = mTransformId;
  memcpy(mTexGenCoefficients[aTextureUnit], aCoefficients,
         aComponents * 3 * sizeof(GLfloat));
}

void
GL::EnableTexCoordArray(TextureUnit aTextureUnit, const GLfloat* aTexCoords)
{
  if (!mTexCoordArraysEnabled[aTextureUnit]) {
    EnableClientStateiEXT(GL_TEXTURE_COORD_ARRAY, aTextureUnit);
    mTexCoordArraysEnabled[aTextureUnit] = true;
  }

  MultiTexCoordPointerEXT(GL_TEXTURE0 + aTextureUnit, 2, GL_FLOAT, 0, aTexCoords);
}

void
GL::EnableTexCoordArrayToRect(TextureUnit aTextureUnit,
                              const Point& aPosition, const Size& aSize)
{
  RectToClientArray(mTexCoordRectArray[aTextureUnit], aPosition, aSize);
  EnableTexCoordArray(aTextureUnit, mTexCoordRectArray[aTextureUnit]);
}

void
GL::EnableTexCoordArrayToRect(TextureUnit aTextureUnit, const Rect& aRect)
{
  EnableTexCoordArrayToRect(aTextureUnit, aRect.TopLeft(), aRect.Size());
}

void
GL::EnableTexCoordArrayToUnitRect(TextureUnit aTextureUnit)
{
  EnableTexCoordArrayToRect(aTextureUnit, Rect(0, 0, 1, 1));
}

void
GL::DisableTexCoordArray(TextureUnit aTextureUnit)
{
  if (!mTexCoordArraysEnabled[aTextureUnit]) {
    return;
  }

  DisableClientStateiEXT(GL_TEXTURE_COORD_ARRAY, aTextureUnit);
  mTexCoordArraysEnabled[aTextureUnit] = false;
}

void
GL::SetVertexArray(const GLfloat* aVertices)
{
  VertexPointer(2, GL_FLOAT, 0, aVertices);
}

void
GL::SetVertexArrayToRect(const Point& aPosition, const Size& aSize)
{
  RectToClientArray(mVertexRectArray, aPosition, aSize);
  SetVertexArray(mVertexRectArray);
}

void
GL::SetVertexArrayToRect(const Rect& aRect)
{
  SetVertexArrayToRect(aRect.TopLeft(), aRect.Size());
}

}
}
}
