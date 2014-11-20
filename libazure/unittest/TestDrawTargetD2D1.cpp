/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "TestDrawTargetD2D1.h"
#include <d3d11.h>

using namespace mozilla::gfx;
TestDrawTargetD2D1::TestDrawTargetD2D1()
{
  D3D_FEATURE_LEVEL featureLevels[] = {
    D3D_FEATURE_LEVEL_11_1,
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
    D3D_FEATURE_LEVEL_9_3
  };


  HRESULT hr = ::D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL,
                                   D3D11_CREATE_DEVICE_BGRA_SUPPORT,
                                   featureLevels, sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL),
                                   D3D11_SDK_VERSION, byRef(mDevice), nullptr, nullptr);

  Factory::SetDirect3D11Device(mDevice);

  mDT = Factory::CreateDrawTarget(BACKEND_DIRECT2D1_1, IntSize(DT_WIDTH, DT_HEIGHT), FORMAT_B8G8R8A8);
}
