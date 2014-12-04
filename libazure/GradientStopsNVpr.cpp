/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GradientStopsNVpr.h"
#include <deque>
#include <vector>

static const int sMaxColorRampSize = 4096;
static const size_t sMaxRampTexturePoolSize = 32;

using namespace mozilla::gfx::nvpr;
using namespace std;

struct TextureColor {
  TextureColor() {}
  TextureColor(const mozilla::gfx::Color& color)
    : r(static_cast<GLubyte>(color.a * color.r * 255))
    , g(static_cast<GLubyte>(color.a * color.g * 255))
    , b(static_cast<GLubyte>(color.a * color.b * 255))
    , a(static_cast<GLubyte>(color.a * 255))
  {}
  void Lerp(TextureColor aColor1, TextureColor aColor2, uint8_t aWeight)
  {
    r = (aColor1.r * (256 - aWeight) + aColor2.r * aWeight) >> 8;
    g = (aColor1.g * (256 - aWeight) + aColor2.g * aWeight) >> 8;
    b = (aColor1.b * (256 - aWeight) + aColor2.b * aWeight) >> 8;
    a = (aColor1.a * (256 - aWeight) + aColor2.a * aWeight) >> 8;
  }
  void Average(TextureColor aColor1, TextureColor aColor2)
  {
    r = (aColor1.r + aColor2.r) >> 1;
    g = (aColor1.g + aColor2.g) >> 1;
    b = (aColor1.b + aColor2.b) >> 1;
    a = (aColor1.a + aColor2.a) >> 1;
  }
  GLubyte r, g, b, a;
};

struct ColorRampData : public mozilla::gfx::nvpr::UserData::Object {
  vector<TextureColor> mRampBuffer;
  size_t mNumLevels;
  deque<GLuint> mTexturePool;
};

