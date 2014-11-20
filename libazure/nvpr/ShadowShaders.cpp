/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ShadowShaders.h"
#include "ShaderProgram.h"
#include "SourceSurfaceNVpr.h"
#include <iostream>
#include <sstream>

using namespace std;

namespace mozilla {
namespace gfx {
namespace nvpr {

static const char* HorizontalConvolutionShaderSrc = "\
uniform float uWeights[1 + RADIUS];                                                       \n\
uniform float uOffsets[RADIUS];                                                           \n\
uniform float uLod;                                                                       \n\
uniform sampler2D uImage;                                                                 \n\
                                                                                          \n\
void main()                                                                               \n\
{                                                                                         \n\
  float convolution = uWeights[0] * textureLod(uImage, gl_TexCoord[0], uLod).a;           \n\
  for (int i = 1; i <= RADIUS; i++) {                                                     \n\
    vec2 offset = vec2(uOffsets[i - 1], 0);                                               \n\
    convolution += uWeights[i] * (textureLod(uImage, gl_TexCoord[0] + offset, uLod).a     \n\
                                  + textureLod(uImage, gl_TexCoord[0] - offset, uLod).a); \n\
  }                                                                                       \n\
  gl_FragColor[CHANNEL] = convolution;                                                    \n\
}                                                                                         \n\                                                                                  \n\
";

class ShadowShaders::HorizontalConvolutionShader : public ShaderProgram {
public:
  static TemporaryRef<HorizontalConvolutionShader>
  Create(ConvolutionChannel aConvolutionChannel, size_t aRadius)
  {
    MOZ_ASSERT(gl->IsCurrent());

    const char channel = aConvolutionChannel == RED ? 'r' : 'a';
    RefPtr<HorizontalConvolutionShader> shader = new HorizontalConvolutionShader();

    ostringstream fragSrc;
    fragSrc << "#define RADIUS " << aRadius << endl;
    fragSrc << "#define CHANNEL " << (aConvolutionChannel == RED ? '0' : '3') << endl;
    fragSrc << HorizontalConvolutionShaderSrc;
    shader->Initialize(fragSrc.str().c_str());

    return shader.forget();
  }

  UniformFloatArray uWeights;
  UniformFloatArray uOffsets;
  UniformFloat uLod;
  UniformSampler uImage;

private:
  HorizontalConvolutionShader()
    : uWeights("uWeights")
    , uOffsets("uOffsets")
    , uLod("uLod")
    , uImage("uImage", GL::UNIT_0)
  {}

  void Initialize(const GLchar* aFragmentSource)
  {
    ShaderProgram::Initialize(aFragmentSource);
    uWeights.Initialize(*this);
    uOffsets.Initialize(*this);
    uLod.Initialize(*this);
    uImage.Initialize(*this);
  }
};

static const char* ShadowShaderSrc = "\
uniform float uWeights[1 + RADIUS];                                                    \n\
uniform float uOffsets[RADIUS];                                                        \n\
uniform vec4 uClampRect;                                                               \n\
uniform vec4 uShadowColor;                                                             \n\
uniform sampler2D uHorizontalConvolution;                                              \n\
                                                                                       \n\
float getSample(float offsetT)                                                         \n\
{                                                                                      \n\
  vec2 coords = vec2(gl_TexCoord[0].s, gl_TexCoord[0].t + offsetT);                    \n\
  coords = clamp(coords, uClampRect.xy, uClampRect.zw);                                \n\
  return texture2D(uHorizontalConvolution, coords)[CHANNEL];                           \n\
}                                                                                      \n\
                                                                                       \n\
void main()                                                                            \n\
{                                                                                      \n\
  float alpha = uWeights[0] * getSample(0);                                            \n\
  for (int i = 1; i <= RADIUS; i++) {                                                  \n\
    alpha += uWeights[i] * (getSample(uOffsets[i - 1]) + getSample(-uOffsets[i - 1])); \n\
  }                                                                                    \n\
  gl_FragColor = alpha * uShadowColor;                                                 \n\
}                                                                                      \n\
";

class ShadowShaders::ShadowShader : public ShaderProgram {
public:
  static TemporaryRef<ShadowShader>
  Create(ConvolutionChannel aConvolutionChannel, size_t aRadius)
  {
    MOZ_ASSERT(gl->IsCurrent());

    RefPtr<ShadowShader> shader = new ShadowShader();

    ostringstream fragSrc;
    fragSrc << "#define RADIUS " << aRadius << endl;
    fragSrc << "#define CHANNEL " << (aConvolutionChannel == RED ? '0' : '3') << endl;
    fragSrc << ShadowShaderSrc;
    shader->Initialize(fragSrc.str().c_str());

    return shader.forget();
  }

  UniformVec4 uClampRect;
  UniformFloatArray uWeights;
  UniformFloatArray uOffsets;
  UniformVec4 uShadowColor;
  UniformSampler uHorizontalConvolution;

private:
  ShadowShader()
    : uClampRect("uClampRect")
    , uWeights("uWeights")
    , uOffsets("uOffsets")
    , uShadowColor("uShadowColor")
    , uHorizontalConvolution("uHorizontalConvolution", GL::UNIT_0)
  {}

