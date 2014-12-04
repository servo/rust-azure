/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_PATHNVPR_H_
#define MOZILLA_GFX_PATHNVPR_H_

#include "2D.h"
#include "nvpr/ConvexPolygon.h"
#include "nvpr/GL.h"
#include <vector>

namespace mozilla {
namespace gfx {

struct Line;
struct PathDescriptionNVpr;

/**
 * This is a ref-counted wrapper for NV_path_rendering path objects. It manages
 * the path's GL state and allows multiple clients to share the same GL object
 * (e.g. PathNVpr, the path cache, etc.)
 */
class PathObjectNVpr : public RefCounted<PathObjectNVpr> {
public:
  PathObjectNVpr(const PathDescriptionNVpr& aDescription,
                 const Point& aStartPoint, const Point& aCurrentPoint,
                 nvpr::ConvexPolygon&& aPassPolygon = nvpr::ConvexPolygon());
  PathObjectNVpr(const PathObjectNVpr& aPath, const Matrix& aTransform);
  ~PathObjectNVpr();

  const Point& StartPoint() const { return mStartPoint; }
  const Point& CurrentPoint() const { return mCurrentPoint; }
  const nvpr::ConvexPolygon& Polygon() const { return mPolygon; }
  operator GLuint() const { return mObject; }

  void ApplySencliClipBits(GLuint aClipBits);
  void ApplyStrokeOptions(const StrokeOptions& aStrokeOptions);

private:
  const Point mStartPoint;
  const Point mCurrentPoint;
  nvpr::ConvexPolygon mPolygon;
  GLuint mObject;

  GLuint mStencilClipBits;
  GLfloat mStrokeWidth;
  GLfloat mMiterLimit;
  GLfloat mDashOffset;
  JoinStyle mJoinStyle;
  CapStyle mCapStyle;
  std::vector<GLfloat> mDashArray;
};

/**
 * This is the NV_path_rendering-specific implementation for Azure's Path
 * interface. It's basically just a reference to a GL path object plus a fill
 * rule.
 */
class PathNVpr : public Path
{
public:
  PathNVpr(FillRule aFillRule, TemporaryRef<PathObjectNVpr> aPathObject);

  virtual BackendType GetBackendType() const { return BackendType::NVPR; }

  virtual TemporaryRef<PathBuilder> CopyToBuilder(FillRule aFillRule = FillRule::FILL_WINDING) const;
  virtual TemporaryRef<PathBuilder> TransformedCopyToBuilder(const Matrix& aTransform,
                                                             FillRule aFillRule = FillRule::FILL_WINDING) const;

  virtual bool ContainsPoint(const Point& aPoint, const Matrix& aTransform) const;

  virtual bool StrokeContainsPoint(const StrokeOptions& aStrokeOptions,
                                   const Point& aPoint,
                                   const Matrix& aTransform) const;

  virtual Rect GetBounds(const Matrix& aTransform = Matrix()) const;

  virtual Rect GetStrokedBounds(const StrokeOptions& aStrokeOptions,
                                const Matrix& aTransform = Matrix()) const;

  virtual void StreamToSink(PathSink *aSink) const { MOZ_ASSERT(false); }

  virtual FillRule GetFillRule() const { return mFillRule; }

  bool IsSamePath(const PathNVpr* aPath) const;

  const nvpr::ConvexPolygon& Polygon() const { return mPathObject->Polygon(); }

  operator GLuint() const { return *mPathObject; }

  TemporaryRef<PathNVpr> Clone() const;

  void ApplyStrokeOptions(const StrokeOptions& aStrokeOptions) const;

private:
  const FillRule mFillRule;
  RefPtr<PathObjectNVpr> mPathObject;
};

}
}

#endif /* MOZILLA_GFX_PATHNVPR_H_ */
