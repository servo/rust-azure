/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "TestDrawTargetBase.h"
#include <sstream>
#include "Tools.h"
#include "Filters.h"

using namespace mozilla;
using namespace mozilla::gfx;
using namespace std;

TestDrawTargetBase::TestDrawTargetBase()
  : mFlush(nullptr)
{
  REGISTER_TEST(TestDrawTargetBase, FillRect50x50x500);
  REGISTER_TEST(TestDrawTargetBase, FillRect200x200x500);
  REGISTER_TEST(TestDrawTargetBase, FillRect50x50x2000);
  REGISTER_TEST(TestDrawTargetBase, FillRect200x200x2000);
  REGISTER_TEST(TestDrawTargetBase, FillRect800x800x2000);
  REGISTER_TEST(TestDrawTargetBase, FillRect50x50x500Add);
  REGISTER_TEST(TestDrawTargetBase, FillRect200x200x500Add);
  REGISTER_TEST(TestDrawTargetBase, FillRect50x50x2000Add);
  REGISTER_TEST(TestDrawTargetBase, FillRect200x200x2000Add);
  REGISTER_TEST(TestDrawTargetBase, CreateGradientStops);
  REGISTER_TEST(TestDrawTargetBase, CreateSourceSurfaceForData100x100);
  REGISTER_TEST(TestDrawTargetBase, CreateSourceSurfaceForData200x200);
  REGISTER_TEST(TestDrawTargetBase, CreateSourceSurfaceForData500x500);
  REGISTER_TEST(TestDrawTargetBase, FillRadialSimple);
  REGISTER_TEST(TestDrawTargetBase, FillRadialComplex);
  REGISTER_TEST(TestDrawTargetBase, FillRadialSimpleUncached);
  REGISTER_TEST(TestDrawTargetBase, FillRadialComplexUncached);
  REGISTER_TEST(TestDrawTargetBase, DrawTransparentSurfaceUnscaledAligned);
  REGISTER_TEST(TestDrawTargetBase, DrawTransparentSurfaceUnscaled);
  REGISTER_TEST(TestDrawTargetBase, DrawTransparentSurfaceScaled);
  REGISTER_TEST(TestDrawTargetBase, DrawOpaqueSurfaceUnscaledAligned);
  REGISTER_TEST(TestDrawTargetBase, DrawOpaqueSurfaceUnscaled);
  REGISTER_TEST(TestDrawTargetBase, DrawOpaqueSurfaceScaled);
  REGISTER_TEST(TestDrawTargetBase, StrokeRectThin);
  REGISTER_TEST(TestDrawTargetBase, StrokeRectThick);
  REGISTER_TEST(TestDrawTargetBase, StrokeCurveThin);
  REGISTER_TEST(TestDrawTargetBase, StrokeCurveThinUncached);
  REGISTER_TEST(TestDrawTargetBase, StrokeCurveThick);
  REGISTER_TEST(TestDrawTargetBase, MaskSurface100x100);
  REGISTER_TEST(TestDrawTargetBase, MaskSurface500x500);
  REGISTER_TEST(TestDrawTargetBase, DrawShadow10x10SmallRadius);
  REGISTER_TEST(TestDrawTargetBase, DrawShadow200x200SmallRadius);
  REGISTER_TEST(TestDrawTargetBase, DrawShadow10x10LargeRadius);
  REGISTER_TEST(TestDrawTargetBase, DrawShadow200x200LargeRadius);
  REGISTER_TEST(TestDrawTargetBase, CreateRandom200);
  REGISTER_TEST(TestDrawTargetBase, DrawTurbulence500x500x10);
  REGISTER_TEST(TestDrawTargetBase, DrawMorphologyFilter200x200x100Radius40);
  REGISTER_TEST(TestDrawTargetBase, Premultiply200x200x1000);
  REGISTER_TEST(TestDrawTargetBase, Unpremultiply200x200x1000);
  REGISTER_TEST(TestDrawTargetBase, ComponentTransfer200x200x1000);
  REGISTER_TEST(TestDrawTargetBase, ColorMatrix200x200x1000);
  REGISTER_TEST(TestDrawTargetBase, Composite200x200x1000);
  REGISTER_TEST(TestDrawTargetBase, CompositeA8Single200x200x1000);
  REGISTER_TEST(TestDrawTargetBase, Blend200x200x1000);
  REGISTER_TEST(TestDrawTargetBase, Blur500x500x50);
  REGISTER_TEST(TestDrawTargetBase, ArithmeticCombine200x200x100);

  mGroup = GROUP_DRAWTARGETS;
}

