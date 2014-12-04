/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_NVPR_CONVEXPOLYGON_H_
#define MOZILLA_GFX_NVPR_CONVEXPOLYGON_H_

#include "Types.h"
#include "Line.h"
#include "Matrix.h"
#include "Point.h"
#include "Rect.h"
#include <vector>

namespace mozilla {
namespace gfx {
namespace nvpr {

class ConvexPolygon {
public:
  ConvexPolygon() {}
  ConvexPolygon(const ConvexPolygon& aPolygon);
  ConvexPolygon(ConvexPolygon&& aPolygon);
  ConvexPolygon(const Rect& aRect);
  ConvexPolygon(const std::vector<Point>& aPoints);

  ConvexPolygon& operator =(const ConvexPolygon& aPolygon);
  ConvexPolygon& operator =(ConvexPolygon&& aPolygon);

  bool IsEmpty() const { return mPoints.empty(); }
  size_t NumSides() const { return mPoints.size(); }
  const Point* Points() const { return mPoints.data(); }
  const Line* Sides() const;

  void Transform(const Matrix& aTransform);
  void Intersect(const ConvexPolygon& aPolygon);
  void Clip(const Line& aLine);

  void Swap(ConvexPolygon& aPolygon);

private:
  void EnsureAntiClockwise();

  std::vector<Point> mPoints;
  mutable std::vector<Line> mSides;
};

}
}
}

namespace std
{
  template<>
  inline void swap(mozilla::gfx::nvpr::ConvexPolygon& aPolygon1,
                   mozilla::gfx::nvpr::ConvexPolygon& aPolygon2)
  {
    aPolygon1.Swap(aPolygon2);
  }
}

#endif /* MOZILLA_GFX_NVPR_CONVEXPOLYGON_H_ */
