/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "Paint.h"
#include "ShaderProgram.h"
#include "GradientStopsNVpr.h"
#include "SourceSurfaceNVpr.h"
#include "Logging.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

namespace mozilla {
namespace gfx {
namespace nvpr {

PaintConfig::PaintConfig()
  : mPaintMode(MODE_NONE)
  , mTexGenComponents(GL::TEXGEN_NONE)
{
}

void
PaintConfig::SetToPattern(const Pattern& aPattern)
{
  switch (aPattern.GetType()) {
    default: MOZ_ASSERT(!"Invalid pattern type");
    case PatternType::COLOR:
      SetToColor(static_cast<const ColorPattern&>(aPattern).mColor);
      return;

    case PatternType::SURFACE: {
      const SurfacePattern& pat(static_cast<const SurfacePattern&>(aPattern));
      MOZ_ASSERT(pat.mSurface->GetType() == SURFACE_NVPR_TEXTURE);

      SetToTexgenSurface(static_cast<SourceSurfaceNVpr*>(pat.mSurface.get()),
                         pat.mExtendMode, pat.mFilter, pat.mMatrix);
      return;
    }

    case PatternType::LINEAR_GRADIENT: {
      const LinearGradientPattern& pat(
        static_cast<const LinearGradientPattern&>(aPattern));
      MOZ_ASSERT(pat.mStops->GetBackendType() == BACKEND_NVPR);

      SetToLinearGradient(static_cast<GradientStopsNVpr*>(pat.mStops.get()),
                          pat.mBegin, pat.mEnd, pat.mMatrix);
      return;
    }

    case PatternType::RADIAL_GRADIENT: {
      const RadialGradientPattern& pat(
        static_cast<const RadialGradientPattern&>(aPattern));
      MOZ_ASSERT(pat.mStops->GetBackendType() == BACKEND_NVPR);

      if (pat.mRadius1 == 0) {
        SetToFocalGradient(static_cast<GradientStopsNVpr*>(pat.mStops.get()),
                           pat.mCenter1, pat.mCenter2, pat.mRadius2, pat.mMatrix);
        return;
      }

      SetToRadialGradient(static_cast<GradientStopsNVpr*>(pat.mStops.get()),
                          pat.mCenter1, pat.mRadius1, pat.mCenter2,
                          pat.mRadius2, pat.mMatrix);
      return;
    }
  }
}

void
PaintConfig::SetToColor(const Color& aColor)
{
  mPaintMode = MODE_SOLID_COLOR;
  uColor[0] = aColor.a * aColor.r;
  uColor[1] = aColor.a * aColor.g;
  uColor[2] = aColor.a * aColor.b;
  uColor[3] = aColor.a;
}

void
PaintConfig::SetToSurface(SourceSurfaceNVpr* aSurface, Filter aFilter)
{
  mPaintMode = MODE_TEXTURE_2D;
  mTextureId = *aSurface;

  aSurface->SetWrapMode(ExtendMode::CLAMP);
  aSurface->SetFilter(aFilter);
}

void
PaintConfig::SetToClampedSurface(SourceSurfaceNVpr* aSurface, Filter aFilter,
                                 const Rect& aSamplingBounds)
{
  Rect clampRect = aSamplingBounds;
  clampRect.Deflate(0.5f);
  clampRect.ScaleInverse(aSurface->GetSize().width, aSurface->GetSize().height);

  mPaintMode = MODE_TEXTURE_2D_CLAMPED;
  mTextureId = *aSurface;
  uClampRect[0] = clampRect.x;
  uClampRect[1] = clampRect.y;
  uClampRect[2] = clampRect.XMost();
  uClampRect[3] = clampRect.YMost();

  aSurface->SetWrapMode(ExtendMode::CLAMP);
  aSurface->SetFilter(aFilter);
}

void
PaintConfig::SetToTexgenSurface(SourceSurfaceNVpr* aSurface,
                                ExtendMode aExtendMode, Filter aFilter,
                                const Matrix& aMatrix)
{
  Matrix textureCoords = aMatrix;
  textureCoords.Invert();
  textureCoords.PostScale(1.0f / aSurface->GetSize().width,
                          1.0f / aSurface->GetSize().height);

  mPaintMode = MODE_TEXTURE_2D;
  mTextureId = *aSurface;
  mTexGenComponents = GL::TEXGEN_ST;
  SetTexGenCoefficients(textureCoords);

  aSurface->SetWrapMode(aExtendMode);
  aSurface->SetFilter(aFilter);
}

void
PaintConfig::SetToLinearGradient(GradientStopsNVpr* aStops,
                                 const Point& aBeginPoint, const Point& aEndPoint,
                                 const Matrix& aMatrix)
{
  const Point beginPoint = aMatrix * aBeginPoint;
  const Point vector = aMatrix * (aEndPoint - aBeginPoint);
  const float lengthSquared = (vector.x * vector.x + vector.y * vector.y);

  if (!lengthSquared || !*aStops) {
    SetToColor(aStops->FinalColor());
    return;
  }

  mPaintMode = MODE_TEXTURE_1D;
  mTextureId = *aStops;
  mTexGenComponents = GL::TEXGEN_S;
  mTexGenCoefficients[0] = vector.x / lengthSquared;
  mTexGenCoefficients[1] = vector.y / lengthSquared;
  mTexGenCoefficients[2] =
    -(beginPoint.x * vector.x + beginPoint.y * vector.y) / lengthSquared;
}

void
PaintConfig::SetToFocalGradient(GradientStopsNVpr* aStops,
                                const Point& aFocalPoint,
                                const Point& aEndCenter, float aEndRadius,
                                const Matrix& aMatrix)
{
  if (!aEndRadius) {
    mPaintMode = MODE_NONE;
    return;
  }

  if (!*aStops) {
    // TODO: This doesn't exclude regions not in the gradient from being drawn.
    SetToColor(aStops->FinalColor());
    return;
  }

  mTextureId = *aStops;
  mTexGenComponents = GL::TEXGEN_ST;

  // Setup a transformation where the gradient is the unit-circle.
  Matrix gradientCoords;
  gradientCoords.PreScale(1 / aEndRadius, 1 / aEndRadius);
  gradientCoords.PreTranslate(-aEndCenter.x, -aEndCenter.y);

  Point focalPoint = gradientCoords * aFocalPoint;
  const float focalOffsetSquared = focalPoint.x * focalPoint.x
                                   + focalPoint.y * focalPoint.y;

  // The gradient is drawn in 'pattern space' onto an infinite plane.
  // aMatrix transforms the pattern-space plane back into user space.
  Matrix patternSpace = aMatrix;
  patternSpace.Invert();

  if (fabs(focalOffsetSquared) < 1e-5f) { // The focal point is at [0, 0].
    mPaintMode = MODE_FOCAL_GRAD_CENTERED;
    SetTexGenCoefficients(gradientCoords * patternSpace);
    return;
  }

  // With the following variables inside the unit circle:
  //
  //   f = focal point, normalized to a unit-circle gradient
  //   p = sample's [x,y] location, normalized to a unit-circle gradient
  //
  // A shader program can find the sample's gradient offset using the general
  // radial gradient equation:
  //
  //   offset = (dot(p - f, f) +/- sqrt(dot(p - f, p - f) - cross(p - f, f)^2))
  //            / (1 - dot(f, f))
  //
  // Below we massage this equation to make the math more efficient.

  // 1) Rotate the gradient so the focal point is on the x-axis (i.e. f.y == 0):
  //
  //   Now offset = ((p - f).x * f.x +/- sqrt((p - f).x^2 + (p - f).y^2
  //                                          - (p - f).y^2 * f.x^2))
  //                / (1 - dot(f, f))
  //
  //              = ((p - f).x * f.x +/- sqrt((p - f).x^2
  //                                          + (1 - f.x^2) * (p - f).y^2))
  //                / (1 - dot(f, f))
  //
  Matrix rotation = Matrix::Rotation(-atan2(focalPoint.y, focalPoint.x));
  gradientCoords = gradientCoords * rotation;
  focalPoint = Point(sqrt(focalOffsetSquared), 0);

  // 2) Let q = p - f
  //
  // Now offset = (q.x * f.x +/- sqrt(q.x^2 + (1 - f.x^2) * q.y^2))
  //              / (1 - dot(f, f))
  //
  Matrix qCoords = gradientCoords;
  qCoords.PostTranslate(-focalPoint.x, -focalPoint.y);

  if (fabs(1 - focalOffsetSquared) < 1e-5f) {
    // The focal point is touching the circle. We can't use the general equation
    // because it would divide by zero. Instead we use a special-case formula
    // knowing that f = [0, 1]:
    //
    // offset = dot(p - f, p - f) / (-2 * (p - f).x)
    //
    //        = dot(q, q) / (-2 * q.x)
    //
    mPaintMode = MODE_FOCAL_GRAD_TOUCHING;
    SetTexGenCoefficients(qCoords * patternSpace);

    return;
  }

  // 3) Let a = 1 / (1 - dot(f, f)):
  //
  // Now offset = a * q.x * f.x + sqrt(a^2 * q.x^2 + a^2 * (1 - f.x^2) * q.y^2))
  //
  // (Note that this reverses the sign of the sqrt when a < 0, and that's exacly
  //  what we want since it allows us to just always use + with it)
  //
  float a = 1 / (1 - focalOffsetSquared);

  if (a < 0) { // The focal point is outside the circle.
    // 4) q.x *= a
    //    q.y *= a
    //
    // Now offset = q.x * f.x + sqrt(q.x^2 + (1 - f.x^2) * q.y^2))
    //
    qCoords.PostScale(a, a);

    mPaintMode = MODE_FOCAL_GRAD_OUTSIDE;
    SetTexGenCoefficients(qCoords * patternSpace);
    uFocalX = focalPoint.x;
    u1MinuxFx_2 = 1 - focalPoint.x * focalPoint.x;

    return;
  }

  // 4) q.x *= a
  //    q.y *= a * sqrt(1 - f.x^2)
  //
  // Now offset = q.x * f.x + sqrt(q.x^2 + q.y^2)
  //
  //            = q.x * f.x + length(q)
  //
  qCoords.PostScale(a, a * sqrt(1 - focalOffsetSquared));

  mPaintMode = MODE_FOCAL_GRAD_INSIDE;
  SetTexGenCoefficients(qCoords * patternSpace);
  uFocalX = focalPoint.x;
}

void
PaintConfig::SetToRadialGradient(GradientStopsNVpr* aStops,
                                 const Point& aBeginCenter, float aBeginRadius,
                                 const Point& aEndCenter, float aEndRadius,
                                 const Matrix& aMatrix)
{
  if (aBeginCenter == aEndCenter && aBeginRadius == aEndRadius) {
    mPaintMode = MODE_NONE;
    return;
  }

  if (!*aStops) {
    // TODO: This doesn't exclude regions not in the gradient from being drawn.
    SetToColor(aStops->FinalColor());
    return;
  }

  // Setup a transformation where the begin circle is the unit-circle.
  Matrix gradientCoords;
  gradientCoords.PreScale(1 / aBeginRadius, 1 / aBeginRadius);
  gradientCoords.PreTranslate(-aBeginCenter.x, -aBeginCenter.y);

  // At this point, the begin circle is the unit-circle and we define the
  // following variables:
  //
  //   c = end circle's center
  //   r = end circle's radius
  //   p = sample's [x,y] location
  //   A = dot(c, c) - r^2 + 2 * r - 1
  //
  // A shader program can use the this equation to find the gradient offset:
  //
  //   offset = (dot(c, p) + r - 1 +/- sqrt((dot(c, p) + r - 1)^2
  //                                        - 4 * A * (dot(p, p) - 1))) / A
  Point endCenter = gradientCoords * aEndCenter;
  float endRadius = aEndRadius / aBeginRadius;
  float A = endCenter.x * endCenter.x + endCenter.y * endCenter.y
            - endRadius * endRadius + 2 * endRadius - 1;

  // TODO: Make a special case for A ~= 0.

  // Let q = (1 / A) * p, B = (r - 1) / A, C = 1 / A
  //
  // Now      d = dot(c, q) + B
  //     offset = d +/- sqrt(d^2 - A * dot(q, q) + C)
  //
  // (Note that this reverses the sign of the sqrt when A < 0)
  float C = 1 / A;
  float B = (endRadius - 1) * C;
  Matrix qCoords = gradientCoords;
  qCoords.PostScale(C, C);

  mTextureId = *aStops;
  mTexGenComponents = GL::TEXGEN_ST;

  // The gradient is drawn in 'pattern space' onto an infinite plane.
  // aMatrix transforms the pattern-space plane back into user space.
  Matrix patternSpace = aMatrix;
  patternSpace.Invert();
  SetTexGenCoefficients(qCoords * patternSpace);

  uEndCenter[0] = endCenter.x;
  uEndCenter[1] = endCenter.y;
  uA = A;
  uB = B;
  uC = C;

  if (A >= 0) {
    mPaintMode = (aEndRadius - aBeginRadius > 1e-5f)
      ? MODE_RADIAL_GRAD_OUTSIDE_DISCARD_LOW
      : MODE_RADIAL_GRAD_OUTSIDE_DISCARD_HIGH;
    uOffsetLimit = aBeginRadius / (aBeginRadius - aEndRadius);
    return;
  }

  mPaintMode = (aEndRadius > aBeginRadius)
    ? MODE_RADIAL_GRAD_INSIDE_ADD_SQRT
    : MODE_RADIAL_GRAD_INSIDE_SUBTRACT_SQRT;
}

void
PaintConfig::SetTexGenCoefficients(const Matrix& aTransform)
{
  mTexGenCoefficients[0] = aTransform._11;
  mTexGenCoefficients[1] = aTransform._21;
  mTexGenCoefficients[2] = aTransform._31;
  mTexGenCoefficients[3] = aTransform._12;
  mTexGenCoefficients[4] = aTransform._22;
  mTexGenCoefficients[5] = aTransform._32;
}


static string
AppendTextureUnit(const char* aName, GL::TextureUnit aUnit)
{
  stringstream fullName;
  fullName << aName << '_' << aUnit;
  return fullName.str();
}

class PaintShader::ColorFunction : public RefCounted<ColorFunction> {
public:
  static TemporaryRef<ColorFunction> Create(PaintConfig::PaintMode aPaintMode,
                                            GL::TextureUnit aTextureUnit);
  virtual ~ColorFunction() {}

