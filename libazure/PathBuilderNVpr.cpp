/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "PathBuilderNVpr.h"
#include "PathNVpr.h"
#include "Line.h"
#include "nvpr/ConvexPolygon.h"
#include <map>

using namespace mozilla::gfx::nvpr;
using namespace std;

namespace mozilla {
namespace gfx {

class PathCache
  : public map<PathDescriptionNVpr, RefPtr<PathObjectNVpr> >
  , public nvpr::UserData::Object
{};

PathBuilderNVpr::PathBuilderNVpr(FillRule aFillRule)
  : mFillRule(aFillRule)
  , mIsPolygon(true)
{
}

PathBuilderNVpr::PathBuilderNVpr(FillRule aFillRule,
                                 TemporaryRef<PathObjectNVpr> aPathObject)
  : mFillRule(aFillRule)
{
  RefPtr<PathObjectNVpr> pathObject = aPathObject;
  mPathObject = pathObject.forget();
  mIsPolygon = mPathObject->Polygon().IsEmpty();
}

PathBuilderNVpr::PathBuilderNVpr(FillRule aFillRule,
                                 TemporaryRef<PathObjectNVpr> aPathObject,
                                 const Matrix& aTransform)
  : mFillRule(aFillRule)
{
  RefPtr<PathObjectNVpr> pathObject = aPathObject;
  mPathObject = new PathObjectNVpr(*pathObject, aTransform);
  mIsPolygon = mPathObject->Polygon().IsEmpty();
}

PathBuilderNVpr::~PathBuilderNVpr()
{
}

void
PathBuilderNVpr::MoveTo(const Point& aPoint)
{
  MakeWritable();

  if (!mDescription.IsEmpty()) {
    mIsPolygon = false;
  }

  mDescription.AppendCommand(GL_MOVE_TO_NV);
  mDescription.AppendPoint(aPoint);

  mStartPoint = aPoint;
  mCurrentPoint = aPoint;
}

void
PathBuilderNVpr::LineTo(const Point& aPoint)
{
  MakeWritable();

  if (mDescription.IsEmpty()) {
    MoveTo(aPoint);
    return;
  }

  if (mDescription.mCommands.back() != GL_MOVE_TO_NV
      && mDescription.mCommands.back() != GL_LINE_TO_NV) {
    mIsPolygon = false;
  }

  mDescription.AppendCommand(GL_LINE_TO_NV);
  mDescription.AppendPoint(aPoint);

  mCurrentPoint = aPoint;
}

void
PathBuilderNVpr::BezierTo(const Point& aCP1,
                          const Point& aCP2,
                          const Point& aCP3)
{
  MakeWritable();

  if (mDescription.IsEmpty()) {
    MoveTo(aCP1);
  }

  mDescription.AppendCommand(GL_CUBIC_CURVE_TO_NV);
  mDescription.AppendPoint(aCP1);
  mDescription.AppendPoint(aCP2);
  mDescription.AppendPoint(aCP3);

  mCurrentPoint = aCP3;

  mIsPolygon = false;
}

void
PathBuilderNVpr::QuadraticBezierTo(const Point& aCP1,
                                   const Point& aCP2)
{
  MakeWritable();

  if (mDescription.IsEmpty()) {
    MoveTo(aCP1);
  }

  mDescription.AppendCommand(GL_QUADRATIC_CURVE_TO_NV);
  mDescription.AppendPoint(aCP1);
  mDescription.AppendPoint(aCP2);

  mCurrentPoint = aCP2;

  mIsPolygon = false;
}

void
PathBuilderNVpr::Close()
{
  MakeWritable();

  mDescription.AppendCommand(GL_CLOSE_PATH_NV);

  mCurrentPoint = mStartPoint;
}

void
PathBuilderNVpr::Arc(const Point& aOrigin, Float aRadius, Float aStartAngle,
                     Float aEndAngle, bool aAntiClockwise)
{
  MakeWritable();

  const Point startPoint(aOrigin.x + cos(aStartAngle) * aRadius,
                         aOrigin.y + sin(aStartAngle) * aRadius);

  // The spec says to begin with a line to the start point.
  LineTo(startPoint);

  mIsPolygon = false;

  if (fabs(aEndAngle - aStartAngle) > 2 * M_PI - 1e-5) {
    // The spec says to just draw the whole circle in this case.
    mDescription.AppendCommand(GL_CIRCULAR_CCW_ARC_TO_NV);
    mDescription.AppendPoint(aOrigin);
    mDescription.AppendFloat(aRadius);
    mDescription.AppendFloat(aStartAngle * 180 / M_PI);
    mDescription.AppendFloat(360 + aStartAngle * 180 / M_PI);
    return;
  }

  const Point endPoint(aOrigin.x + cos(aEndAngle) * aRadius,
                       aOrigin.y + sin(aEndAngle) * aRadius);

  if (aAntiClockwise && aEndAngle < aStartAngle) {
    aEndAngle += 2 * M_PI;
  } else if (!aAntiClockwise && aEndAngle > aStartAngle) {
    aEndAngle -= 2 * M_PI;
  }

  // 'Anticlockwise' in HTML5 seems to be relative to a downward-pointing Y-axis,
  // whereas CW/CCW are relative to an upward-facing Y-axis in NV_path_rendering.
  if (fabs(aEndAngle - aStartAngle) < M_PI) {
    mDescription.AppendCommand(aAntiClockwise ? GL_LARGE_CW_ARC_TO_NV
                                              : GL_LARGE_CCW_ARC_TO_NV);
  } else {
    mDescription.AppendCommand(aAntiClockwise ? GL_SMALL_CW_ARC_TO_NV
                                              : GL_SMALL_CCW_ARC_TO_NV);
  }
  mDescription.AppendFloat(aRadius); // x-radius
  mDescription.AppendFloat(aRadius); // y-radius
  mDescription.AppendFloat(0);
  mDescription.AppendPoint(endPoint);

  mCurrentPoint = endPoint;
}

Point
PathBuilderNVpr::CurrentPoint() const
{
  return mCurrentPoint;
}

TemporaryRef<Path>
PathBuilderNVpr::Finish()
{
  if (mPathObject) {
    MOZ_ASSERT(mDescription.IsEmpty());

    // Client code called 'CopyToBuilder' and then didn't modify the path.
    return new PathNVpr(mFillRule, mPathObject.forget());
  }

  PathCache& pathCache = gl->GetUserObject<PathCache>(&nvpr::UserData::mPathCache);
  RefPtr<PathObjectNVpr>& pathObject = pathCache[mDescription];

  if (pathObject) {
    return new PathNVpr(mFillRule, pathObject);
  }

  if (mIsPolygon) {
    vector<Point> points(mDescription.mCoords.size() / 2);
    for (size_t i = 0; i < mDescription.mCoords.size(); i += 2) {
      points.push_back(Point(mDescription.mCoords[i],
                             mDescription.mCoords[i + 1]));
    }

    pathObject = new PathObjectNVpr(mDescription, mStartPoint, mCurrentPoint,
                                    ConvexPolygon(points));
  } else {
    pathObject = new PathObjectNVpr(mDescription, mStartPoint, mCurrentPoint);
  }

  return new PathNVpr(mFillRule, pathObject);
}

void
PathBuilderNVpr::MakeWritable()
{
  if (!mPathObject) {
    return;
  }

  MOZ_ASSERT(mDescription.IsEmpty());

  gl->MakeCurrent();

  GLint commandCount;
  gl->GetPathParameterivNV(*mPathObject, GL_PATH_COMMAND_COUNT_NV, &commandCount);
  mDescription.mCommands.resize(commandCount);
  gl->GetPathCommandsNV(*mPathObject, mDescription.mCommands.data());

  GLint coordCount;
  gl->GetPathParameterivNV(*mPathObject, GL_PATH_COORD_COUNT_NV, &coordCount);
  mDescription.mCoords.resize(coordCount);
  gl->GetPathCoordsNV(*mPathObject, mDescription.mCoords.data());

  mStartPoint = mPathObject->StartPoint();
  mCurrentPoint = mPathObject->CurrentPoint();

  mPathObject = nullptr;
}


PathDescriptionNVpr::PathDescriptionNVpr(const PathDescriptionNVpr& aOther)
  : mCommands(aOther.mCommands)
  , mCoords(aOther.mCoords)
{
}

PathDescriptionNVpr::PathDescriptionNVpr(PathDescriptionNVpr&& aOther)
{
  swap(mCommands, aOther.mCommands);
  swap(mCoords, aOther.mCoords);
}

bool
PathDescriptionNVpr::operator <(const PathDescriptionNVpr& aOther) const
{
  if (mCoords.size() != aOther.mCoords.size()) {
    return mCoords.size() < aOther.mCoords.size();
  }

  if (mCommands.size() != aOther.mCommands.size()) {
    return mCommands.size() < aOther.mCommands.size();
  }

  for (size_t i = 0; i < mCoords.size(); i++) {
    if (mCoords[i] != aOther.mCoords[i]) {
      return mCoords[i] < aOther.mCoords[i];
    }
  }

  for (size_t i = 0; i < mCommands.size(); i++) {
    if (mCommands[i] != aOther.mCommands[i]) {
      return mCommands[i] < aOther.mCommands[i];
    }
  }

  // The path descriptions equal.
  return false;
}

}
}
