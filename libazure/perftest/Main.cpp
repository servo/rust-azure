/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SanityChecks.h"
#ifdef WIN32
#include "TestDrawTargetD2D.h"
#include "TestDrawTargetD2DWarp.h"
#endif
#ifdef USE_SKIA
#include "TestDrawTargetSkiaSoftware.h"
#endif
#ifdef USE_CAIRO
#include "TestDrawTargetCairoImage.h"
#endif

#include <string>
#include <sstream>
#include <fstream>

struct TestObject {
  TestBase *test;
  std::string name;
};

std::string sGroupNames[] = { "None", "DrawTargets", "Unknown" };

using namespace std;

int
main()
{
  TestObject tests[] = 
  {
    { new SanityChecks(), "Sanity Checks" },
#ifdef WIN32
    { new TestDrawTargetD2D(), "DrawTarget (D2D)" },
    { new TestDrawTargetD2DWarp(), "DrawTarget (D2D WARP)" },
#endif
#ifdef USE_SKIA
    { new TestDrawTargetSkiaSoftware(), "DrawTarget (Skia Software)" },
#endif
#ifdef USE_CAIRO
    { new TestDrawTargetCairoImage(), "DrawTarget (Cairo Image)" },
#endif
  };

  bool sGroupInitialized[GROUP_COUNT];
  for (int i = 0; i < GROUP_COUNT; i++) {
    sGroupInitialized[i] = false;
  }

  int totalFailures = 0;
  int totalTests = 0;
  stringstream message;
  printf("------ STARTING RUNNING TESTS ------\n");
  for (int i = 0; i < sizeof(tests) / sizeof(TestObject); i++) {
    ofstream fileStream;
    TestGroup group = tests[i].test->GetGroup();
    if (group != GROUP_NONE) {
      ios_base::openmode mode = ios_base::binary;
      if (sGroupInitialized[group]) {
        mode |= ios_base::app;
      }
      std::string fileName = "PerfData";
      fileName.append(sGroupNames[group]);
      fileName.append(".csv");
      fileStream.open(fileName.c_str(), mode);

      if (!sGroupInitialized[group]) {
        fileStream.write(",", 1);
        for(unsigned int c = 0; c < tests[i].test->mTests.size(); c++) {
          fileStream.write(tests[i].test->mTests[c].name.c_str(), tests[i].test->mTests[c].name.size());
          fileStream.write(",", 1);
        }
        fileStream.write("\n", 1);
      }
      fileStream.write(tests[i].name.c_str(), tests[i].name.size());
      fileStream.write(",", 1);
      sGroupInitialized[group] = true;
    }

    message << "--- RUNNING TESTS: " << tests[i].name << " ---\n";
    printf("%s", message.str().c_str());
    message.str("");
    int failures = 0;
    totalTests += tests[i].test->RunTests(group != GROUP_NONE ? &fileStream : nullptr);
    totalFailures += failures;

    if (group != GROUP_NONE) {
      fileStream.write("\n", 1);
    }

    // Done with this test!
    delete tests[i].test;
  }
  message << "------ FINISHED RUNNING TESTS ------\nTests run: " << totalTests << "\n";
  printf("%s", message.str().c_str());
  return totalFailures;
}