void
TestDrawTargetBase::FillSquare(int aSize, int aRepeat, CompositionOp aOp)
{
  for (int i = 0; i < aRepeat; i++) {
    mDT->FillRect(Rect(i / 6, i / 4, aSize, aSize), ColorPattern(Color(1.0f, 0, 0, 1.0f)), DrawOptions(0.5f, aOp));
  }
  Flush();
}

void
TestDrawTargetBase::FillRect50x50x500()
{
  FillSquare(50, 500);
}

void
TestDrawTargetBase::FillRect200x200x500()
{
  FillSquare(200, 500);
}

void
TestDrawTargetBase::FillRect50x50x2000()
{
  FillSquare(50, 2000);
}

void
TestDrawTargetBase::FillRect200x200x2000()
{
  FillSquare(200, 2000);
}

void
TestDrawTargetBase::FillRect800x800x2000()
{
  FillSquare(800, 2000);
}

void
TestDrawTargetBase::FillRect50x50x500Add()
{
  FillSquare(50, 500, CompositionOp::OP_ADD);
}

void
TestDrawTargetBase::FillRect200x200x500Add()
{
  FillSquare(200, 500, CompositionOp::OP_ADD);
}

void
TestDrawTargetBase::FillRect50x50x2000Add()
{
  FillSquare(50, 2000, CompositionOp::OP_ADD);
}

void
TestDrawTargetBase::FillRect200x200x2000Add()
{
  FillSquare(200, 2000, CompositionOp::OP_ADD);
}

void
TestDrawTargetBase::CreateGradientStops()
{
  GradientStop stops[2];
  stops[0].color = Color(1.0f, 0, 0, 1.0f);
  stops[0].offset = 0;
  stops[1].color = Color(0, 1.0f, 0, 1.0f);
  stops[1].offset = 1.0f;

  for (int i = 0; i < 500; i++) {
    RefPtr<GradientStops> dtStops = mDT->CreateGradientStops(stops, 2);
  }
}

void
TestDrawTargetBase::CreateSourceSurfaceForData100x100()
{
  unsigned char *surfData = new unsigned char[100 * 100 * 4];

  for (int i = 0; i < 200; i++) {
    RefPtr<SourceSurface> surf = mDT->CreateSourceSurfaceFromData(surfData, IntSize(100, 100), 400, SurfaceFormat::B8G8R8A8);
  }

  delete [] surfData;
}

void
TestDrawTargetBase::CreateSourceSurfaceForData200x200()
{
  unsigned char *surfData = new unsigned char[200 * 200 * 4];

  for (int i = 0; i < 200; i++) {
    RefPtr<SourceSurface> surf = mDT->CreateSourceSurfaceFromData(surfData, IntSize(200, 200), 800, SurfaceFormat::B8G8R8A8);
  }

  delete [] surfData;
}

void
TestDrawTargetBase::CreateSourceSurfaceForData500x500()
{
  unsigned char *surfData = new unsigned char[500 * 500 * 4];

  for (int i = 0; i < 200; i++) {
    RefPtr<SourceSurface> surf = mDT->CreateSourceSurfaceFromData(surfData, IntSize(500, 500), 2000, SurfaceFormat::B8G8R8A8);
  }

  delete [] surfData;
}

void
TestDrawTargetBase::FillRadialSimple()
{
  RefPtr<GradientStops> stops = CreateSimpleGradientStops();
  for (int i = 0; i < 200; i++) {
    mDT->FillRect(Rect(i / 6, i / 4, 500, 500), RadialGradientPattern(Point(250, 250), Point(250, 250), 0, 500, stops));
  }
  Flush();
}

