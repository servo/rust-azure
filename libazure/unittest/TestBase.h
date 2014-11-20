/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include <string>
#include <sstream>
#include <vector>

#include "Point.h"

#ifdef _MSC_VER
// On MSVC otherwise our generic member pointer trick doesn't work.
#pragma pointers_to_members(full_generality, single_inheritance)
#endif

#define VERIFY(arg) if (!(arg)) { \
  LogMessage("VERIFY FAILED: "#arg"\n"); \
  mTestFailed = true; \
  }

#define VERIFYVALUE(arg1, arg2) if (!VerifyValue(arg1, arg2, #arg1, #arg2)) { mTestFailed = true; }
#define VERIFYVALUEFUZZY(arg1, arg2, arg3) if (!VerifyValueFuzzy<Float>(arg1, arg2, arg3, #arg1, #arg2)) { mTestFailed = true; }

#define REGISTER_CLASS_TEST(className, testName) \
  mTests.push_back(Test(static_cast<TestCall>(&className::testName), #testName, this))
#define REGISTER_TEST(testName) REGISTER_CLASS_TEST(TEST_CLASS, testName)

class TestBase
{
public:
  TestBase() {}
  virtual ~TestBase() {}

  typedef void (TestBase::*TestCall)();

  int RunTests(int *aFailures);

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
protected:
  static void LogMessage(std::string aMessage);

  std::vector<Test> mTests;

  template<typename T>
  bool VerifyValue(T aVal1, T aVal2, std::string aVal1String, std::string aVal2String)
  {
    std::stringstream msg;

    if (aVal1 != aVal2) {
      msg << "VERIFY FAILED: " << aVal1String << " (" << aVal1 << ") != " << aVal2String << " (" << aVal2 << ")";
      LogMessage(msg.str());
      return false;
    }
    return true;
  }

  bool VerifyValue(mozilla::gfx::Point aVal1, mozilla::gfx::Point aVal2, std::string aVal1String, std::string aVal2String)
  {
    std::stringstream msg;

    if (aVal1 != aVal2) {
      msg << "VERIFY FAILED: " << aVal1String << " (" << aVal1.x << "x" << aVal2.y << ") != " << aVal2String << " (" << aVal2.x << "x" << aVal2.y << ")";
      LogMessage(msg.str());
      return false;
    }
    return true;
  }

  template<typename T>
  bool VerifyValueFuzzy(T aVal1, T aVal2, T aFuzz, std::string aVal1String, std::string aVal2String)
  {
    std::stringstream msg;

    if ((aVal1 < (aVal2 - aFuzz)) || (aVal1 > (aVal2 + aFuzz))) {
      msg << "VERIFY FAILED: " << aVal1String << " (" << aVal1 << ") != " << aVal2String << " (" << aVal2 << ") - Fuzz: " << aFuzz;
      LogMessage(msg.str());
      return false;
    }
    return true;
  }

  bool mTestFailed;

private:
  // This doesn't really work with our generic member pointer trick.
  TestBase(const TestBase &aOther);
};
