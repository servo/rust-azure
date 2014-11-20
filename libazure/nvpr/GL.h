/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_NVPR_GL_H_
#define MOZILLA_GFX_NVPR_GL_H_

#include "2D.h"
#include "GLDefs.h"
#include <memory>
#include <stack>

namespace mozilla {
namespace gfx {
namespace nvpr {

typedef uint64_t UniqueId;

class ConvexPolygon;

struct UserData {
  class Object { public: virtual ~Object() {} };

  std::unique_ptr<Object> mPathCache;
  std::unique_ptr<Object> mColorRampData;
  std::unique_ptr<Object> mFonts;
  std::unique_ptr<Object> mPaintShaders;
  std::unique_ptr<Object> mShadowShaders;
};

class GL
{
public:
  bool IsValid() const { return mIsValid; }

  bool IsCurrent() const;
  void MakeCurrent() const;

  bool BlitTextureToForeignTexture(const IntSize& aSize, GLuint aSourceTextureId,
                                   void* aForeignContext, GLuint aForeignTextureId);

  enum Extension {
    EXT_texture_filter_anisotropic,
    EXT_direct_state_access,
    ARB_texture_storage,
    NV_path_rendering,
    NV_blend_equation_advanced,
    NV_blend_equation_advanced_coherent,
    EXTENSION_COUNT
  };
  bool HasExtension(Extension aExtension) const
  {
    return mSupportedExtensions[aExtension];
  }

  GLint MaxRenderbufferSize() const { return mMaxRenderbufferSize; }
  GLint MaxTextureSize() const { return mMaxTextureSize; }
  GLint MaxClipPlanes() const { return mMaxClipPlanes; }
  GLint MaxAnisotropy() const { return mMaxAnisotropy; }

  template<typename T> T&
  GetUserObject(std::unique_ptr<UserData::Object> UserData::*aObject)
  {
    std::unique_ptr<UserData::Object>& object = mUserData.*aObject;
    if (!object) {
      object.reset(new T());
    }
    return static_cast<T&>(*object.get());
  }

  UniqueId GetUniqueId() { return mNextUniqueId++; }
  UniqueId TransformId() const { return mTransformId; }
  UniqueId ClipPolygonId() const { return mClipPolygonId; }

  void SetSize(const IntSize& aSize);

  void SetFramebuffer(GLenum aFramebufferTarget, GLuint aFramebuffer);
  void SetFramebufferToTexture(GLenum aFramebufferTarget, GLenum aTextureTarget,
                               GLuint aTextureId);

  void SetTransform(const Matrix& aTransform, UniqueId aTransformId);
  void ScaleTransform(GLfloat x, GLfloat y);
  void SetTransformToIdentity();

  enum ColorWriteMaskBit { WRITE_NONE = 0x0, WRITE_RED = 0x1, WRITE_GREEN = 0x2,
                           WRITE_BLUE = 0x4, WRITE_ALPHA = 0x8,
                           WRITE_COLOR_AND_ALPHA = 0xf };
  typedef unsigned ColorWriteMask;
  void SetColorWriteMask(ColorWriteMask aColorWriteMask);

  void SetClearColor(const Color& aColor);
  void SetClearColor(const Color& aColor, GLfloat aAlpha);

  void EnableScissorTest(const IntRect& aScissorRect);
  void DisableScissorTest();

  void EnableClipPlanes(const ConvexPolygon& aPolygon, UniqueId aPolygonId);
  void DisableClipPlanes();

  enum UnaryStencilTest { PASS_IF_NOT_ZERO, PASS_IF_ALL_SET };
  enum BinaryStencilTest { ALWAYS_PASS, PASS_IF_EQUAL, PASS_IF_NOT_EQUAL };
  enum StencilOperation { LEAVE_UNCHANGED, CLEAR_PASSING_VALUES,
                          REPLACE_PASSING_WITH_COMPARAND,
                          REPLACE_PASSING_CLEAR_FAILING };
  void EnableStencilTest(UnaryStencilTest aTest, GLuint aTestMask,
                         StencilOperation aOp, GLuint aWriteMask = ~0);
  void EnableStencilTest(BinaryStencilTest aTest,
                         GLint aComparand, GLuint aTestMask,
                         StencilOperation aOp, GLuint aWriteMask = ~0);
  void DisableStencilTest();

