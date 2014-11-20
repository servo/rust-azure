/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "TestMatrix.h"

#include "Matrix.h"

using namespace mozilla::gfx;

TestMatrix::TestMatrix()
{
#define TEST_CLASS TestMatrix
  REGISTER_TEST(Multiplication5x4);
#undef TEST_CLASS
}

static Matrix5x4
MakeColorsBlack()
{
  Matrix5x4 result;
  result._11 = result._22 = result._33 = 0;
  return result;
}

static Matrix5x4
TurnRedAllTheWayUp()
{
  Matrix5x4 result;
  result._11 = 0;
  result._51 = 1;
  return result;
}

static Matrix5x4
RotateRedGreenBlue()
{
  Matrix5x4 result;
  result._11 = result._22 = result._33 = 0;
  result._12 = result._23 = result._31 = 1;
  return result;
}

void
TestMatrix::Multiplication5x4()
{
  Matrix5x4 a1 = MakeColorsBlack() * TurnRedAllTheWayUp();
  VERIFY(a1 != MakeColorsBlack());
  VERIFY(a1 != TurnRedAllTheWayUp());
  VERIFY(a1._11 == 0);
  VERIFY(a1._51 == 1);
  VERIFY(a1._44 == 1);

  Matrix5x4 a2 = TurnRedAllTheWayUp() * MakeColorsBlack();
  VERIFY(a2 == MakeColorsBlack());

  Matrix5x4 b1 = TurnRedAllTheWayUp() * RotateRedGreenBlue();
  VERIFY(b1 != TurnRedAllTheWayUp());
  VERIFY(b1 != RotateRedGreenBlue());
  VERIFY(b1._11 + b1._21 + b1._31 + b1._41 == 1);
  VERIFY(b1._12 + b1._22 + b1._32 + b1._42 == 0);
  VERIFY(b1._13 + b1._23 + b1._33 + b1._43 == 1);
  VERIFY(b1._14 + b1._24 + b1._34 + b1._44 == 1);
  VERIFY(b1._51 == 0);
  VERIFY(b1._52 == 1);
  VERIFY(b1._53 == 0);

  Matrix5x4 b2 = RotateRedGreenBlue() * TurnRedAllTheWayUp();
  VERIFY(b2 != RotateRedGreenBlue());
  VERIFY(b2 != TurnRedAllTheWayUp());
  VERIFY(b2._11 + b2._21 + b2._31 + b2._41 == 0);
  VERIFY(b2._12 + b2._22 + b2._32 + b2._42 == 1);
  VERIFY(b2._13 + b2._23 + b2._33 + b2._43 == 1);
  VERIFY(b2._14 + b2._24 + b2._34 + b2._44 == 1);
  VERIFY(b2._51 == 1);
  VERIFY(b2._52 == 0);
  VERIFY(b2._53 == 0);

  Matrix5x4 b3 = RotateRedGreenBlue();
  b3 *= TurnRedAllTheWayUp();
  VERIFY(b2 == b3);
}
