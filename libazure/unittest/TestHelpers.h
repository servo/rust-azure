/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this
  * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define DT_WIDTH 500
#define DT_HEIGHT 500

#define IMPLEMENT_DT_TESTS(backendName, backendValue, className) \
  class className##backendName : public className##Base \
  { \
  public: \
    className##backendName() \
    {  mDT = mozilla::gfx::Factory::CreateDrawTarget(mozilla::gfx::BackendType::backendValue, \
                                                     mozilla::gfx::IntSize(DT_WIDTH, DT_HEIGHT), \
                                                     mozilla::gfx::SurfaceFormat::B8G8R8A8); } \
  }