void
TestDrawTargetBase::FillRadialComplex()
{
  RefPtr<GradientStops> stops = CreateSimpleGradientStops();
  for (int i = 0; i < 200; i++) {
    mDT->FillRect(Rect(i / 6, i / 4, 500, 500), RadialGradientPattern(Point(250, 250), Point(300, 300), 40, 500, stops));
  }
  Flush();
}

void
TestDrawTargetBase::FillRadialSimpleUncached()
{
  for (int i = 0; i < 200; i++) {
    RefPtr<GradientStops> stops = CreateSimpleGradientStops();
    mDT->FillRect(Rect(float(i) / 6, float(i) / 4, 500, 500), RadialGradientPattern(Point(250, 250), Point(250, 250), 0, 500, stops));
  }
  Flush();
}

void
TestDrawTargetBase::FillRadialComplexUncached()
{
  for (int i = 0; i < 200; i++) {
    RefPtr<GradientStops> stops = CreateSimpleGradientStops();
    mDT->FillRect(Rect(float(i) / 6, float(i) / 4, 500, 500), RadialGradientPattern(Point(250, 250), Point(300, 300), 40, 500, stops));
  }
  Flush();
}

void
TestDrawTargetBase::DrawTransparentSurfaceUnscaledAligned()
{
  RefPtr<SourceSurface> surf = CreateSquareRandomSourceSurface(400, SurfaceFormat::B8G8R8A8);
  for (int i = 0; i < 200; i++) {
    mDT->DrawSurface(surf, Rect(i, i, 400, 400), Rect(0, 0, 400, 400));
  }
  Flush();
}

void
TestDrawTargetBase::DrawTransparentSurfaceUnscaled()
{
  RefPtr<SourceSurface> surf = CreateSquareRandomSourceSurface(400, SurfaceFormat::B8G8R8A8);
  for (int i = 0; i < 200; i++) {
    mDT->DrawSurface(surf, Rect(float(i) / 6, float(i) / 4, 400, 400), Rect(0, 0, 400, 400));
  }
  Flush();
}

void
TestDrawTargetBase::DrawTransparentSurfaceScaled()
{
  RefPtr<SourceSurface> surf = CreateSquareRandomSourceSurface(400, SurfaceFormat::B8G8R8A8);
  for (int i = 0; i < 200; i++) {
    mDT->DrawSurface(surf, Rect(float(i) / 6, float(i) / 4, 500, 500), Rect(0, 0, 400, 400));
  }
  Flush();
}

void
TestDrawTargetBase::DrawOpaqueSurfaceUnscaledAligned()
{
  RefPtr<SourceSurface> surf = CreateSquareRandomSourceSurface(400, SurfaceFormat::B8G8R8X8);
  for (int i = 0; i < 200; i++) {
    mDT->DrawSurface(surf, Rect(i, i, 400, 400), Rect(0, 0, 400, 400));
  }
  Flush();
}

void
TestDrawTargetBase::DrawOpaqueSurfaceUnscaled()
{
  RefPtr<SourceSurface> surf = CreateSquareRandomSourceSurface(400, SurfaceFormat::B8G8R8X8);
  for (int i = 0; i < 200; i++) {
    mDT->DrawSurface(surf, Rect(float(i) / 6, float(i) / 4, 400, 400), Rect(0, 0, 400, 400));
  }
  Flush();
}

void
TestDrawTargetBase::DrawOpaqueSurfaceScaled()
{
  RefPtr<SourceSurface> surf = CreateSquareRandomSourceSurface(400, SurfaceFormat::B8G8R8X8);
  for (int i = 0; i < 200; i++) {
    mDT->DrawSurface(surf, Rect(float(i) / 6, float(i) / 4, 500, 500), Rect(0, 0, 400, 400));
  }
  Flush();
}

void
TestDrawTargetBase::StrokeRectThin()
{
  for (int i = 0; i < 500; i++) {
    mDT->StrokeRect(Rect(30, 30, 200, 200), ColorPattern(Color(0, 0, 0, 1)), StrokeOptions(1.0f));
  }
  Flush();
}

