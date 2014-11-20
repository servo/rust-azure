/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "TestDrawTargetSkiaSoftware.h"

using namespace mozilla;
using namespace mozilla::gfx;

using namespace mozilla::gfx;
TestDrawTargetSkiaSoftware::TestDrawTargetSkiaSoftware()
{
  mDT = Factory::CreateDrawTarget(BackendType::SKIA, IntSize(DT_WIDTH, DT_HEIGHT), SurfaceFormat::B8G8R8A8);

  mDT->ClearRect(Rect(0, 0, DT_WIDTH, DT_HEIGHT));
}