  virtual bool IsEmpty() const { return false; }

  virtual void WriteDeclarations(std::ostream& aOut) const {}
  virtual void WriteColorFunction(std::ostream& aOut) const = 0;
  virtual void Initialize(GLuint aShaderProgram) {}

  virtual void ApplyFragmentUniforms(const PaintConfig& aConfig) {}

protected:
  ColorFunction() {}
};

class EmptyFunction : public PaintShader::ColorFunction {
public:
  virtual bool IsEmpty() const { return true; }

  virtual void WriteColorFunction(std::ostream& aOut) const
  {
    aOut << "return vec4(1);" << endl;
  }
};

class SolidColorFunction : public PaintShader::ColorFunction {
public:
  SolidColorFunction(GL::TextureUnit aTextureUnit)
    : uColor(AppendTextureUnit("uColor", aTextureUnit).c_str())
  {}

  virtual void WriteDeclarations(std::ostream& aOut) const
  {
    aOut << "uniform vec4 " << uColor << ";" << endl;
  }

  virtual void WriteColorFunction(std::ostream& aOut) const
  {
    aOut << "return " << uColor << ";" << endl;
  }

  virtual void Initialize(GLuint aShaderProgram)
  {
    uColor.Initialize(aShaderProgram);
  }

  virtual void ApplyFragmentUniforms(const PaintConfig& aConfig)
  {
    const GLfloat* color = aConfig.uColor;

    if (color[3] == 1) {
      uColor.Set(color);
    } else {
      const float a = color[3];
      const float premultiplied[] = {a * color[0], a * color[1], a * color[2], a};
      uColor.Set(premultiplied);
    }
  }

protected:
  UniformVec4 uColor;
};

class Texture1DFunction : public PaintShader::ColorFunction {
public:
  Texture1DFunction(GL::TextureUnit aTextureUnit)
    : mTextureUnit(aTextureUnit)
    , uTexture(AppendTextureUnit("uTexture", aTextureUnit).c_str(), aTextureUnit)
  {}