void
TestDrawTargetBase::StrokeRectThick()
{
  for (int i = 0; i < 500; i++) {
    mDT->StrokeRect(Rect(30, 30, 200, 200), ColorPattern(Color(0, 0, 0, 1)), StrokeOptions(30.0f));
  }
  Flush();
}

void
TestDrawTargetBase::StrokeCurveThin()
{
  RefPtr<PathBuilder> builder = mDT->CreatePathBuilder();
  builder->MoveTo(Point(30, 30));
  builder->BezierTo(Point(600, 50), Point(-100, 400), Point(700, 700));
  RefPtr<Path> path = builder->Finish();
  for (int i = 0; i < 500; i++) {
    mDT->Stroke(path, ColorPattern(Color(0, 0, 0, 1)), StrokeOptions(1.0f));
  }
  Flush();
}  

void
TestDrawTargetBase::StrokeCurveThinUncached()
{
  for (int i = 0; i < 500; i++) {
    RefPtr<PathBuilder> builder = mDT->CreatePathBuilder();
    builder->MoveTo(Point(30, 30));
    builder->BezierTo(Point(600, 50), Point(-100, 400), Point(700, 700));
    RefPtr<Path> path = builder->Finish();
    mDT->Stroke(path, ColorPattern(Color(0, 0, 0, 1)), StrokeOptions(1.0f));
  }
  Flush();
}  

void
TestDrawTargetBase::StrokeCurveThick()
{
  RefPtr<PathBuilder> builder = mDT->CreatePathBuilder();
  builder->MoveTo(Point(30, 30));
  builder->BezierTo(Point(600, 50), Point(-100, 400), Point(700, 700));
  RefPtr<Path> path = builder->Finish();
  for (int i = 0; i < 500; i++) {
    mDT->Stroke(path, ColorPattern(Color(0, 0, 0, 1)), StrokeOptions(30.0f));
  }
  Flush();
}

void
TestDrawTargetBase::MaskSurface100x100()
{
  RefPtr<SourceSurface> surf = CreateSquareRandomSourceSurface(100, SurfaceFormat::B8G8R8A8);
  RefPtr<SourceSurface> mask = CreateSquareRandomSourceSurface(100, SurfaceFormat::A8);
  for (int i = 0; i < 200; i++) {
    mDT->MaskSurface(SurfacePattern(surf, ExtendMode::CLAMP), mask, Point());
  }
  Flush();
}

void
TestDrawTargetBase::MaskSurface500x500()
{
  RefPtr<SourceSurface> surf = CreateSquareRandomSourceSurface(500, SurfaceFormat::B8G8R8A8);
  RefPtr<SourceSurface> mask = CreateSquareRandomSourceSurface(500, SurfaceFormat::A8);
  for (int i = 0; i < 200; i++) {
    mDT->MaskSurface(SurfacePattern(surf, ExtendMode::CLAMP), mask, Point());
  }
  Flush();
}

void
TestDrawTargetBase::DrawShadow10x10SmallRadius()
{
  RefPtr<SourceSurface> surf = CreateSquareRandomSourceSurface(10, SurfaceFormat::B8G8R8A8);
  RefPtr<DrawTarget> dt = mDT->CreateShadowDrawTarget(IntSize(10, 10), SurfaceFormat::B8G8R8A8, 3.0f);
  dt->DrawSurface(surf, Rect(0, 0, 10, 10), Rect(0, 0, 10, 10));
  surf = dt->Snapshot();
  for (int i = 0; i < 200; i++) {
    mDT->DrawSurfaceWithShadow(surf, Point(100, 100), Color(0, 0, 0, 1.0f), Point(), 3.0f, CompositionOp::OP_OVER);
  }
  Flush();
}

