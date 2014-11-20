/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SanityChecks.h"
#include "TestPoint.h"
#include "TestRect.h"
#include "TestMatrix.h"
#include "TestScaling.h"
#include "TestBugs.h"
#ifdef WIN32
#include <d3d10_1.h>
#ifdef USE_D2D1_1
#include <d3d11.h>
#endif
#endif
#include "TestDrawTarget.h"
#include "TestPath.h"

#include <string>
#include <sstream>

struct TestObject {
  TestBase *test;
  std::string name;
};


using namespace std;
using namespace mozilla;
using namespace mozilla::gfx;

int
main()
{
#ifdef WIN32
  RefPtr<ID3D10Device1> mDevice;
  ::D3D10CreateDevice1(nullptr,
                       D3D10_DRIVER_TYPE_HARDWARE,
                       nullptr,
                       D3D10_CREATE_DEVICE_BGRA_SUPPORT |
                       D3D10_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS,
                       D3D10_FEATURE_LEVEL_10_0,
                       D3D10_1_SDK_VERSION,
                       byRef(mDevice));

  Factory::SetDirect3D10Device(mDevice);

#ifdef USE_D2D1_1
  RefPtr<ID3D11Device> d3d11device;
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
                                   D3D11_SDK_VERSION, byRef(d3d11device), nullptr, nullptr);

  Factory::SetDirect3D11Device(d3d11device);
#endif
#endif

  TestObject tests[] = 
  {
    { new SanityChecks(), "Sanity Checks" },
  #ifdef WIN32
    { new TestDrawTargetD2D(), "DrawTarget (D2D)" },
    { new TestPathD2D(), "Path (D2D)" },
    { new TestDrawTargetCapture(), "DrawTarget (Capture)" },
  #ifdef USE_D2D1_1
    { new  TestDrawTargetD2D1(), "DrawTarget (D2D 1.1)" },
  #endif
  #endif
  #ifdef USE_CAIRO
    { new TestDrawTargetCairoImage(), "DrawTarget (Cairo Image)" },
    { new TestPathCairo(), "Path (Cairo)" },
  #endif
  #ifdef USE_SKIA
    { new TestDrawTargetSkiaSoftware(), "DrawTarget (Skia Software)" },
    { new TestPathSkia(), "Path (Skia)" },
  #endif
    { new TestPoint(), "Point Tests" },
    { new TestRect(), "Rect Tests" },
    { new TestMatrix(), "Matrix Tests" },
    { new TestScaling(), "Scaling Tests" },
    { new TestBugs(), "Bug Tests" }
  };

  int totalFailures = 0;
  int totalTests = 0;
  stringstream message;
  printf("------ STARTING RUNNING TESTS ------\n");
  for (int i = 0; i < sizeof(tests) / sizeof(TestObject); i++) {
    message << "--- RUNNING TESTS: " << tests[i].name << " ---\n";
    printf("%s", message.str().c_str());
    message.str("");
    int failures = 0;
    totalTests += tests[i].test->RunTests(&failures);
    totalFailures += failures;
    // Done with this test!
    delete tests[i].test;
  }
  message << "------ FINISHED RUNNING TESTS ------\nTests run: " << totalTests << " - Passes: " << totalTests - totalFailures << " - Failures: " << totalFailures << "\n";
  printf("%s", message.str().c_str());
  return totalFailures;
}
