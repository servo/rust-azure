/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "Clip.h"
#include "DrawTargetNVpr.h"
#include "GL.h"
#include "Matrix.h"

namespace mozilla {
namespace gfx {
namespace nvpr {

ScissorClip::ScissorClip(DrawTargetNVpr* aDrawTarget,
                         TemporaryRef<ScissorClip> aPrevious,
                         const Matrix& aTransform,
                         const Rect& aClipRect, bool& aSuccess)
  : Clip(aDrawTarget, aPrevious)
{
  aSuccess = false;

  if (!aTransform.IsRectilinear()) {
    return;
  }

  if (!aTransform.TransformBounds(aClipRect).ToIntRect(&mScissorRect)) {
    return;
  }

  if (mPrevious) {
    mScissorRect.IntersectRect(mScissorRect, mPrevious->mScissorRect);
  }

  aSuccess = true;
}

PlanesClip::PlanesClip(DrawTargetNVpr* aDrawTarget,
                       TemporaryRef<PlanesClip> aPrevious,
                       const Matrix& aTransform,
                       ConvexPolygon& aPolygon, bool& aSuccess)
  : Clip(aDrawTarget, aPrevious)
{
  mPolygon.Swap(aPolygon);

  aSuccess = false;

  mPolygon.Transform(aTransform);
  if (mPrevious) {
    mPolygon.Intersect(mPrevious->Polygon());
  }

  if (mPolygon.NumSides() > gl->MaxClipPlanes()) {
    return;
  }

  mPolygonId = gl->GetUniqueId();

  aSuccess = true;
}

void StencilClip::ApplyToStencilBuffer()
{
  MOZ_ASSERT(!mOwnClipBit);
  MOZ_ASSERT(gl->IsCurrent());

  gl->SetTransform(mTransform, mTransformId);
  gl->SetColorWriteMask(GL::WRITE_NONE);
  gl->SetShaderProgram(0);

  mOwnClipBit = mDrawTarget->ReserveStencilClipBit();
  if (mOwnClipBit) {
    // We own a stencil bit plane for clipping. Now we etch in our path.
    const GLubyte existingClipBits = ~(mOwnClipBit | (mOwnClipBit - 1)) & 0xff;

    gl->ConfigurePathStencilTest(existingClipBits);
    gl->StencilFillPathNV(*mPath, GL_COUNT_UP_NV,
                          (mPath->GetFillRule() == FillRule::FILL_WINDING)
                           ? mOwnClipBit - 1 : 0x1);

    gl->EnableStencilTest(GL::PASS_IF_NOT_EQUAL, mOwnClipBit, mOwnClipBit - 1,
                          GL::REPLACE_PASSING_WITH_COMPARAND,
                          mOwnClipBit | (mOwnClipBit - 1));
    gl->CoverFillPathNV(*mPath, GL_BOUNDING_BOX_NV);

    return;
  }

  // There aren't enough stencil bit planes left for us to get our own. We have
  // to destructively intersect our path into an existing clip bit.
  const StencilClip* lastClipBitOwner = GetLastClipBitOwner();
  MOZ_ASSERT(lastClipBitOwner);

  const GLubyte sharedClipBit = lastClipBitOwner->mOwnClipBit;
  const GLubyte existingClipBits = ~(sharedClipBit - 1) & 0xff;

  gl->ConfigurePathStencilTest(existingClipBits);
  gl->StencilFillPathNV(*mPath, GL_COUNT_UP_NV,
                        (mPath->GetFillRule() == FillRule::FILL_WINDING)
                         ? sharedClipBit - 1 : 0x1);

  gl->SetTransform(lastClipBitOwner->mTransform, lastClipBitOwner->mTransformId);

  gl->EnableStencilTest(GL::PASS_IF_NOT_EQUAL, sharedClipBit, sharedClipBit - 1,
                        GL::REPLACE_PASSING_CLEAR_FAILING,
                        sharedClipBit | (sharedClipBit - 1));
  gl->CoverFillPathNV(*lastClipBitOwner->mPath, GL_BOUNDING_BOX_NV);
}

void StencilClip::RestoreStencilBuffer()
{
  if (!mOwnClipBit) {
    // We destroyed the previous clip state when we intersected our path into an
    // existing clip bit in the stencil buffer. We have to clear that bit plane
    // and then etch the previous path(s) back into it again.
    MOZ_ASSERT(mPrevious);
    mPrevious->RestoreStencilBuffer();
    mPrevious->ApplyToStencilBuffer();
    return;
  }

  MOZ_ASSERT(gl->IsCurrent());

  gl->SetTransform(mTransform, mTransformId);
  gl->SetColorWriteMask(GL::WRITE_NONE);
  gl->SetShaderProgram(0);

  // In order to reset the stencil buffer to the previous clipping state, we
  // need to clear our bit plane as well as any stencil data from future clips.
  const GLuint newFreeBits = mOwnClipBit | (mOwnClipBit - 1);
  gl->EnableStencilTest(GL::PASS_IF_NOT_ZERO, newFreeBits,
                        GL::CLEAR_PASSING_VALUES, newFreeBits);
  gl->CoverFillPathNV(*mPath, GL_BOUNDING_BOX_NV);

  mDrawTarget->ReleaseStencilClipBits(newFreeBits);
  mOwnClipBit = 0;
}

}
}
}
