/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "PathNVpr.h"
#include "PathBuilderNVpr.h"
#include "Line.h"

using namespace mozilla::gfx::nvpr;
using namespace std;

namespace mozilla {
namespace gfx {

PathObjectNVpr::PathObjectNVpr(const PathDescriptionNVpr& aDescription,
                               const Point& aStartPoint,
                               const Point& aCurrentPoint,
                               ConvexPolygon&& aPassPolygon)
  : mStartPoint(aStartPoint)
  , mCurrentPoint(aCurrentPoint)
  , mStencilClipBits(0)
  , mStrokeWidth(1)
  , mMiterLimit(4)
  , mDashOffset(0)
  , mJoinStyle(JoinStyle::MITER_OR_BEVEL)
  , mCapStyle(CapStyle::BUTT)
{
  mPolygon.Swap(aPassPolygon);

  gl->MakeCurrent();
  mObject = gl->GenPathsNV(1);
  gl->PathCommandsNV(mObject, aDescription.mCommands.size(),
                     aDescription.mCommands.data(), aDescription.mCoords.size(),
                     GL_FLOAT, aDescription.mCoords.data());
}

PathObjectNVpr::PathObjectNVpr(const PathObjectNVpr& aPathObject,
                               const Matrix& aTransform)
  : mStartPoint(aTransform * aPathObject.mStartPoint)
  , mCurrentPoint(aTransform * aPathObject.mCurrentPoint)
  , mStencilClipBits(0)
  , mStrokeWidth(aPathObject.mStrokeWidth)
  , mMiterLimit(aPathObject.mMiterLimit)
  , mDashOffset(aPathObject.mDashOffset)
  , mJoinStyle(aPathObject.mJoinStyle)
  , mCapStyle(aPathObject.mCapStyle)
{
  mPolygon = aPathObject.mPolygon;
  mPolygon.Transform(aTransform);

  gl->MakeCurrent();

  GLfloat transform[] = {
      aTransform._11, aTransform._21, aTransform._31,
      aTransform._12, aTransform._22, aTransform._32
  };

  mObject = gl->GenPathsNV(1);
  gl->TransformPathNV(mObject, aPathObject.mObject, GL_AFFINE_2D_NV, transform);
}

PathObjectNVpr::~PathObjectNVpr()
{
  gl->MakeCurrent();
  gl->DeletePathsNV(mObject, 1);
}

void PathObjectNVpr::ApplyStrokeOptions(const StrokeOptions& aStrokeOptions)
{
  MOZ_ASSERT(gl->IsCurrent());

  if (mStrokeWidth != aStrokeOptions.mLineWidth) {
    gl->PathParameterfNV(mObject, GL_PATH_STROKE_WIDTH_NV, aStrokeOptions.mLineWidth);
    mStrokeWidth = aStrokeOptions.mLineWidth;
  }

  if (mMiterLimit != aStrokeOptions.mMiterLimit) {
    gl->PathParameterfNV(mObject, GL_PATH_MITER_LIMIT_NV, aStrokeOptions.mMiterLimit);
    mMiterLimit = aStrokeOptions.mMiterLimit;
  }

  if (mDashOffset != aStrokeOptions.mDashOffset) {
    gl->PathParameterfNV(mObject, GL_PATH_DASH_OFFSET_NV, aStrokeOptions.mDashOffset);
    mDashOffset = aStrokeOptions.mDashOffset;
  }

  if (mJoinStyle != aStrokeOptions.mLineJoin) {
    GLenum newJoinStyle;
    switch (aStrokeOptions.mLineJoin) {
      default:
        MOZ_ASSERT(!"Invalid line join style in StrokeOptions");
      case JoinStyle::MITER_OR_BEVEL:
        newJoinStyle = GL_MITER_REVERT_NV;
        break;
      case JoinStyle::MITER:
        newJoinStyle = GL_MITER_TRUNCATE_NV;
        break;
      case JoinStyle::ROUND:
        newJoinStyle = GL_ROUND_NV;
        break;
      case JoinStyle::BEVEL:
        newJoinStyle = GL_BEVEL_NV;
        break;
    }

    gl->PathParameteriNV(mObject, GL_PATH_JOIN_STYLE_NV, newJoinStyle);

    mJoinStyle = aStrokeOptions.mLineJoin;
  }

  if (mCapStyle != aStrokeOptions.mLineCap) {
    GLenum newCapStyle;
    switch (aStrokeOptions.mLineCap) {
      default:
        MOZ_ASSERT(!"Invalid line cap style in StrokeOptions");
      case CapStyle::BUTT:
        newCapStyle = GL_FLAT;
        break;
      case CapStyle::ROUND:
        newCapStyle = GL_ROUND_NV;
        break;
      case CapStyle::SQUARE:
        newCapStyle = GL_SQUARE_NV;
        break;
    }

    gl->PathParameteriNV(mObject, GL_PATH_INITIAL_END_CAP_NV, newCapStyle);
    gl->PathParameteriNV(mObject, GL_PATH_TERMINAL_END_CAP_NV, newCapStyle);
    gl->PathParameteriNV(mObject, GL_PATH_INITIAL_DASH_CAP_NV, newCapStyle);
    gl->PathParameteriNV(mObject, GL_PATH_TERMINAL_DASH_CAP_NV, newCapStyle);

    mCapStyle = aStrokeOptions.mLineCap;
  }

  MOZ_ASSERT(aStrokeOptions.mDashArray || !aStrokeOptions.mDashLength);
  if (mDashArray.size() != aStrokeOptions.mDashLength
      || memcmp(mDashArray.data(), aStrokeOptions.mDashPattern,
                sizeof(mDashArray[0]) * aStrokeOptions.mDashLength)) {

    gl->PathDashArrayNV(mObject, aStrokeOptions.mDashLength,
                        aStrokeOptions.mDashPattern);
    mDashArray.resize(aStrokeOptions.mDashLength);
    memcpy(mDashArray.data(), aStrokeOptions.mDashPattern,
           sizeof(mDashArray[0]) * aStrokeOptions.mDashLength);
  }
}


PathNVpr::PathNVpr(FillRule aFillRule, TemporaryRef<PathObjectNVpr> aPathObject)
  : mFillRule(aFillRule)
  , mPathObject(aPathObject)
{
}

TemporaryRef<PathBuilder>
PathNVpr::CopyToBuilder(FillRule aFillRule) const
{
  return new PathBuilderNVpr(aFillRule, mPathObject.get());
}

TemporaryRef<PathBuilder>
PathNVpr::TransformedCopyToBuilder(const Matrix& aTransform, FillRule aFillRule) const
{
  return new PathBuilderNVpr(aFillRule, mPathObject.get(), aTransform);
}

bool
PathNVpr::ContainsPoint(const Point& aPoint, const Matrix& aTransform) const
{
  Matrix inverse = aTransform;
  inverse.Invert();
  Point transformed = inverse * aPoint;

  gl->MakeCurrent();
  return gl->IsPointInFillPathNV(*mPathObject, mFillRule == FillRule::FILL_WINDING ? ~0 : 0x1,
                                 transformed.x, transformed.y);
}

bool
PathNVpr::StrokeContainsPoint(const StrokeOptions& aStrokeOptions,
                              const Point& aPoint,
                              const Matrix& aTransform) const
{
  Matrix inverse = aTransform;
  inverse.Invert();
  Point transformed = inverse * aPoint;

  gl->MakeCurrent();
  ApplyStrokeOptions(aStrokeOptions);
  return gl->IsPointInStrokePathNV(*mPathObject, transformed.x, transformed.y);
}

Rect
PathNVpr::GetBounds(const Matrix& aTransform) const
{
  gl->MakeCurrent();

  GLfloat bounds[] = {0, 0, 0, 0};

  if (aTransform.IsIdentity()) {
    gl->GetPathParameterfvNV(*mPathObject, GL_PATH_OBJECT_BOUNDING_BOX_NV, bounds);
  } else {
    PathObjectNVpr transformed(*mPathObject, aTransform);
    gl->GetPathParameterfvNV(transformed, GL_PATH_OBJECT_BOUNDING_BOX_NV, bounds);
  }

  return Rect(bounds[0], bounds[2], bounds[2], bounds[3]);
}

Rect
PathNVpr::GetStrokedBounds(const StrokeOptions& aStrokeOptions,
                           const Matrix& aTransform) const
{
  GLfloat bounds[] = {0, 0, 0, 0};

  gl->MakeCurrent();

  ApplyStrokeOptions(aStrokeOptions);

  if (aTransform.IsIdentity()) {
    gl->GetPathParameterfvNV(*mPathObject, GL_PATH_STROKE_BOUNDING_BOX_NV, bounds);
  } else {
    PathObjectNVpr transformed(*mPathObject, aTransform);
    gl->GetPathParameterfvNV(transformed, GL_PATH_STROKE_BOUNDING_BOX_NV, bounds);
  }

  return Rect(bounds[0], bounds[2], bounds[2], bounds[3]);
}

bool
PathNVpr::IsSamePath(const PathNVpr* aPath) const
{
  return mFillRule == aPath->mFillRule && mPathObject == aPath->mPathObject;
}

TemporaryRef<PathNVpr>
PathNVpr::Clone() const
{
  return new PathNVpr(mFillRule, mPathObject.get());
}

void
PathNVpr::ApplyStrokeOptions(const StrokeOptions& aStrokeOptions) const
{
  mPathObject->ApplyStrokeOptions(aStrokeOptions);
}

}
}
