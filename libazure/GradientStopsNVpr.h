/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_GRADIENTSTOPSNVPR_H_
#define MOZILLA_GFX_GRADIENTSTOPSNVPR_H_

#include "2D.h"
#include "nvpr/GL.h"
#include <mozilla/RefPtr.h>

namespace mozilla {
namespace gfx {

class GradientStopsNVpr : public GradientStops {
public:
  static TemporaryRef<GradientStopsNVpr>
  create(GradientStop* aRawStops, uint32_t aNumStops, ExtendMode aExtendMode)
  {
    return new GradientStopsNVpr(aRawStops, aNumStops, aExtendMode);
  }

  ~GradientStopsNVpr();

  virtual BackendType GetBackendType() const { return BackendType::NVPR; }

  const Color& FinalColor() const { return mFinalColor; }
  operator GLuint() const { return mRampTextureId; }

private:
  GradientStopsNVpr(GradientStop* aRawStops, uint32_t aNumStops,
                    ExtendMode aExtendMode);

  GLuint mRampTextureId;
  Color mInitialColor;
  Color mFinalColor;
};

}
}

#endif /* MOZILLA_GFX_GRADIENTSTOPSNVPR_H_ */