  virtual void WriteDeclarations(std::ostream& aOut) const
  {
    aOut << "uniform sampler1D " << uTexture << ";" << endl;
  }

  virtual void WriteColorFunction(std::ostream& aOut) const
  {
    aOut << "float texCoord = gl_TexCoord[" << mTextureUnit << "].s;" << endl;
    aOut << "return texture1D(" << uTexture << ", texCoord);" << endl;
  }

  virtual void Initialize(GLuint aShaderProgram)
  {
    uTexture.Initialize(aShaderProgram);
  }

  virtual void ApplyFragmentUniforms(const PaintConfig& aConfig)
  {
    gl->SetTexture(mTextureUnit, GL_TEXTURE_1D, aConfig.mTextureId);
  }

protected:
  GL::TextureUnit mTextureUnit;
  UniformSampler uTexture;
};

class Texture2DFunction : public PaintShader::ColorFunction {
public:
  Texture2DFunction(GL::TextureUnit aTextureUnit)
    : mTextureUnit(aTextureUnit)
    , uTexture(AppendTextureUnit("uTexture", aTextureUnit).c_str(), aTextureUnit)
  {}

  virtual void WriteDeclarations(std::ostream& aOut) const
  {
    aOut << "uniform sampler2D " << uTexture << ";" << endl;
  }