void
TestDrawTargetBase::DrawShadow200x200SmallRadius()
{
  RefPtr<SourceSurface> surf = CreateSquareRandomSourceSurface(200, SurfaceFormat::B8G8R8A8);
  RefPtr<DrawTarget> dt = mDT->CreateShadowDrawTarget(IntSize(200, 200), SurfaceFormat::B8G8R8A8, 3.0f);
  dt->DrawSurface(surf, Rect(0, 0, 200, 200), Rect(0, 0, 200, 200));
  surf = dt->Snapshot();
  for (int i = 0; i < 200; i++) {
    mDT->DrawSurfaceWithShadow(surf, Point(100, 100), Color(0, 0, 0, 1.0f), Point(), 3.0f, CompositionOp::OP_OVER);
  }
  Flush();
}

void
TestDrawTargetBase::DrawShadow10x10LargeRadius()
{
  RefPtr<SourceSurface> surf = CreateSquareRandomSourceSurface(10, SurfaceFormat::B8G8R8A8);
  RefPtr<DrawTarget> dt = mDT->CreateShadowDrawTarget(IntSize(10, 10), SurfaceFormat::B8G8R8A8, 20.0f);
  dt->DrawSurface(surf, Rect(0, 0, 10, 10), Rect(0, 0, 10, 10));
  surf = dt->Snapshot();
  for (int i = 0; i < 200; i++) {
    mDT->DrawSurfaceWithShadow(surf, Point(100, 100), Color(0, 0, 0, 1.0f), Point(), 20.0f, CompositionOp::OP_OVER);
  }
  Flush();
}

void
TestDrawTargetBase::DrawShadow200x200LargeRadius()
{
  RefPtr<SourceSurface> surf = CreateSquareRandomSourceSurface(200, SurfaceFormat::B8G8R8A8);
  RefPtr<DrawTarget> dt = mDT->CreateShadowDrawTarget(IntSize(200, 200), SurfaceFormat::B8G8R8A8, 20.0f);
  dt->DrawSurface(surf, Rect(0, 0, 200, 200), Rect(0, 0, 200, 200));
  surf = dt->Snapshot();
  for (int i = 0; i < 200; i++) {
    mDT->DrawSurfaceWithShadow(surf, Point(100, 100), Color(0, 0, 0, 1.0f), Point(), 20.0f, CompositionOp::OP_OVER);
  }
  Flush();
}

void
TestDrawTargetBase::CreateRandom200()
{
  mRandom200 = CreateSquareRandomSourceSurface(200, SurfaceFormat::B8G8R8A8, false);
}

void
TestDrawTargetBase::DrawTurbulence500x500x10()
{
  mDT->ClearRect(Rect(0, 0, DT_WIDTH, DT_HEIGHT));

  RefPtr<FilterNode> filter = mDT->CreateFilter(FilterType::TURBULENCE);

  for (int32_t i = 0; i < 10; i++) {
    filter->SetAttribute(ATT_TURBULENCE_BASE_FREQUENCY, Size(0.025, 0.025));
    filter->SetAttribute(ATT_TURBULENCE_NUM_OCTAVES, 4u);
    filter->SetAttribute(ATT_TURBULENCE_SEED, 0u);
    filter->SetAttribute(ATT_TURBULENCE_STITCHABLE, false);
    filter->SetAttribute(ATT_TURBULENCE_TYPE, (uint32_t)TURBULENCE_TYPE_TURBULENCE);
    filter->SetAttribute(ATT_TURBULENCE_RECT, IntRect(0, 0, 500, 500));

    mDT->DrawFilter(filter, Rect(0, 0, 500, 500), Point());
  }
  Flush();
}

void
TestDrawTargetBase::DrawMorphologyFilter200x200x100Radius40()
{
  mDT->ClearRect(Rect(0, 0, DT_WIDTH, DT_HEIGHT));

  RefPtr<SourceSurface> src = mRandom200;

  for (int32_t i = 0; i < 100; i++) {
    RefPtr<FilterNode> filter = mDT->CreateFilter(FilterType::MORPHOLOGY);
    filter->SetAttribute(ATT_MORPHOLOGY_RADII, IntSize(40, 40));
    filter->SetAttribute(ATT_MORPHOLOGY_OPERATOR, (uint32_t)MORPHOLOGY_OPERATOR_DILATE);
    filter->SetInput(0, src);
    mDT->DrawFilter(filter, Rect(0, 0, 200, 200), Point());
  }

  Flush();
}

