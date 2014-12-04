/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "TestDrawTargetD2DRecording.h"

using namespace mozilla;
using namespace mozilla::gfx;

TestDrawTargetD2DRecording::TestDrawTargetD2DRecording()
{
  RefPtr<DrawEventRecorder> recorder = Factory::CreateEventRecorderForFile("unittestrecording.aer");
  
  RefPtr<DrawTarget> d2dDT = Factory::CreateDrawTarget(BACKEND_DIRECT2D, IntSize(DT_WIDTH, DT_HEIGHT), FORMAT_B8G8R8A8);

  mDT = Factory::CreateRecordingDrawTarget(recorder, d2dDT);
}
