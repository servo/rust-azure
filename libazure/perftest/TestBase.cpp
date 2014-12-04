/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "TestBase.h"

#include <sstream>
#include <fstream>
#include <math.h>

using namespace std;

const int sN = 10;

int
TestBase::RunTests(std::ostream *aCSVOutput)
{
  int testsRun = 0;

  Initialize();

  for(unsigned int i = 0; i < mTests.size(); i++) {
    stringstream stream;
    stream << "Test (" << mTests[i].name << "): ";

    double *data = new double[sN + 1];
    double average = 0;

    for (int c = 0; c < (sN + 1); c++) {
      HighPrecisionMeasurement timer;

      timer.Start();
      // Don't try this at home! We know these are actually pointers to members
      // of child clases, so we reinterpret cast those child class pointers to
      // TestBase and then call the functions. Because the compiler believes
      // these function calls are members of TestBase.
      ((*reinterpret_cast<TestBase*>((mTests[i].implPointer))).*(mTests[i].funcCall))();

      data[c] = timer.Measure();
      if (c > 0) {
        average += data[c];
      }
    }

    average /= sN;

    double sqDiffSum = 0;
    for (int c = 1; c < sN + 1; c++) {
      sqDiffSum += pow(data[c] - average, 2);
    }

    sqDiffSum /= sN;

    if (aCSVOutput) {
      stringstream newData;
      newData << average << ",";
      string str = newData.str();
      aCSVOutput->write(str.c_str(), str.size());
    }

    stream << " " << average << "ms +/- " << (2 * sqrt(sqDiffSum)) << "\n";

    LogMessage(stream.str());

    testsRun++;
  }

  Finalize();

  return testsRun;
}

void
TestBase::LogMessage(string aMessage)
{
  printf("%s", aMessage.c_str());
}
