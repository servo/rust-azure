/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include <string>
#include <vector>

#ifdef _MSC_VER
// On MSVC otherwise our generic member pointer trick doesn't work.
#pragma pointers_to_members(full_generality, single_inheritance)
#include <Windows.h>
#else
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#endif

inline void SleepMS(int aMilliseconds)
{
#ifdef _MSC_VER
  ::Sleep(aMilliseconds);
#else
  usleep(1000 * aMilliseconds);
#endif
}

class HighPrecisionMeasurement
{
public:
  void Start() {
#ifdef WIN32
    ::QueryPerformanceCounter(&mStart);
#else
    gettimeofday(&mStart, NULL);
#endif
  }

  double Measure() {
#ifdef WIN32
    LARGE_INTEGER end, freq;
    ::QueryPerformanceCounter(&end);
    ::QueryPerformanceFrequency(&freq);
    return (double(end.QuadPart) - double(mStart.QuadPart)) / double(freq.QuadPart) * 1000.00;
#else
    struct timeval end;
    gettimeofday(&end, NULL);

    long seconds = end.tv_sec - mStart.tv_sec;
    long useconds = end.tv_usec - mStart.tv_usec;
    double mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    return mtime;
#endif
  }
private:
#ifdef WIN32
  LARGE_INTEGER mStart;
#else
  struct timeval mStart;
#endif
};

#define REGISTER_TEST(className, testName) \
  mTests.push_back(Test(static_cast<TestCall>(&className::testName), #testName, this))

enum TestGroup
{
  GROUP_NONE,
  GROUP_DRAWTARGETS,
  GROUP_COUNT
};

class TestBase
{
public:
  TestBase() : mGroup(GROUP_NONE) {}
  virtual ~TestBase() {}

  typedef void (TestBase::*TestCall)();

  virtual void Initialize() {}

  int RunTests(std::ostream *aCSVOutput);

  virtual void Finalize() {}

  TestGroup GetGroup() { return mGroup; }

  struct Test {
    Test(TestCall aCall, std::string aName, void *aImplPointer)
      : funcCall(aCall)
      , name(aName)
      , implPointer(aImplPointer)
    {
    }
    TestCall funcCall;
    std::string name;
    void *implPointer;
  };
  std::vector<Test> mTests;

protected:
  static void LogMessage(std::string aMessage);

  TestGroup mGroup;
private:
  // This doesn't really work with our generic member pointer trick.
  TestBase(const TestBase &aOther);
};
