/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this
  * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
  
#pragma once
  
#include "2D.h"
#include "TestBase.h"
#include "TestHelpers.h"

/* This general DrawTarget test class can be reimplemented by a child class
  * with optional additional drawtarget-specific tests. And is intended to run
  * on a 500x500 32 BPP drawtarget.
  */
class TestDrawTargetBase : public TestBase
{
public:
  void Initialized();
  void FillCompletely();
  void FillRect();
  void StrokeRect();
  void StrokeLine();
  void Translate();
  void FillMultiRect();
  void FillMultiRectTransform1();
  void FillMultiRectTransform2();
  void FillMultiRectTransform3();
  void ClipRect();
  void ClipRectClear();
  void Clip();
  void FillTriangle();
  void StrokeTriangle();
  void DrawSurface();
  void FillWithSurface();
  void FillWithPartialLargeSurface();
  void FillWithScaledLargeSurface();
  void FillGradient();
  void FillRadialGradient();
  void FillWithSnapshot();
  void Mask();
  void CopySurface();
  void Shadow();
  void StreamToSink();
  void RoundtripThroughA8MakesColorsBlack();
  void ColorMatrix();
  void Blend();
  void Morphology();
  void Flood();
  void Tile();
  void TableTransfer();
  void DiscreteTransfer();
  void LinearTransfer();
  void GammaTransfer();
  void ConvolveMatrixNone();
  void ConvolveMatrixWrap();
  void ConvolveMatrixOffset();
  void OffsetFilter();
  void DisplacementMap();
  void Turbulence();
  void ArithmeticCombine();
  void Composite();
  void GaussianBlur();
  
protected:
  TestDrawTargetBase();
  
  void RefreshSnapshot();
  
  void VerifyAllPixels(const mozilla::gfx::Color &aColor,
                       uint8_t aTolerance = 0);
  void VerifyPixel(const mozilla::gfx::IntPoint &aPoint,
                   const mozilla::gfx::Color &aColor,
                   uint8_t aTolerance = 0);
  
  uint32_t BGRAPixelFromColor(const mozilla::gfx::Color &aColor);
  
  mozilla::RefPtr<mozilla::gfx::DrawTarget> mDT;
  mozilla::RefPtr<mozilla::gfx::DataSourceSurface> mDataSnapshot;
}; 

#ifdef WIN32
IMPLEMENT_DT_TESTS(D2D, DIRECT2D, TestDrawTarget);
#ifdef USE_D2D1_1
IMPLEMENT_DT_TESTS(D2D1, DIRECT2D1_1, TestDrawTarget);
#endif
#endif
#ifdef USE_SKIA
IMPLEMENT_DT_TESTS(SkiaSoftware, SKIA, TestDrawTarget);
#endif
#ifdef USE_CAIRO
IMPLEMENT_DT_TESTS(CairoImage, CAIRO, TestDrawTarget);
#endif

class TestDrawTargetCapture : public TestDrawTargetBase
{
public:
  TestDrawTargetCapture()
  {
    mozilla::RefPtr<mozilla::gfx::DrawTarget> dt =
      mozilla::gfx::Factory::CreateDrawTarget(mozilla::gfx::BackendType::DIRECT2D,
                                              mozilla::gfx::IntSize(1, 1),
                                              mozilla::gfx::SurfaceFormat::B8G8R8A8);

    mDT = dt->CreateCaptureDT(mozilla::gfx::IntSize(DT_WIDTH, DT_HEIGHT));
  }
};
