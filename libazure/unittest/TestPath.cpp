/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "TestPath.h"
#include "PathHelpers.h"

using namespace mozilla::gfx;

TestPathBase::TestPathBase()
{
#define TEST_CLASS TestPathBase
  REGISTER_TEST(Line);
  REGISTER_TEST(PolyLine);
  REGISTER_TEST(SegmentEndVector);
  REGISTER_TEST(ZeroLengthPath);
  REGISTER_TEST(SimpleBezier);
  REGISTER_TEST(LinearBezier);
  REGISTER_TEST(LineAndBezier);
  REGISTER_TEST(SingleInflection);
  REGISTER_TEST(Cusp);
  REGISTER_TEST(DoubleInflection);
  REGISTER_TEST(EndsInEmptySegments);
  REGISTER_TEST(MultipleEmptySegments);
  REGISTER_TEST(MultipleMoves);
  REGISTER_TEST(ClosedPathEnding);
  REGISTER_TEST(Bug984796);
#undef TEST_CLASS
}

void
TestPathBase::Line()
{
  mBuilder = mDT->CreatePathBuilder();
  mBuilder->MoveTo(Point(0, 0));
  mBuilder->LineTo(Point(100, 0));
  mPath = mBuilder->Finish();

  VerifyComputeLength(100);
  VerifyComputePointAtLength(0, Point(0, 0), Point(1, 0));
  VerifyComputePointAtLength(50, Point(50, 0), Point(1, 0));
  VerifyComputePointAtLength(100, Point(100, 0), Point(1, 0));
}

void
TestPathBase::PolyLine()
{
  mBuilder = mDT->CreatePathBuilder();
  mBuilder->MoveTo(Point(0, 0));
  mBuilder->LineTo(Point(100, 0));
  mBuilder->LineTo(Point(100, 100));
  mPath = mBuilder->Finish();

  VerifyComputeLength(200);
  VerifyComputePointAtLength(0, Point(0, 0), Point(1, 0));
  VerifyComputePointAtLength(50, Point(50, 0), Point(1, 0));
  VerifyComputePointAtLength(100, Point(100, 0), Point(0, 1));
  VerifyComputePointAtLength(150, Point(100, 50), Point(0, 1));
  VerifyComputePointAtLength(200, Point(100, 100), Point(0, 1));
}

void
TestPathBase::SegmentEndVector()
{
  mBuilder = mDT->CreatePathBuilder();
  mBuilder->MoveTo(Point(0, 0));
  mBuilder->LineTo(Point(10, 0));
  mBuilder->LineTo(Point(0, 0));
  mPath = mBuilder->Finish();
 
  VerifyComputePointAtLength(10, Point(10, 0), Point(-1, 0));
}

void
TestPathBase::SimpleBezier()
{
  mBuilder = mDT->CreatePathBuilder();
  mBuilder->MoveTo(Point(200, 100));
  ArcToBezier<PathBuilder>(mBuilder, Point(100, 100), Size(100.f, 100.f), 0.f, Float(M_PI) / 2.0f, false);
  mPath = mBuilder->Finish();

  VerifyComputeLength(Float(M_PI) / 2.0f * 100);
  VerifyComputePointAtLength(0, Point(200, 100), Point(0, 1));
  VerifyComputePointAtLength(mPath->ComputeLength(), Point(100, 200), Point(-1, 0));
}

void
TestPathBase::LinearBezier()
{
  mBuilder = mDT->CreatePathBuilder();
  mBuilder->MoveTo(Point(0, 0));
  mBuilder->BezierTo(Point(50, 0), Point(100, 0), Point(150, 0));
  mPath = mBuilder->Finish();

  VerifyComputeLength(150);
  VerifyComputePointAtLength(50, Point(50, 0), Point(1, 0));
}

void TestPathBase::LineAndBezier()
{
  mBuilder = mDT->CreatePathBuilder();
  mBuilder->MoveTo(Point(100, 100));
  mBuilder->LineTo(Point(200, 100));
  ArcToBezier<PathBuilder>(mBuilder, Point(100, 100), Size(100.f, 100.f), 0.f, Float(M_PI) / 2.0f, false);
  mPath = mBuilder->Finish();

  VerifyComputeLength(Float(M_PI) / 2.0f * 100 + 100);
}

