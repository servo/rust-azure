/* -*- Mode: c++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ConvexPolygon.h"

using namespace std;

namespace mozilla {
namespace gfx {
namespace nvpr {

static inline Float
CrossProduct(const Point& aPt1, const Point& aPt2, const Point& aPt3)
{
  return (aPt2.x - aPt1.x) * (aPt3.y - aPt2.y)
         - (aPt2.y - aPt1.y) * (aPt3.y - aPt2.x);
}

static inline Point
Intersection(const Point& aPt1, const Line& aLine, const Point& aPt2)
{
  const Point v = aPt2 - aPt1;
  const Float t = (aLine.C - aPt1.x * aLine.A - aPt1.y * aLine.B)
                  / (v.x * aLine.A + v.y * aLine.B);
  return aPt1 + v * t;
}


ConvexPolygon::ConvexPolygon(const ConvexPolygon& aPolygon)
  : mPoints(aPolygon.mPoints)
{
}

ConvexPolygon::ConvexPolygon(ConvexPolygon&& aPolygon)
{
  Swap(aPolygon);
}

ConvexPolygon::ConvexPolygon(const Rect& aRect)
{
  if (!aRect.width || !aRect.height) {
    return;
  }

  mPoints.reserve(4);
  mPoints.push_back(aRect.BottomRight());
  mPoints.push_back(aRect.TopRight());
  mPoints.push_back(aRect.TopLeft());
  mPoints.push_back(aRect.BottomLeft());

  EnsureAntiClockwise();
}

ConvexPolygon::ConvexPolygon(const vector<Point>& aPoints)
{
  if (aPoints.size() < 3) {
    return;
  }

  mPoints.reserve(aPoints.size());

  Float previousCross = 0;

  for (size_t i = 0; i < aPoints.size(); i++) {
    Float cross = CrossProduct(aPoints[i ? i - 1 : aPoints.size() - 1],
                               aPoints[i],
                               aPoints[i + 1 < aPoints.size() ? i + 1 : 0]);

    if (!cross) {
      // This point is irrelevant since it's between two colinear points.
      continue;
    }

    if (previousCross && (previousCross > 0) != (cross > 0)) {
      // The polygon is not convex.
      mPoints.clear();
      return;
    }

    mPoints.push_back(aPoints[i]);
    previousCross = cross;
  }

  EnsureAntiClockwise();
}

ConvexPolygon&
ConvexPolygon::operator =(const ConvexPolygon& aPolygon)
{
  mPoints = aPolygon.mPoints;
  mSides.clear();
  return *this;
}

ConvexPolygon&
ConvexPolygon::operator =(ConvexPolygon&& aPolygon)
{
  Swap(aPolygon);
  return *this;
}

void
ConvexPolygon::EnsureAntiClockwise()
{
  if (mPoints.size() < 3) {
    return;
  }

  if (CrossProduct(mPoints[0], mPoints[1], mPoints[2]) < 0) {
    // The points are clockwise.
    reverse(mPoints.begin(), mPoints.end());
  }
}

const Line*
ConvexPolygon::Sides() const
{
  if (mSides.empty() && mPoints.size() >= 3) {
    for (size_t i = 1; i < mPoints.size(); i++) {
      mSides.push_back(Line(mPoints[i - 1], mPoints[i]));
    }
    mSides.push_back(Line(mPoints.back(), mPoints.front()));
  }

  return mSides.data();
}

void
ConvexPolygon::Transform(const Matrix& aTransform)
{
  if (aTransform.IsIdentity()) {
    return;
  }

  for (Point& point : mPoints) {
    point = aTransform * point;
  }

  mSides.clear();
  EnsureAntiClockwise();
}

void
ConvexPolygon::Intersect(const ConvexPolygon& aPolygon)
{
  if (mPoints.size() < 3) {
    return;
  }

  // This algorithm is O(N^2), but it's perfectly acceptable and maybe even
  // faster for the size of polygons we expect to be dealing with.
  for (Line& line : aPolygon.mSides) {
    Clip(line);
  }
}

void
ConvexPolygon::Clip(const Line& aLine)
{
  if (mPoints.size() < 3) {
    return;
  }

  const Point firstPoint = mPoints.front();
  const bool firstInside = aLine.A * firstPoint.x
                           + aLine.B * firstPoint.y >= aLine.C;

  Point previousPoint = firstPoint;
  bool previousInside = firstInside;

  size_t newSize = 0;

  // Start on the point at index 1 so we can update the points in-place.
  for (size_t i = 1; i < mPoints.size(); i++) {
    const Point point = mPoints[i];
    const bool inside = aLine.A * point.x + aLine.B * point.y >= aLine.C;

    if (previousInside != inside) {
      MOZ_ASSERT(newSize <= i);
      mPoints[newSize++] = Intersection(previousPoint, aLine, point);
    }

    if (inside) {
      MOZ_ASSERT(newSize <= i);
      mPoints[newSize++] = point;
    }

    previousPoint = point;
    previousInside = inside;
  }

  // Finally add the first point that we initially skipped.
  if (previousInside != firstInside) {
    MOZ_ASSERT(newSize <= mPoints.size());
    if (newSize == mPoints.size()) {
      mPoints.resize(mPoints.size() + 1);
    }
    mPoints[newSize++] = Intersection(previousPoint, aLine, firstPoint);
  }

  if (firstInside) {
    MOZ_ASSERT(newSize <= mPoints.size());
    if (newSize == mPoints.size()) {
      mPoints.resize(mPoints.size() + 1);
    }
    mPoints[newSize++] = firstPoint;
  }

  mPoints.resize(newSize);
  mSides.clear();
}

void
ConvexPolygon::Swap(ConvexPolygon& aPolygon)
{
  swap(mPoints, aPolygon.mPoints);
  swap(mSides, aPolygon.mSides);
}

}
}
}