namespace mozilla {
namespace gfx {

GradientStopsNVpr::GradientStopsNVpr(GradientStop* aRawStops, uint32_t aNumStops,
                                     ExtendMode aExtendMode)
  : mRampTextureId(0)
  , mInitialColor(1, 1, 1, 1)
  , mFinalColor(1, 1, 1, 1)
{
  if (!aRawStops || !aNumStops) {
    return;
  }

  if (aNumStops == 1) {
    mInitialColor = mFinalColor = aRawStops[0].color;
    return;
  }

  vector<GradientStop> sortedStops(aNumStops);
  memcpy(sortedStops.data(), aRawStops, aNumStops * sizeof(GradientStop));
  sort(sortedStops.begin(), sortedStops.end());

  mInitialColor = sortedStops.front().color;
  mFinalColor = sortedStops.back().color;

  gl->MakeCurrent();

  // Draw the color stops into a linear color ramp buffer.
  // TODO: Optimize this with SSE/NEON.
  ColorRampData& rampData =
    gl->GetUserObject<ColorRampData>(&nvpr::UserData::mColorRampData);
  vector<TextureColor>& ramp = rampData.mRampBuffer;

  if (ramp.empty()) {
    ramp.resize(min(sMaxColorRampSize, gl->MaxTextureSize()));
    rampData.mNumLevels = 0;
    for (size_t width = ramp.size(); width; width >>= 1) {
      rampData.mNumLevels++;
    }
  }

  TextureColor startColor(mInitialColor);
  MOZ_ASSERT(sortedStops[0].offset >= 0 && sortedStops[0].offset <= 1);
  size_t startIndex = sortedStops[0].offset * (ramp.size() - 1);
  for (size_t i = 0; i < startIndex; i++) {
    ramp[i] = startColor;
  }

  for (size_t i = 1; i < sortedStops.size(); i++) {
    MOZ_ASSERT(sortedStops[i].offset >= 0 && sortedStops[i].offset <= 1);
    const TextureColor endColor(sortedStops[i].color);
    const size_t endIndex = sortedStops[i].offset * (ramp.size() - 1);

    if (endIndex == startIndex) {
      startColor = endColor;
      continue;
    }

    const uint16_t weightStep = (1 << 16) / (endIndex - startIndex);
    uint16_t weight = 0;
    for (size_t i = startIndex; i < endIndex; i++) {
      ramp[i].Lerp(startColor, endColor, weight >> 8);
      weight += weightStep;
    }

    startColor = endColor;
    startIndex = endIndex;
  }

  const TextureColor endColor(mFinalColor);
  for (size_t i = startIndex; i < ramp.size(); i++) {
    ramp[i] = endColor;
  }

  // Create a texture from the color ramp buffer.
  if (!rampData.mTexturePool.empty()) {
    mRampTextureId = rampData.mTexturePool.front();
    rampData.mTexturePool.pop_front();
  } else {
    gl->GenTextures(1, &mRampTextureId);
    gl->TextureStorage1DEXT(mRampTextureId, GL_TEXTURE_1D,
                            rampData.mNumLevels, GL_RGBA8, ramp.size());
  }

  gl->TextureSubImage1DEXT(mRampTextureId, GL_TEXTURE_1D, 0, 0, ramp.size(),
                           GL_RGBA, GL_UNSIGNED_BYTE, ramp.data());

  size_t previousWidth = ramp.size();
  for (size_t level = 1; level < rampData.mNumLevels - 1; level++) {
    // Generate a mipmap image where the begin and end texels are the same
    // colors as the begin and end stops, to ensure proper clamping.
    const size_t width = previousWidth >> 1;
    for (size_t i = 1; i < width - 1; i++) {
      ramp[i].Average(ramp[2 * i], ramp[2 * i + 1]);
    }
    ramp[width - 1] = ramp[previousWidth - 1];

    gl->TextureSubImage1DEXT(mRampTextureId, GL_TEXTURE_1D, level, 0, width,
                             GL_RGBA, GL_UNSIGNED_BYTE, ramp.data());

    previousWidth = width;
  }

  // Configure texturing parameters.
  gl->TextureParameteriEXT(mRampTextureId, GL_TEXTURE_1D,
                           //GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                           GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  gl->TextureParameteriEXT(mRampTextureId, GL_TEXTURE_1D,
                           GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (aExtendMode == ExtendMode::CLAMP) {
    gl->TextureParameteriEXT(mRampTextureId, GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL,
                             rampData.mNumLevels - 2);
  } else {
    gl->TextureParameteriEXT(mRampTextureId, GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL,
                             rampData.mNumLevels - 1);
    ramp[0].Average(ramp[0], ramp[1]);
    gl->TextureSubImage1DEXT(mRampTextureId, GL_TEXTURE_1D,
                             rampData.mNumLevels - 1, 0, 1, GL_RGBA,
                             GL_UNSIGNED_BYTE, ramp.data());
  }

  GLenum wrapMode;
  switch (aExtendMode) {
    default:
      MOZ_ASSERT(!"Invalid gradient extend mode");
    case ExtendMode::CLAMP:
      wrapMode = GL_CLAMP_TO_EDGE;
      break;
    case ExtendMode::REPEAT:
      wrapMode = GL_REPEAT;
      break;
    case ExtendMode::REFLECT:
      wrapMode = GL_MIRRORED_REPEAT;
      break;
  }
  gl->TextureParameteriEXT(mRampTextureId, GL_TEXTURE_1D,
                           GL_TEXTURE_WRAP_S, wrapMode);
}

GradientStopsNVpr::~GradientStopsNVpr()
{
  ColorRampData& rampData =
    gl->GetUserObject<ColorRampData>(&nvpr::UserData::mColorRampData);

  if (rampData.mTexturePool.size() < sMaxRampTexturePoolSize) {
    rampData.mTexturePool.push_back(mRampTextureId);
    return;
  }

  gl->MakeCurrent();
  gl->DeleteTexture(mRampTextureId);
}

}
}
