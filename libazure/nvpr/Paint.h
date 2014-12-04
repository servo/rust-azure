/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_NVPR_PAINT_SHADER_H_
#define MOZILLA_GFX_NVPR_PAINT_SHADER_H_

#include "2D.h"
#include "ShaderProgram.h"

namespace mozilla {
namespace gfx {

class GradientStopsNVpr;
class SourceSurfaceNVpr;

namespace nvpr {

struct PaintConfig {
  enum PaintMode {
    MODE_NONE,
    MODE_SOLID_COLOR,
    MODE_TEXTURE_1D,
    MODE_TEXTURE_2D,
    MODE_TEXTURE_2D_CLAMPED,
    MODE_FOCAL_GRAD_CENTERED,
    MODE_FOCAL_GRAD_INSIDE,
    MODE_FOCAL_GRAD_OUTSIDE,
    MODE_FOCAL_GRAD_TOUCHING,
    MODE_RADIAL_GRAD_INSIDE_ADD_SQRT,
    MODE_RADIAL_GRAD_INSIDE_SUBTRACT_SQRT,
    MODE_RADIAL_GRAD_OUTSIDE_DISCARD_HIGH,
    MODE_RADIAL_GRAD_OUTSIDE_DISCARD_LOW,
    MODE_COUNT
  };

  PaintConfig();
  void SetToPattern(const Pattern& aPattern);
  void SetToColor(const Color& aColor);
  void SetToSurface(SourceSurfaceNVpr* aSurface, Filter aFilter = Filter::LINEAR);
  void SetToClampedSurface(SourceSurfaceNVpr* aSurface, Filter aFilter,
                           const Rect& aSamplingBounds);
  void SetToTexgenSurface(SourceSurfaceNVpr* aSurface, ExtendMode aExtendMode,
                          Filter aFilter, const Matrix& aMatrix);
  void SetToLinearGradient(GradientStopsNVpr* aStops, const Point& aBeginPoint,
                           const Point& aEndPoint, const Matrix& aMatrix);
  void SetToFocalGradient(GradientStopsNVpr* aStops, const Point& aFocalPoint,
                          const Point& aEndCenter, float aEndRadius,
                          const Matrix& aMatrix);
  void SetToRadialGradient(GradientStopsNVpr* aStops,
                           const Point& aBeginCenter, float aBeginRadius,
                           const Point& aEndCenter, float aEndRadius,
                           const Matrix& aMatrix);
  void SetTexGenCoefficients(const Matrix& aTransform);

  PaintMode mPaintMode;
  GLuint mTextureId;
  GL::TexGenComponents mTexGenComponents;
  GLfloat mTexGenCoefficients[6];
  union {
    struct {
      GLfloat uColor[4];
    };
    struct {
      GLfloat uClampRect[4];
    };
    struct {
      GLfloat uFocalX;
      GLfloat u1MinuxFx_2;
    };
    struct {
      GLfloat uEndCenter[2];
      GLfloat uA;
      GLfloat uB;
      GLfloat uC;
      GLfloat uOffsetLimit;
    };
  };
};

struct Paint : public PaintConfig {
  Paint() : mGlobalAlpha(1) {}
  PaintConfig mMask;
  GLfloat mGlobalAlpha;
};

class PaintShader : public ShaderProgram {
public:
  static const GL::TextureUnit PAINT_UNIT = GL::UNIT_0;
  static const GL::TextureUnit MASK_UNIT = GL::UNIT_1;

  class ColorFunction;

  static TemporaryRef<PaintShader> Create(const Paint& aPaint);
  virtual ~PaintShader();

  virtual void ApplyFragmentUniforms(const Paint& aPaint);

protected:
  PaintShader(PaintConfig::PaintMode aPaintMode, PaintConfig::PaintMode aMaskMode);
  virtual void WriteDeclarations(std::ostream& aOut) const;
  virtual void WriteMainFunction(std::ostream& aOut) const;
  virtual void Initialize();

  RefPtr<ColorFunction> mPaint;
  RefPtr<ColorFunction> mMask;
};

}
}
}

#endif /* MOZILLA_GFX_NVPR_PAINT_SHADER_H_ */