void
TestDrawTargetBase::Premultiply200x200x1000()
{
  mDT->ClearRect(Rect(0, 0, DT_WIDTH, DT_HEIGHT));

  RefPtr<SourceSurface> surf = mRandom200;
  for (int i = 0; i < 1000; i++) {
    RefPtr<FilterNode> filter = mDT->CreateFilter(FilterType::PREMULTIPLY);
    filter->SetInput(IN_PREMULTIPLY_IN, surf);
    mDT->DrawFilter(filter, Rect(0, 0, 200, 200), Point());
  }

  Flush();
}

void
TestDrawTargetBase::Unpremultiply200x200x1000()
{
  mDT->ClearRect(Rect(0, 0, DT_WIDTH, DT_HEIGHT));

  RefPtr<SourceSurface> surf = mRandom200;
  RefPtr<DrawTarget> dt = mDT->CreateSimilarDrawTarget(IntSize(200, 200), SurfaceFormat::B8G8R8A8);
  RefPtr<FilterNode> premultiply = mDT->CreateFilter(FilterType::PREMULTIPLY);
  premultiply->SetInput(IN_PREMULTIPLY_IN, surf);
  dt->DrawFilter(premultiply, Rect(0, 0, 200, 200), Point());
  surf = dt->Snapshot();

  for (int i = 0; i < 1000; i++) {
    RefPtr<FilterNode> filter = mDT->CreateFilter(FilterType::UNPREMULTIPLY);
    filter->SetInput(IN_UNPREMULTIPLY_IN, surf);
    mDT->DrawFilter(filter, Rect(0, 0, 200, 200), Point());
  }

  Flush();
}

void
TestDrawTargetBase::ComponentTransfer200x200x1000()
{
  mDT->ClearRect(Rect(0, 0, DT_WIDTH, DT_HEIGHT));

  RefPtr<SourceSurface> surf = mRandom200;

  Float table[256];
  uint8_t c = 0;
  for (size_t i = 0; i < 256; i++) {
    table[i] = c;
    c += 17;
  }

  for (int i = 0; i < 1000; i++) {
    RefPtr<FilterNode> filter = mDT->CreateFilter(FilterType::DISCRETE_TRANSFER);
    filter->SetAttribute(ATT_DISCRETE_TRANSFER_DISABLE_R, false);
    filter->SetAttribute(ATT_DISCRETE_TRANSFER_DISABLE_G, false);
    filter->SetAttribute(ATT_DISCRETE_TRANSFER_DISABLE_B, false);
    filter->SetAttribute(ATT_DISCRETE_TRANSFER_TABLE_R, table, 256);
    filter->SetAttribute(ATT_DISCRETE_TRANSFER_TABLE_G, table, 256);
    filter->SetAttribute(ATT_DISCRETE_TRANSFER_TABLE_B, table, 256);
    filter->SetInput(IN_DISCRETE_TRANSFER_IN, surf);
    mDT->DrawFilter(filter, Rect(0, 0, 200, 200), Point());
  }

  // Flush();
}

void
TestDrawTargetBase::ColorMatrix200x200x1000()
{
  mDT->ClearRect(Rect(0, 0, DT_WIDTH, DT_HEIGHT));

  RefPtr<SourceSurface> surf = mRandom200;

  Matrix5x4 m(0.1, 0.4, 0.3, 0.2,
              0.4, 0.2, 0.2, 0.1,
              0.2, 0.3, 0.1, 0.4,
              0.3, 0.1, 0.4, 0.2,
              0.0, 0.0, 0.0, 0.0);

  for (int i = 0; i < 1000; i++) {
    RefPtr<FilterNode> filter = mDT->CreateFilter(FilterType::COLOR_MATRIX);
    filter->SetAttribute(ATT_COLOR_MATRIX_MATRIX, m);
    filter->SetAttribute(ATT_COLOR_MATRIX_ALPHA_MODE, (uint32_t)ALPHA_MODE_STRAIGHT);
    filter->SetInput(IN_COLOR_MATRIX_IN, surf);
    mDT->DrawFilter(filter, Rect(0, 0, 200, 200), Point());
  }

  Flush();
}

