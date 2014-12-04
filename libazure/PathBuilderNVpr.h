/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_PATHBUILDERNVPR_H_
#define MOZILLA_GFX_PATHBUILDERNVPR_H_

#include "2D.h"
#include "nvpr/GL.h"
#include <vector>

namespace mozilla {
namespace gfx {

class PathObjectNVpr;

/**
 * This is basically just a wrapper for NV_path_rendering path commands.
 * However, it also serves as the path cache's key data type. The path cache is
 * a binary tree, so it uses this class's less-than operator to do its magic.
 * Cache accesses are very quick because the less-than operator is an O(1)
 * operation on average (we can almost always determine which of two path
 * descriptions is "greater" by only looking at the lengths of the arrays, or
 * the first couple of coordinates).
 */
struct PathDescriptionNVpr {
  PathDescriptionNVpr() {}

  PathDescriptionNVpr(const PathDescriptionNVpr& aOther);
  PathDescriptionNVpr(PathDescriptionNVpr&& aOther);

  bool operator <(const PathDescriptionNVpr& aOther) const;

  bool IsEmpty() const { return mCommands.empty() && mCoords.empty(); }
  void Clear() { mCommands.clear(); mCoords.clear(); }
  void AppendCommand(GLubyte aCommand) { mCommands.push_back(aCommand); }
  void AppendPoint(const Point& aPt) { AppendFloat(aPt.x); AppendFloat(aPt.y); }
  void AppendFloat(float aVal) { mCoords.push_back(aVal); }

  std::vector<GLubyte> mCommands;
  std::vector<GLfloat> mCoords;
};

/**
 * This is the NV_path_rendering-specific implementation for Azure's PathBuilder
 * interface. A path builder generally has a PathDescriptionNVpr that it fills
 * with commands. On Finish, it uses its path description to locate a saved GL
 * path object in the draw target's path cache, or else create a new one.
 * However, when Azure calls CopyToBuilder (or TransformedCopyToBuilder) on a
 * path, this class just takes a reference to the existing GL path object. Once
 * the client code actually tries to modify the path, it populates and begins
 * using the path description, and releases the original GL path object.
 */
class PathBuilderNVpr : public PathBuilder
{
public:
  PathBuilderNVpr(FillRule aFillRule);
  PathBuilderNVpr(FillRule aFillRule, TemporaryRef<PathObjectNVpr> aPathObject);
  PathBuilderNVpr(FillRule aFillRule, TemporaryRef<PathObjectNVpr> aPathObject,
                  const Matrix& aTransform);
  virtual ~PathBuilderNVpr();

  virtual void MoveTo(const Point& aPoint);
  virtual void LineTo(const Point& aPoint);
  virtual void BezierTo(const Point& aCP1,
                        const Point& aCP2,
                        const Point& aCP3);
  virtual void QuadraticBezierTo(const Point& aCP1,
                                 const Point& aCP2);
  virtual void Close();
  virtual void Arc(const Point& aOrigin, Float aRadius, Float aStartAngle,
                   Float aEndAngle, bool aAntiClockwise = false);

  virtual Point CurrentPoint() const;

  virtual TemporaryRef<Path> Finish();

private:
  void MakeWritable();

  const FillRule mFillRule;
  RefPtr<PathObjectNVpr> mPathObject;
  PathDescriptionNVpr mDescription;
  Point mStartPoint;
  Point mCurrentPoint;
  bool mIsPolygon;
};

}
}

#endif /* MOZILLA_GFX_PATHBUILDERNVPR_H_ */
