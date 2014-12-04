/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include "TestDrawTargetBase.h"

class TestDrawTargetD2D1 : public TestDrawTargetBase
{
public:
  TestDrawTargetD2D1();

private:
  mozilla::RefPtr<ID3D11Device> mDevice;
};