  virtual void WriteColorFunction(std::ostream& aOut) const
  {
    aOut << "vec2 texCoords = gl_TexCoord[" << mTextureUnit << "].st;" << endl;
    aOut << "return texture2D(" << uTexture << ", texCoords);" << endl;
  }

  virtual void Initialize(GLuint aShaderProgram)
  {
    uTexture.Initialize(aShaderProgram);
  }

  virtual void ApplyFragmentUniforms(const PaintConfig& aConfig)
  {
    gl->SetTexture(mTextureUnit, GL_TEXTURE_2D, aConfig.mTextureId);
  }

protected:
  GL::TextureUnit mTextureUnit;
  UniformSampler uTexture;
};

class Texture2DClampedFunction : public Texture2DFunction {
public:
  Texture2DClampedFunction(GL::TextureUnit aTextureUnit)
    : Texture2DFunction(aTextureUnit)
    , uClampRect(AppendTextureUnit("uClampRect", aTextureUnit).c_str())
  {}

  virtual void WriteDeclarations(std::ostream& aOut) const
  {
    Texture2DFunction::WriteDeclarations(aOut);
    aOut << "uniform vec4 " << uClampRect << ";" << endl;
  }

  virtual void WriteColorFunction(std::ostream& aOut) const
  {
    aOut << "vec2 texCoords = clamp(gl_TexCoord[" << mTextureUnit << "].st, "
                                    << uClampRect << ".xy, "
                                    << uClampRect << ".zw);" << endl;
    aOut << "return texture2D(" << uTexture << ", texCoords);" << endl;
  }

