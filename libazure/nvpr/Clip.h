/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_NVPR_CLIP_H_
#define MOZILLA_GFX_NVPR_CLIP_H_

#include "2D.h"
#include "ConvexPolygon.h"
#include "Line.h"
#include "PathNVpr.h"
#include <mozilla/RefPtr.h>

namespace mozilla {
namespace gfx {

class DrawTargetNVpr;
class PathNVpr;

namespace nvpr {

template<typename SubclassType>
class Clip : public RefCounted<SubclassType> {
public:
  Clip(DrawTargetNVpr* aDrawTarget, TemporaryRef<SubclassType> aPrevious)
    : mPrevious(aPrevious)
    , mDrawTarget(aDrawTarget)
  {}

  SubclassType* GetPrevious() const { return mPrevious.get(); }
  TemporaryRef<SubclassType> Pop() { return mPrevious.forget(); }

protected:
  DrawTargetNVpr* const mDrawTarget;
  RefPtr<SubclassType> mPrevious;
};

/**
 * 'Scissor clips' are a stack of pixel-aligned clip rects that use glScissor.
 */
class ScissorClip : public Clip<ScissorClip> {
public:
  static TemporaryRef<ScissorClip>
  Create(DrawTargetNVpr* aDrawTarget, TemporaryRef<ScissorClip> aPrevious,
         const Matrix& aTransform, const Rect& aClipRect)
  {
    bool success;
    RefPtr<ScissorClip> clip = new ScissorClip(aDrawTarget, aPrevious,
                                               aTransform, aClipRect, success);
    return success ? clip.forget() : nullptr;
  }

  const IntRect& ScissorRect() const { return mScissorRect; }

private:
  ScissorClip(DrawTargetNVpr* aDrawTarget, TemporaryRef<ScissorClip> aPrevious,
              const Matrix& aTransform, const Rect& aClipRect,
              bool& aSuccess);

  IntRect mScissorRect;
};

/**
 * 'Planes clips' are a stack of convex polygons stored in device space. We
 * compute the intersection of all polygons in the stack and then use OpenGL
 * clipping planes to clip to that intersection.
 */
class PlanesClip : public Clip<PlanesClip> {
public:
  static TemporaryRef<PlanesClip>
  Create(DrawTargetNVpr* aDrawTarget, TemporaryRef<PlanesClip> aPrevious,
         const Matrix& aTransform, ConvexPolygon&& aPassPolygon)
  {
    bool success;
    RefPtr<PlanesClip> clip = new PlanesClip(aDrawTarget, aPrevious, aTransform,
                                             aPassPolygon, success);
    return success ? clip.forget() : nullptr;
  }

  const ConvexPolygon& Polygon() const { return mPolygon; }
  UniqueId PolygonId() const { return mPolygonId; }

private:
  PlanesClip(DrawTargetNVpr* aDrawTarget, TemporaryRef<PlanesClip> aPrevious,
             const Matrix& aTransform, ConvexPolygon& aPassPolygon,
             bool& aSuccess);

  ConvexPolygon mPolygon;
  UniqueId mPolygonId;
};

/**
 * A 'stencil clip' etches its path into a bit plane of the stencil buffer. When
 * a stencil bit clip is active, we configure NV_path_rendering to discard
 * samples not in the clip path (samples where the clip bit is not set). When
 * there are two stencil bit clips, each gets its own bit plane, but with three
 * or more they start sharing a clip bit (by etching in just the intersection of
 * paths). That way there are always at least 6 bits left for winding numbers.
 */
class StencilClip : public Clip<StencilClip> {
public:
  static TemporaryRef<StencilClip>
  Create(DrawTargetNVpr* aDrawTarget, TemporaryRef<StencilClip> aPrevious,
         const Matrix& aTransform, UniqueId aTransformId,
         TemporaryRef<PathNVpr> aPath)
  {
    return new StencilClip(aDrawTarget, aPrevious, aTransform, aTransformId, aPath);
  }

  void ApplyToStencilBuffer();

  void RestoreStencilBuffer();

private:
  StencilClip* GetLastClipBitOwner()
  {
    return mOwnClipBit ? this : mPrevious->GetLastClipBitOwner();
  }

  StencilClip(DrawTargetNVpr* aDrawTarget, TemporaryRef<StencilClip> aPrevious,
              const Matrix& aTransform, UniqueId aTransformId,
              TemporaryRef<PathNVpr> aPath)
    : Clip(aDrawTarget, aPrevious)
    , mTransform(aTransform)
    , mTransformId(aTransformId)
    , mPath(aPath)
    , mOwnClipBit(0)
  {}

  const Matrix mTransform;
  const UniqueId mTransformId;
  RefPtr<PathNVpr> mPath;
  GLubyte mOwnClipBit;
};

}
}
}

#endif /* MOZILLA_GFX_NVPR_CLIP_H_ */