  void Initialize(const GLchar* aFragmentSource)
  {
    ShaderProgram::Initialize(aFragmentSource);
    uClampRect.Initialize(*this);
    uWeights.Initialize(*this);
    uOffsets.Initialize(*this);
    uShadowColor.Initialize(*this);
    uHorizontalConvolution.Initialize(*this);
  }
};

ShadowShaders::ShadowShaders()
  : mSigma(0)
  , mRadius(0)
  , mScale(1)
  , mFilteredWeightsId(0)
{
  mWeights[0] = 1;
}

ShadowShaders::~ShadowShaders()
{
}

void
ShadowShaders::ConfigureShaders(const IntSize& aFramebufferSize,
                                const Rect& aShadowRect,
                                const Color& aShadowColor,
                                Float aSigma,
                                ConvolutionChannel aConvolutionChannel,
                                Rect* aHorizontalConvolutionRect,
                                GLuint* aHorizontalConvolutionShader,
                                GLuint* aShadowShader)
{
  MOZ_ASSERT(gl->IsCurrent());
  MOZ_ASSERT(aSigma > 0);

  if (mSigma != aSigma) {
    // Compute the right side of a 1D Gaussian blur kernel for aSigma.
    size_t radius = 2 * ceil(1.5f * aSigma);
    if (radius > kMaxRadius) {
      mRadius = kMaxRadius;
      mScale = static_cast<float>(radius) / kMaxRadius;
    } else {
      mRadius = radius;
      mScale = 1;
    }

    static const float oneOverSqrt2Pi = 0.39894228f;
    const float oneOverSigma = 1 / aSigma;
    const float a = oneOverSqrt2Pi * oneOverSigma;
    const float b = -2 * oneOverSigma * oneOverSigma * mScale * mScale;

    mWeights[0] = a;
    float weightSum = mWeights[0];
    for (size_t x = 1; x <= mRadius; x++) {
      mWeights[x] = a * exp(x * x * b);
      weightSum += 2 * mWeights[x];
    }

    const float weightAdjust = 1 / weightSum;
    for (size_t x = 0; x <= mRadius; x++) {
      mWeights[x] *= weightAdjust;
    }

    // The "filtered weights" and "offsets" are used by the shaders to take
    // advantage of texture filtering and perform the convolution in N/2
    // lookups.
    mFilteredWeights[0] = mWeights[0];
    for (size_t x = 1; x <= mRadius / 2; x++) {
      mFilteredWeights[x] = mWeights[2 * x - 1] + mWeights[2 * x];
      mOffsets[x - 1] = 2 * x - 1 + mWeights[2 * x] / mFilteredWeights[x];
    }
    mFilteredWeightsId = gl->GetUniqueId();

    mSigma = aSigma;
  }

  *aHorizontalConvolutionRect = aShadowRect;
  aHorizontalConvolutionRect->ScaleInverse(mScale);

  const size_t filteredRadius = mRadius / 2;
  float adjustedOffsets[1 + kMaxRadius / 2];

  if (aHorizontalConvolutionRect && aHorizontalConvolutionShader) {
    RefPtr<HorizontalConvolutionShader>& shader =
      mHorizontalConvolutionShaders[filteredRadius][aConvolutionChannel];
    if (!shader) {
      shader = HorizontalConvolutionShader::Create(aConvolutionChannel,
                                                   filteredRadius);
    }

    const GLfloat sampleWidth = mScale / aShadowRect.width;
    for (size_t i = 0; i < filteredRadius; i++) {
      adjustedOffsets[i] = sampleWidth * mOffsets[i];
    }

    shader->uWeights.Set(mFilteredWeights, 1 + filteredRadius,
                         mFilteredWeightsId);
    shader->uOffsets.Set(adjustedOffsets, filteredRadius);
    shader->uLod.Set(2 * (log(mScale) / log(2))); // Double to prefer blurry over blocky.

    *aHorizontalConvolutionShader = *shader;
  }

  if (aShadowShader) {
    RefPtr<ShadowShader>& shader =
      mShadowShaders[filteredRadius][aConvolutionChannel];
    if (!shader) {
      shader = ShadowShader::Create(aConvolutionChannel, filteredRadius);
    }

    const Size inverseFramebufferSize(1.0f / aFramebufferSize.width,
                                      1.0f / aFramebufferSize.height);

    Rect clampRect(*aHorizontalConvolutionRect);
    clampRect.Deflate(1); // Deflate by 1 instead of .5 because the convolution
                          // rect may not lie on pixel boundaries.
    clampRect.Scale(inverseFramebufferSize.width, inverseFramebufferSize.height);

    for (size_t i = 0; i < filteredRadius; i++) {
      adjustedOffsets[i] = mOffsets[i] * inverseFramebufferSize.height;
    }

    shader->uClampRect.Set(clampRect.TopLeft(), clampRect.BottomRight());
    shader->uWeights.Set(mFilteredWeights, 1 + filteredRadius,
                         mFilteredWeightsId);
    shader->uOffsets.Set(adjustedOffsets, 1 + filteredRadius);
    shader->uShadowColor.Set(aShadowColor.a * aShadowColor.r,
                             aShadowColor.a * aShadowColor.g,
                             aShadowColor.a * aShadowColor.b,
                             aShadowColor.a);

    *aShadowShader = *shader;
  }
}

}
}
}