  virtual void Initialize(GLuint aShaderProgram)
  {
    uClampRect.Initialize(aShaderProgram);
  }

  virtual void ApplyFragmentUniforms(const PaintConfig& aConfig)
  {
    Texture2DFunction::ApplyFragmentUniforms(aConfig);
    uClampRect.Set(aConfig.uClampRect);
  }

protected:
  UniformVec4 uClampRect;
};

class FocalGradCenteredFunction : public Texture1DFunction {
public:
  FocalGradCenteredFunction(GL::TextureUnit aTextureUnit)
    : Texture1DFunction(aTextureUnit)
  {}

  virtual void WriteColorFunction(std::ostream& aOut) const
  {
    aOut << "vec2 p = gl_TexCoord[" << mTextureUnit << "].st;" << endl;
    aOut << "float offset = length(p);" << endl;
    aOut << "return texture1D(" << uTexture << ", offset);" << endl;
  }
};

class FocalGradInsideFunction : public Texture1DFunction {
public:
  FocalGradInsideFunction(GL::TextureUnit aTextureUnit)
    : Texture1DFunction(aTextureUnit)
    , uFocalX(AppendTextureUnit("uFocalX", aTextureUnit).c_str())
  {}

  virtual void WriteDeclarations(std::ostream& aOut) const
  {
    Texture1DFunction::WriteDeclarations(aOut);
    aOut << "uniform float " << uFocalX << ";" << endl;
  }

  virtual void WriteColorFunction(std::ostream& aOut) const
  {
    aOut << "vec2 q = gl_TexCoord[" << mTextureUnit << "].st;" << endl;
    aOut << "float offset = q.x * " << uFocalX << " + length(q);" << endl;
    aOut << "return texture1D(" << uTexture << ", offset);" << endl;
  }

  virtual void Initialize(GLuint aShaderProgram)
  {
    uFocalX.Initialize(aShaderProgram);
  }