  void ConfigurePathStencilTest(GLubyte aClipBits);

  void SetShaderProgram(GLuint aShaderProgram);
  void DeleteShaderProgram(GLuint aShaderProgram);

  void SetBlendMode(CompositionOp aBlendMode);
  void BlendBarrier();

  void EnableMultisample();
  void DisableMultisample();

  enum TextureUnit { UNIT_0, UNIT_1, TEXTURE_UNIT_COUNT };
  void SetTexture(TextureUnit aTextureUnit, GLenum aTextureTarget,
                  GLuint aTextureId);
  void DeleteTexture(GLuint aTextureId);

  enum TexGenComponents { TEXGEN_NONE, TEXGEN_S, TEXGEN_ST };
  void SetTexGen(TextureUnit aTextureUnit, TexGenComponents aComponents,
                 const GLfloat* aCoefficients = nullptr);

  void EnableTexCoordArray(TextureUnit aTextureUnit, const GLfloat* aTexCoords);
  void EnableTexCoordArrayToRect(TextureUnit aTextureUnit,
                                 const Point& aPosition, const Size& aSize);
  void EnableTexCoordArrayToRect(TextureUnit aTextureUnit, const Rect& aRect);
  void EnableTexCoordArrayToUnitRect(TextureUnit aTextureUnit);
  void DisableTexCoordArray(TextureUnit aTextureUnit);

  void SetVertexArray(const GLfloat* aVertices);
  void SetVertexArrayToRect(const Point& aPosition, const Size& aSize);
  void SetVertexArrayToRect(const Rect& aRect);

protected:
  GL();
  virtual ~GL();

  void Initialize();

private:
  bool mIsValid;
  bool mSupportedExtensions[EXTENSION_COUNT];
  GLint mMaxRenderbufferSize;
  GLint mMaxTextureSize;
  GLint mMaxClipPlanes;
  GLint mMaxAnisotropy;
  UserData mUserData;
  UniqueId mNextUniqueId;
  GLuint mTextureFramebuffer1D;
  GLuint mTextureFramebuffer2D;
  GLfloat mTexCoordRectArray[TEXTURE_UNIT_COUNT][8];
  GLfloat mVertexRectArray[8];