void
TestDrawTargetBase::Composite200x200x1000()
{
  mDT->ClearRect(Rect(0, 0, DT_WIDTH, DT_HEIGHT));

  RefPtr<SourceSurface> surf = mRandom200;
  RefPtr<DrawTarget> dt = mDT->CreateSimilarDrawTarget(IntSize(200, 200), SurfaceFormat::B8G8R8A8);
  RefPtr<FilterNode> premultiply = mDT->CreateFilter(FilterType::PREMULTIPLY);
  premultiply->SetInput(IN_PREMULTIPLY_IN, surf);
  dt->DrawFilter(premultiply, Rect(0, 0, 200, 200), Point());
  surf = dt->Snapshot();

  for (int i = 0; i < 1000; i++) {
    RefPtr<FilterNode> filter = mDT->CreateFilter(FilterType::COMPOSITE);
    filter->SetAttribute(ATT_COMPOSITE_OPERATOR, (uint32_t)COMPOSITE_OPERATOR_XOR);
    filter->SetInput(IN_COMPOSITE_IN_START, surf);
    filter->SetInput(IN_COMPOSITE_IN_START + 1, surf);
    filter->SetInput(IN_COMPOSITE_IN_START + 2, surf);
    mDT->DrawFilter(filter, Rect(0, 0, 200, 200), Point());
  }

  Flush();
}

void
TestDrawTargetBase::CompositeA8Single200x200x1000()
{
  mDT->ClearRect(Rect(0, 0, DT_WIDTH, DT_HEIGHT));

  RefPtr<SourceSurface> surf = mRandom200;
  RefPtr<DrawTarget> dt = mDT->CreateSimilarDrawTarget(IntSize(200, 200), SurfaceFormat::A8);
  RefPtr<FilterNode> premultiply = mDT->CreateFilter(FilterType::PREMULTIPLY);
  premultiply->SetInput(IN_PREMULTIPLY_IN, surf);
  dt->DrawFilter(premultiply, Rect(0, 0, 200, 200), Point());
  surf = dt->Snapshot();

  for (int i = 0; i < 1000; i++) {
    RefPtr<FilterNode> filter = mDT->CreateFilter(FilterType::COMPOSITE);
    filter->SetAttribute(ATT_COMPOSITE_OPERATOR, (uint32_t)COMPOSITE_OPERATOR_OVER);
    filter->SetInput(IN_COMPOSITE_IN_START, surf);
    mDT->DrawFilter(filter, Rect(0, 0, 200, 200), Point());
  }

  Flush();
}

void
TestDrawTargetBase::Blend200x200x1000()
{
  mDT->ClearRect(Rect(0, 0, DT_WIDTH, DT_HEIGHT));

  RefPtr<SourceSurface> surf = mRandom200;
  RefPtr<DrawTarget> dt = mDT->CreateSimilarDrawTarget(IntSize(200, 200), SurfaceFormat::B8G8R8A8);
  RefPtr<FilterNode> premultiply = mDT->CreateFilter(FilterType::PREMULTIPLY);
  premultiply->SetInput(IN_PREMULTIPLY_IN, surf);
  dt->DrawFilter(premultiply, Rect(0, 0, 200, 200), Point());
  surf = dt->Snapshot();

  for (int i = 0; i < 1000; i++) {
    RefPtr<FilterNode> filter = mDT->CreateFilter(FilterType::BLEND);
    filter->SetAttribute(ATT_BLEND_BLENDMODE, (uint32_t)BLEND_MODE_MULTIPLY);
    filter->SetInput(IN_BLEND_IN, surf);
    filter->SetInput(IN_BLEND_IN2, surf);
    mDT->DrawFilter(filter, Rect(0, 0, 200, 200), Point());
  }

  Flush();
}