  virtual void ApplyFragmentUniforms(const PaintConfig& aConfig)
  {
    Texture1DFunction::ApplyFragmentUniforms(aConfig);
    uFocalX.Set(aConfig.uFocalX);
  }

protected:
  UniformFloat uFocalX;
};

class FocalGradOutsideFunction : public Texture1DFunction {
public:
  FocalGradOutsideFunction(GL::TextureUnit aTextureUnit)
    : Texture1DFunction(aTextureUnit)
    , uFocalX(AppendTextureUnit("uFocalX", aTextureUnit).c_str())
    , u1MinusFx_2(AppendTextureUnit("u1MinusFx_2", aTextureUnit).c_str())
  {}

  virtual void WriteDeclarations(std::ostream& aOut) const
  {
    Texture1DFunction::WriteDeclarations(aOut);
    aOut << "uniform float " << uFocalX << ";" << endl;
    aOut << "uniform float " << u1MinusFx_2 << ";" << endl;
  }

  virtual void WriteColorFunction(std::ostream& aOut) const
  {
    aOut << "vec2 q = gl_TexCoord[" << mTextureUnit << "].st;" << endl;
    aOut << "float offset = q.x * " << uFocalX << " + sqrt(q.x * q.x + " << u1MinusFx_2 << " * q.y * q.y);" << endl;
    aOut << "return offset >= 0 ? texture1D(" << uTexture << ", offset) : 0;" << endl;
  }

  virtual void Initialize(GLuint aShaderProgram)
  {
    uFocalX.Initialize(aShaderProgram);
    u1MinusFx_2.Initialize(aShaderProgram);
  }

  virtual void ApplyFragmentUniforms(const PaintConfig& aConfig)
  {
    Texture1DFunction::ApplyFragmentUniforms(aConfig);
    uFocalX.Set(aConfig.uFocalX);
    u1MinusFx_2.Set(aConfig.u1MinuxFx_2);
  }

protected:
  UniformFloat uFocalX;
  UniformFloat u1MinusFx_2;
};

class FocalGradTouchingFunction : public Texture1DFunction {
public:
  FocalGradTouchingFunction(GL::TextureUnit aTextureUnit)
    : Texture1DFunction(aTextureUnit)
  {}

  virtual void WriteColorFunction(std::ostream& aOut) const
  {
    aOut << "vec2 p = gl_TexCoord[" << mTextureUnit << "].st;" << endl;
    aOut << "float offset = dot(q, q) / (-2 * q.x);" << endl;
    aOut << "return offset >= 0 ? texture1D(" << uTexture << ", offset) : 0;" << endl;
  }
};

class RadialGradInsideFunction : public Texture1DFunction {
public:
  RadialGradInsideFunction(GL::TextureUnit aTextureUnit, PaintConfig::PaintMode aPaintMode)
    : Texture1DFunction(aTextureUnit)
    , mPaintMode(aPaintMode)
    , uEndCenter(AppendTextureUnit("uEndCenter", aTextureUnit).c_str())
    , uA(AppendTextureUnit("uA", aTextureUnit).c_str())
    , uB(AppendTextureUnit("uB", aTextureUnit).c_str())
    , uC(AppendTextureUnit("uC", aTextureUnit).c_str())
  {}

  virtual void WriteDeclarations(std::ostream& aOut) const
  {
    Texture1DFunction::WriteDeclarations(aOut);
    aOut << "uniform vec2 " << uEndCenter << ";" << endl;
    aOut << "uniform float " << uA << ";" << endl;
    aOut << "uniform float " << uB << ";" << endl;
    aOut << "uniform float " << uC << ";" << endl;
  }

  virtual void WriteColorFunction(std::ostream& aOut) const
  {
    const char sign =
      mPaintMode == PaintConfig::MODE_RADIAL_GRAD_INSIDE_SUBTRACT_SQRT ? '-' : '+';
    aOut << "vec2 q = gl_TexCoord[" << mTextureUnit << "].st;" << endl;
    aOut << "float d = dot(" << uEndCenter << ", q) + " << uB << ";" << endl;
    aOut << "float offset = d " << sign << " sqrt(d * d - " << uA << " * dot(q, q) + " << uC << ");" << endl;
    aOut << "return texture1D(" << uTexture << ", offset);" << endl;
  }

  virtual void Initialize(GLuint aShaderProgram)
  {
    uEndCenter.Initialize(aShaderProgram);
    uA.Initialize(aShaderProgram);
    uC.Initialize(aShaderProgram);
    uB.Initialize(aShaderProgram);
  }