  // GL state.
  IntSize mSize;
  GLuint mReadFramebuffer;
  GLuint mDrawFramebuffer;
  UniqueId mTransformId;
  size_t mNumClipPlanes;
  UniqueId mClipPolygonId;
  ColorWriteMask mColorWriteMask;
  Color mClearColor;
  Color mColor;
  bool mScissorTestEnabled;
  IntRect mScissorRect;
  bool mStencilTestEnabled;
  BinaryStencilTest mStencilTest;
  GLint mStencilComparand;
  GLuint mStencilTestMask;
  StencilOperation mStencilOp;
  GLuint mStencilWriteMask;
  GLubyte mPathStencilFuncBits;
  CompositionOp mBlendMode;
  bool mMultisampleEnabled;
  GLuint mShaderProgram;
  unsigned mTexGenComponents[TEXTURE_UNIT_COUNT];
  UniqueId mTexGenTransformIds[TEXTURE_UNIT_COUNT];
  GLfloat mTexGenCoefficients[TEXTURE_UNIT_COUNT][6];
  GLenum mActiveTextureTargets[TEXTURE_UNIT_COUNT];
  GLenum mBoundTextures[TEXTURE_UNIT_COUNT];
  bool mTexCoordArraysEnabled[TEXTURE_UNIT_COUNT];

#define FOR_ALL_PUBLIC_GL_ENTRY_POINTS(MACRO) \
  MACRO(GenTextures) \
  MACRO(CreateShader) \
  MACRO(ShaderSource) \
  MACRO(CompileShader) \
  MACRO(GetIntegerv) \
  MACRO(GetShaderiv) \
  MACRO(GetShaderInfoLog) \
  MACRO(GetProgramiv) \
  MACRO(GetProgramInfoLog) \
  MACRO(CreateProgram) \
  MACRO(AttachShader) \
  MACRO(LinkProgram) \
  MACRO(DeleteShader) \
  MACRO(GetUniformLocation) \
  MACRO(ProgramUniform1iEXT) \
  MACRO(ProgramUniform1fEXT) \
  MACRO(ProgramUniform1fvEXT) \
  MACRO(ProgramUniform2fvEXT) \
  MACRO(ProgramUniform4fvEXT) \
  MACRO(GenRenderbuffers) \
  MACRO(DeleteRenderbuffers) \
  MACRO(Clear) \
  MACRO(DrawArrays) \
  MACRO(BlitFramebuffer) \
  MACRO(Rectf) \
  MACRO(GenFramebuffers) \
  MACRO(DeleteFramebuffers) \
  MACRO(PixelStorei) \
  MACRO(ClipPlane) \
  MACRO(GetString) \
  MACRO(Flush) \
  MACRO(Finish) \
  MACRO(TextureStorage1DEXT) \
  MACRO(TextureSubImage1DEXT) \
  MACRO(GenerateTextureMipmapEXT) \
  MACRO(TextureParameteriEXT) \
  MACRO(NamedRenderbufferStorageMultisampleEXT) \
  MACRO(NamedFramebufferRenderbufferEXT) \
  MACRO(TextureImage2DEXT) \
  MACRO(TextureSubImage2DEXT) \
  MACRO(GetTextureImageEXT) \
  MACRO(GenPathsNV) \
  MACRO(PathCommandsNV) \
  MACRO(PathGlyphRangeNV) \
  MACRO(GetPathMetricRangeNV) \
  MACRO(StencilStrokePathNV) \
  MACRO(CoverStrokePathNV) \
  MACRO(StencilFillPathInstancedNV) \
  MACRO(CoverFillPathInstancedNV) \
  MACRO(StencilFillPathNV) \
  MACRO(CoverFillPathNV) \
  MACRO(DeletePathsNV) \
  MACRO(PathParameterfNV) \
  MACRO(PathParameteriNV) \
  MACRO(PathDashArrayNV) \
  MACRO(IsPointInFillPathNV) \
  MACRO(IsPointInStrokePathNV) \
  MACRO(GetPathParameterfvNV) \
  MACRO(TransformPathNV) \
  MACRO(GetPathParameterivNV) \
  MACRO(GetPathCommandsNV) \
  MACRO(GetPathCoordsNV)

#define FOR_ALL_PRIVATE_GL_ENTRY_POINTS(MACRO) \
  MACRO(DeleteTextures) \
  MACRO(DeleteProgram) \
  MACRO(EnableClientState) \
  MACRO(DebugMessageCallback) \
  MACRO(DebugMessageControl) \
  MACRO(Viewport) \
  MACRO(BindFramebuffer) \
  MACRO(ColorMask) \
  MACRO(Scissor) \
  MACRO(StencilFunc) \
  MACRO(StencilOp) \
  MACRO(StencilMask) \
  MACRO(ClearColor) \
  MACRO(UseProgram) \
  MACRO(BlendEquation) \
  MACRO(Enable) \
  MACRO(Disable) \
  MACRO(Enablei) \
  MACRO(Disablei) \
  MACRO(VertexPointer) \
  MACRO(MatrixOrthoEXT) \
  MACRO(MatrixLoadfEXT) \
  MACRO(MatrixPushEXT) \
  MACRO(MatrixPopEXT) \
  MACRO(MatrixLoadIdentityEXT) \
  MACRO(NamedFramebufferTexture1DEXT) \
  MACRO(NamedFramebufferTexture2DEXT) \
  MACRO(MultiTexGenfvEXT) \
  MACRO(BindMultiTextureEXT) \
  MACRO(EnableClientStateiEXT) \
  MACRO(DisableClientStateiEXT) \
  MACRO(MultiTexCoordPointerEXT) \
  MACRO(PathStencilFuncNV) \
  MACRO(PathTexGenNV) \
  MACRO(BlendBarrierNV) \
  MACRO(MatrixScalefEXT)

#define DECLARE_GL_METHOD(NAME) \
  GL##NAME NAME;

public:
  FOR_ALL_PUBLIC_GL_ENTRY_POINTS(DECLARE_GL_METHOD);

protected:
  FOR_ALL_PRIVATE_GL_ENTRY_POINTS(DECLARE_GL_METHOD);

#undef DECLARE_GL_METHOD

  GL(const GL&);
  GL& operator =(const GL&);
};

extern GL* gl;
void InitializeGLIfNeeded();

}
}
}

#endif /* MOZILLA_GFX_NVPR_GL_H_ */
