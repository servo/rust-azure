/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include "TestBase.h"
#include "TestHelpers.h"
#include "2D.h"

class TestPathBase : public TestBase
{
public:
  TestPathBase();

  void Line();
  void PolyLine();
  void SegmentEndVector();
  void ZeroLengthPath();
  void LinearBezier();
  void SimpleBezier();
  void LineAndBezier();
  void SingleInflection();
  void Cusp();
  void DoubleInflection();
  void EndsInEmptySegments();
  void MultipleEmptySegments();
  void MultipleMoves();
  void ClosedPathEnding();
  void Bug984796();

protected:
  mozilla::RefPtr<mozilla::gfx::DrawTarget> mDT;
  mozilla::RefPtr<mozilla::gfx::PathBuilder> mBuilder;
  mozilla::RefPtr<mozilla::gfx::Path> mPath;

private:
  void VerifyComputeLength(mozilla::gfx::Float aLength);
  void VerifyComputePointAtLength(mozilla::gfx::Float aLength,
                                  const mozilla::gfx::Point &aPoint,
                                  const mozilla::gfx::Point &aTangent);
};

#ifdef WIN32
IMPLEMENT_DT_TESTS(D2D, DIRECT2D, TestPath);
#endif
#ifdef USE_SKIA
IMPLEMENT_DT_TESTS(Skia, SKIA, TestPath);
#endif
#ifdef USE_CAIRO
IMPLEMENT_DT_TESTS(Cairo, CAIRO, TestPath);
#endif