  virtual void ApplyFragmentUniforms(const PaintConfig& aConfig)
  {
    Texture1DFunction::ApplyFragmentUniforms(aConfig);
    uEndCenter.Set(aConfig.uEndCenter);
    uA.Set(aConfig.uA);
    uC.Set(aConfig.uB);
    uB.Set(aConfig.uC);
  }

protected:
  PaintConfig::PaintMode mPaintMode;
  UniformVec2 uEndCenter;
  UniformFloat uA;
  UniformFloat uB;
  UniformFloat uC;
};

class RadialGradOutsideFunction : public Texture1DFunction {
public:
  RadialGradOutsideFunction(GL::TextureUnit aTextureUnit, PaintConfig::PaintMode aPaintMode)
    : Texture1DFunction(aTextureUnit)
    , mPaintMode(aPaintMode)
    , uEndCenter(AppendTextureUnit("uEndCenter", aTextureUnit).c_str())
    , uA(AppendTextureUnit("uA", aTextureUnit).c_str())
    , uB(AppendTextureUnit("uB", aTextureUnit).c_str())
    , uC(AppendTextureUnit("uC", aTextureUnit).c_str())
    , uOffsetLimit(AppendTextureUnit("uOffsetLimit", aTextureUnit).c_str())
  {}

  virtual void WriteDeclarations(std::ostream& aOut) const
  {
    aOut << "uniform vec2 " << uEndCenter << ";" << endl;
    aOut << "uniform float " << uA << ";" << endl;
    aOut << "uniform float " << uB << ";" << endl;
    aOut << "uniform float " << uC << ";" << endl;
    aOut << "uniform float " << uOffsetLimit << ";" << endl;
  }

  virtual void WriteColorFunction(std::ostream& aOut) const
  {
    const char* op =
      mPaintMode == PaintConfig::MODE_RADIAL_GRAD_OUTSIDE_DISCARD_HIGH ? "<=" : ">=";
    aOut << "vec2 q = gl_TexCoord[" << mTextureUnit << "].st;" << endl;
    aOut << "float d = dot(" << uEndCenter << ", q) + " << uB << ";" << endl;
    aOut << "float offset = d + sqrt(d * d - " << uA << " * dot(q, q) + " << uC << ");" << endl;
    aOut << "return offset " << op << uOffsetLimit << " ? texture1D(" << uTexture << ", offset) : 0;" << endl;
  }

  virtual void Initialize(GLuint aShaderProgram)
  {
    uEndCenter.Initialize(aShaderProgram);
    uA.Initialize(aShaderProgram);
    uC.Initialize(aShaderProgram);
    uB.Initialize(aShaderProgram);
    uOffsetLimit.Initialize(aShaderProgram);
  }

  virtual void ApplyFragmentUniforms(const PaintConfig& aConfig)
  {
    Texture1DFunction::ApplyFragmentUniforms(aConfig);
    uEndCenter.Set(aConfig.uEndCenter);
    uA.Set(aConfig.uA);
    uC.Set(aConfig.uB);
    uB.Set(aConfig.uC);
    uOffsetLimit.Set(aConfig.uOffsetLimit);
  }

protected:
  PaintConfig::PaintMode mPaintMode;
  UniformVec2 uEndCenter;
  UniformFloat uA;
  UniformFloat uB;
  UniformFloat uC;
  UniformFloat uOffsetLimit;
};

TemporaryRef<PaintShader::ColorFunction>
PaintShader::ColorFunction::Create(PaintConfig::PaintMode aPaintMode,
                                   GL::TextureUnit aTextureUnit)
{
  switch (aPaintMode) {
    default:
    case PaintConfig::MODE_NONE:
      return new EmptyFunction();
    case PaintConfig::MODE_SOLID_COLOR:
      return new SolidColorFunction(aTextureUnit);
    case PaintConfig::MODE_TEXTURE_1D:
      return new Texture1DFunction(aTextureUnit);
    case PaintConfig::MODE_TEXTURE_2D:
      return new Texture2DFunction(aTextureUnit);
    case PaintConfig::MODE_TEXTURE_2D_CLAMPED:
      return new Texture2DClampedFunction(aTextureUnit);
    case PaintConfig::MODE_FOCAL_GRAD_CENTERED:
      return new FocalGradCenteredFunction(aTextureUnit);
    case PaintConfig::MODE_FOCAL_GRAD_INSIDE:
      return new FocalGradInsideFunction(aTextureUnit);
    case PaintConfig::MODE_FOCAL_GRAD_OUTSIDE:
      return new FocalGradOutsideFunction(aTextureUnit);
    case PaintConfig::MODE_FOCAL_GRAD_TOUCHING:
      return new FocalGradTouchingFunction(aTextureUnit);
    case PaintConfig::MODE_RADIAL_GRAD_INSIDE_ADD_SQRT:
    case PaintConfig::MODE_RADIAL_GRAD_INSIDE_SUBTRACT_SQRT:
      return new RadialGradInsideFunction(aTextureUnit, aPaintMode);
    case PaintConfig::MODE_RADIAL_GRAD_OUTSIDE_DISCARD_HIGH:
    case PaintConfig::MODE_RADIAL_GRAD_OUTSIDE_DISCARD_LOW:
      return new RadialGradOutsideFunction(aTextureUnit, aPaintMode);
  }
}


class AlphaPaintShader : public PaintShader {
public:
  AlphaPaintShader(PaintConfig::PaintMode aPaintMode, PaintConfig::PaintMode aMaskMode)
    : PaintShader(aPaintMode, aMaskMode)
    , uGlobalAlpha("uGlobalAlpha")
  {}

