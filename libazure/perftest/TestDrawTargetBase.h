/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include "2D.h"
#include "TestBase.h"

#define DT_WIDTH 1000
#define DT_HEIGHT 1000

typedef void(*FlushFunc)(void*);

/* This general DrawTarget test class can be reimplemented by a child class
 * with optional additional drawtarget-specific tests. And is intended to run
 * on a 1000x1000 32 BPP drawtarget.
 */
class TestDrawTargetBase : public TestBase
{
public:
  void FillRect50x50x500();
  void FillRect50x50x2000();
  void FillRect200x200x500();
  void FillRect200x200x2000();
  void FillRect800x800x2000();
  void FillRect50x50x500Add();
  void FillRect50x50x2000Add();
  void FillRect200x200x500Add();
  void FillRect200x200x2000Add();
  void CreateGradientStops();
  void CreateSourceSurfaceForData100x100();
  void CreateSourceSurfaceForData200x200();
  void CreateSourceSurfaceForData500x500();
  void FillRadialSimple();
  void FillRadialComplex();
  void FillRadialSimpleUncached();
  void FillRadialComplexUncached();
  void DrawTransparentSurfaceUnscaledAligned();
  void DrawTransparentSurfaceUnscaled();
  void DrawTransparentSurfaceScaled();
  void DrawOpaqueSurfaceUnscaledAligned();
  void DrawOpaqueSurfaceUnscaled();
  void DrawOpaqueSurfaceScaled();
  void StrokeRectThin();
  void StrokeRectThick();
  void StrokeCurveThin();
  void StrokeCurveThinUncached();
  void StrokeCurveThick();
  void MaskSurface100x100();
  void MaskSurface500x500();
  void DrawShadow10x10SmallRadius();
  void DrawShadow200x200SmallRadius();
  void DrawShadow10x10LargeRadius();
  void DrawShadow200x200LargeRadius();
  void CreateRandom200();
  void DrawTurbulence500x500x10();
  void DrawMorphologyFilter200x200x100Radius40();
  void Premultiply200x200x1000();
  void Unpremultiply200x200x1000();
  void ComponentTransfer200x200x1000();
  void ColorMatrix200x200x1000();
  void Composite200x200x1000();
  void CompositeA8Single200x200x1000();
  void Blend200x200x1000();
  void Blur500x500x50();
  void ArithmeticCombine200x200x100();

protected:
  FlushFunc mFlush;

  TestDrawTargetBase();

  void Flush() {
    if (mFlush) mFlush(this);
  }

  void FillSquare(int aSize, int aRepeat, mozilla::gfx::CompositionOp aOp = mozilla::gfx::CompositionOp::OP_OVER);
  mozilla::TemporaryRef<mozilla::gfx::SourceSurface> CreateSquareRandomSourceSurface(int aSize, mozilla::gfx::SurfaceFormat aFormat, bool aLeaveUninitialized = true);
  mozilla::TemporaryRef<mozilla::gfx::GradientStops> CreateSimpleGradientStops();

  mozilla::RefPtr<mozilla::gfx::DrawTarget> mDT;
  mozilla::RefPtr<mozilla::gfx::SourceSurface> mRandom200;
};