void
TestPathBase::SingleInflection()
{
  mBuilder = mDT->CreatePathBuilder();
  mBuilder->MoveTo(Point(20, 500));
  mBuilder->BezierTo(Point(20, -100), Point(520, 500), Point(520, 0));
  mPath = mBuilder->Finish();

  VerifyComputeLength(864.45f);
}

void
TestPathBase::Cusp()
{
  mBuilder = mDT->CreatePathBuilder();
  mBuilder->MoveTo(Point(20, 500));
  mBuilder->BezierTo(Point(520, 0), Point(20, 0), Point(520, 500));
  mPath = mBuilder->Finish();

  VerifyComputeLength(914.18f);
}

void
TestPathBase::DoubleInflection()
{
  mBuilder = mDT->CreatePathBuilder();
  mBuilder->MoveTo(Point(20, 500));
  mBuilder->BezierTo(Point(440, 150), Point(70, 150), Point(520, 500));
  mPath = mBuilder->Finish();

  VerifyComputeLength(729.48f);
}

void
TestPathBase::ZeroLengthPath()
{
  mBuilder = mDT->CreatePathBuilder();
  mBuilder->MoveTo(Point(0, 0));
  mPath = mBuilder->Finish();

  VerifyComputeLength(0);
  VerifyComputePointAtLength(0, Point(0, 0), Point(0, 0));
}

void
TestPathBase::EndsInEmptySegments()
{
  mBuilder = mDT->CreatePathBuilder();
  mBuilder->MoveTo(Point(0, 0));
  mBuilder->LineTo(Point(10, 0));
  mBuilder->LineTo(Point(10, 0));
  mBuilder->LineTo(Point(10, 0));
  mPath = mBuilder->Finish();

  VerifyComputeLength(10);
  VerifyComputePointAtLength(10, Point(10, 0), Point(1, 0));
}

void
TestPathBase::MultipleEmptySegments()
{
  mBuilder = mDT->CreatePathBuilder();
  mBuilder->MoveTo(Point(0, 0));
  mBuilder->LineTo(Point(10, 0));
  mBuilder->LineTo(Point(10, 0));
  mBuilder->LineTo(Point(10, 0));
  mBuilder->LineTo(Point(10, 10));
  mPath = mBuilder->Finish();

  VerifyComputeLength(20);
  VerifyComputePointAtLength(10, Point(10, 0), Point(0, 1));
}

void
TestPathBase::MultipleMoves()
{
  mBuilder = mDT->CreatePathBuilder();
  mBuilder->MoveTo(Point(0, 0));
  mBuilder->MoveTo(Point(0, 10));
  mPath = mBuilder->Finish();

  VerifyComputeLength(0);
  VerifyComputePointAtLength(0, Point(0, 10), Point(0, 1));
}

void
TestPathBase::ClosedPathEnding()
{
  mBuilder = mDT->CreatePathBuilder();
  mBuilder->MoveTo(Point(0, 0));
  mBuilder->LineTo(Point(10, 0));
  mBuilder->LineTo(Point(10, 10));
  mBuilder->LineTo(Point(0, 10));
  mBuilder->Close();
  mPath = mBuilder->Finish();

  VerifyComputeLength(40);
  VerifyComputePointAtLength(40, Point(0, 0), Point(0, -1));
}

void
TestPathBase::Bug984796()
{
  mBuilder = mDT->CreatePathBuilder();
  mBuilder->MoveTo(Point(124, 46));
  mBuilder->BezierTo(Point(100, 73), Point(40, 0), Point(0, 0));
  mPath = mBuilder->Finish();

  VerifyComputeLength(138.8f);
}

void
TestPathBase::VerifyComputeLength(Float aExpectedLength)
{
  VERIFYVALUEFUZZY(mPath->ComputeLength(), aExpectedLength, 0.1f);
}

void
TestPathBase::VerifyComputePointAtLength(Float aLength, const Point& aExpectedPoint, const Point& aExpectedTangent)
{
  Point tangent;
  Point point = mPath->ComputePointAtLength(aLength, &tangent);
  VERIFYVALUEFUZZY(point.x, aExpectedPoint.x, 0.05f);
  VERIFYVALUEFUZZY(point.y, aExpectedPoint.y, 0.05f);
  VERIFYVALUEFUZZY(tangent.x, aExpectedTangent.x, 0.05f);
  VERIFYVALUEFUZZY(tangent.y, aExpectedTangent.y, 0.05f);
}
