/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_LINE_H_
#define MOZILLA_GFX_LINE_H_

#include "Types.h"
#include "Point.h"

namespace mozilla {
namespace gfx {

/**
 * This struct defines a line in standard form:
 *
 *   Ax + By = C
 */
struct Line {
  Float A, B, C;

  // Constructors
  Line() : A(0), B(0), C(0) {}
  Line(Float aA, Float aB, Float aC) : A(aA), B(aB), C(aC) {}
  Line(const Point& aPt1, const Point& aPt2)
    : A(aPt1.y - aPt2.y)
    , B(aPt2.x - aPt1.x)
    , C(A * aPt1.x + B * aPt1.y)
  {}

  Line operator -()
  {
    return Line(-A, -B, -C);
  }
};

}
}

#endif /* MOZILLA_GFX_LINE_H_ */