  virtual void ApplyFragmentUniforms(const Paint& aPaint)
  {
    PaintShader::ApplyFragmentUniforms(aPaint);
    uGlobalAlpha.Set(aPaint.mGlobalAlpha);
  }

protected:
  virtual void WriteDeclarations(std::ostream& aOut) const
  {
    PaintShader::WriteDeclarations(aOut);
    aOut << "uniform float " << uGlobalAlpha << ";" << endl;
  }

  virtual void WriteMainFunction(std::ostream& aOut) const
  {
    PaintShader::WriteMainFunction(aOut);
    aOut << "gl_FragColor *= " << uGlobalAlpha << ";" << endl;
  }

  virtual void Initialize()
  {
    PaintShader::Initialize();
    uGlobalAlpha.Initialize(*this);
  }

  UniformFloat uGlobalAlpha;
};

PaintShader::PaintShader(PaintConfig::PaintMode aPaintMode, PaintConfig::PaintMode aMaskMode)
  : mPaint(ColorFunction::Create(aPaintMode, PAINT_UNIT))
  , mMask(ColorFunction::Create(aMaskMode, MASK_UNIT))
{
}

PaintShader::~PaintShader()
{
}

void
PaintShader::WriteDeclarations(ostream& aOut) const
{
  mPaint->WriteDeclarations(aOut);
  mMask->WriteDeclarations(aOut);
}

void
PaintShader::WriteMainFunction(ostream& aOut) const
{
  aOut << "gl_FragColor = GetPaintColor();" << endl;
  if (!mMask->IsEmpty()) {
    aOut << "gl_FragColor *= GetMaskColor().a;" << endl;
  }
}

void
PaintShader::ApplyFragmentUniforms(const Paint& aPaint)
{
  MOZ_ASSERT(gl->IsCurrent());

  mPaint->ApplyFragmentUniforms(aPaint);
  mMask->ApplyFragmentUniforms(aPaint.mMask);
}

void
PaintShader::Initialize()
{
  ostringstream fragSource;
  WriteDeclarations(fragSource);
  fragSource << endl;

  fragSource << "vec4 GetPaintColor() {" << endl;
  mPaint->WriteColorFunction(fragSource);
  fragSource << "}" << endl;
  fragSource << endl;

  if (!mMask->IsEmpty()) {
    fragSource << "vec4 GetMaskColor() {" << endl;
    mMask->WriteColorFunction(fragSource);
    fragSource << "}" << endl;
    fragSource << endl;
  }

  fragSource << "void main(void) {" << endl;
  WriteMainFunction(fragSource);
  fragSource << "}" << endl;

  ShaderProgram::Initialize(fragSource.str().c_str());
  mPaint->Initialize(*this);
  mMask->Initialize(*this);
}

TemporaryRef<PaintShader>
PaintShader::Create(const Paint& aPaint)
{
  RefPtr<PaintShader> paintShader = (aPaint.mGlobalAlpha != 1)
    ? new AlphaPaintShader(aPaint.mPaintMode, aPaint.mMask.mPaintMode)
    : new PaintShader(aPaint.mPaintMode, aPaint.mMask.mPaintMode);

  paintShader->Initialize();

  return paintShader.forget();
}

}
}
}
