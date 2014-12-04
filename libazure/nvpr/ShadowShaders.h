/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_NVPR_SHADOWSHADERS_H_
#define MOZILLA_GFX_NVPR_SHADOWSHADERS_H_

#include "2D.h"
#include "GL.h"
#include <vector>

namespace mozilla {
namespace gfx {

class SourceSurfaceNVpr;

namespace nvpr {

class ShadowShaders : public nvpr::UserData::Object {
  class HorizontalConvolutionShader;
  class ShadowShader;

  static const size_t kMaxRadius = 24;

public:
  ShadowShaders();
  ~ShadowShaders();

  enum ConvolutionChannel { RED, ALPHA, CONVOLUTION_CHANNEL_COUNT };
  void ConfigureShaders(const IntSize& aFramebufferSize,
                        const Rect& aShadowRect, const Color& aShadowColor,
                        Float aSigma, ConvolutionChannel aConvolutionChannel,
                        Rect* aHorizontalConvolutionRect,
                        GLuint* aHorizontalConvolutionShader,
                        GLuint* aShadowShader);

private:
  Float mSigma;
  size_t mRadius;
  Float mScale;
  size_t mMaxRadius;
  GLfloat mWeights[1 + kMaxRadius];
  GLfloat mFilteredWeights[1 + kMaxRadius / 2];
  GLfloat mOffsets[kMaxRadius / 2];
  UniqueId mFilteredWeightsId;
  RefPtr<HorizontalConvolutionShader>
    mHorizontalConvolutionShaders[1 + kMaxRadius / 2][CONVOLUTION_CHANNEL_COUNT];
  RefPtr<ShadowShader>
    mShadowShaders[1 + kMaxRadius / 2][CONVOLUTION_CHANNEL_COUNT];
};

}
}
}

#endif /* MOZILLA_GFX_NVPR_SHADOWSHADERS_H_ */