void
TestDrawTargetBase::Blur500x500x50()
{
  mDT->ClearRect(Rect(0, 0, DT_WIDTH, DT_HEIGHT));

  RefPtr<DrawTarget> dt = mDT->CreateSimilarDrawTarget(IntSize(500, 500), SurfaceFormat::B8G8R8A8);
  RefPtr<FilterNode> tile = mDT->CreateFilter(FilterType::TILE);
  RefPtr<FilterNode> premultiply = mDT->CreateFilter(FilterType::PREMULTIPLY);
  premultiply->SetInput(IN_PREMULTIPLY_IN, mRandom200);
  tile->SetAttribute(ATT_TILE_SOURCE_RECT, IntRect(0, 0, 200, 200));
  tile->SetInput(IN_TILE_IN, premultiply);
  dt->DrawFilter(tile, Rect(0, 0, 500, 500), Point());
  RefPtr<SourceSurface> surf = dt->Snapshot();

  for (int i = 0; i < 50; i++) {
    RefPtr<FilterNode> filter = mDT->CreateFilter(FilterType::GAUSSIAN_BLUR);
    filter->SetAttribute(ATT_GAUSSIAN_BLUR_STD_DEVIATION, 40.0f);
    filter->SetInput(IN_GAUSSIAN_BLUR_IN, surf);
    mDT->DrawFilter(filter, Rect(0, 0, 500, 500), Point());
  }

  Flush();
}

void
TestDrawTargetBase::ArithmeticCombine200x200x100()
{
  mDT->ClearRect(Rect(0, 0, DT_WIDTH, DT_HEIGHT));

  RefPtr<SourceSurface> surf = mRandom200;
  RefPtr<DrawTarget> dt = mDT->CreateSimilarDrawTarget(IntSize(200, 200), SurfaceFormat::B8G8R8A8);
  RefPtr<FilterNode> premultiply = mDT->CreateFilter(FilterType::PREMULTIPLY);
  premultiply->SetInput(IN_PREMULTIPLY_IN, surf);
  dt->DrawFilter(premultiply, Rect(0, 0, 200, 200), Point());
  surf = dt->Snapshot();
  Float coeffs[4] = { 0.2f, -0.5f, 0.3f, 0.1f };

  for (int i = 0; i < 1000; i++) {
    RefPtr<FilterNode> filter = mDT->CreateFilter(FilterType::ARITHMETIC_COMBINE);
    filter->SetAttribute(ATT_ARITHMETIC_COMBINE_COEFFICIENTS, coeffs, 4);
    filter->SetInput(IN_ARITHMETIC_COMBINE_IN, surf);
    filter->SetInput(IN_ARITHMETIC_COMBINE_IN2, surf);
    mDT->DrawFilter(filter, Rect(0, 0, 200, 200), Point());
  }

  Flush();
}

TemporaryRef<SourceSurface>
TestDrawTargetBase::CreateSquareRandomSourceSurface(int aSize, SurfaceFormat aFormat, bool aLeaveUninitialized)
{
  size_t length = aSize * aSize * BytesPerPixel(aFormat);
  unsigned char *surfData = new unsigned char[length];

  if (!aLeaveUninitialized) {
    uint8_t c = 0;
    for (size_t i = 0; i < length; i++) {
      surfData[i] = c;
      c += 113;
      if (i % 4 == 0) {
        c += 173;
      }
    }
  }

  RefPtr<SourceSurface> surf = mDT->CreateSourceSurfaceFromData(surfData, IntSize(aSize, aSize), aSize * BytesPerPixel(aFormat), aFormat);

  delete [] surfData;

  return surf;
}

TemporaryRef<GradientStops>
TestDrawTargetBase::CreateSimpleGradientStops()
{
  GradientStop stops[2];
  stops[0].color = Color(1.0f, 0, 0, 1.0f);
  stops[0].offset = 0;
  stops[1].color = Color(0, 1.0f, 0, 1.0f);
  stops[1].offset = 1.0f;

  return mDT->CreateGradientStops(stops, 2);
}
