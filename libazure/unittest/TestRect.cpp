/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

#include "TestRect.h"

#include "Rect.h"

using namespace mozilla::gfx;

TestRect::TestRect()
{
#define TEST_CLASS TestRect
  REGISTER_TEST(ClampRect);
#undef TEST_CLASS
}

void
TestRect::ClampRect()
{
  Rect a(2, 5, 10, 20);
  Rect b;
  Rect dest;

  b.SetRect(0, 0, 8, 12);
  dest = a.ForceInside(b);
  VERIFY(dest.IsEqualEdges(Rect(2, 5, 8, 12)));

  b.SetRect(19, 0, 8, 12);
  dest = a.ForceInside(b);
  VERIFY(dest.IsEqualEdges(Rect(4, 5, 8, 12)));

  b.SetRect(-2, 40, 8, 12);
  dest = a.ForceInside(b);
  VERIFY(dest.IsEqualEdges(Rect(2, 13, 8, 12)));

  b.SetRect(22, 80, 8, 12);
  dest = a.ForceInside(b);
  VERIFY(dest.IsEqualEdges(Rect(4, 13, 8, 12)));

  b.SetRect(22, 80, 20, 40);
  dest = a.ForceInside(b);
  VERIFY(dest.IsEqualEdges(Rect(2, 5, 10, 20)));

  b.SetRect(5, 10, 3, 6);
  dest = a.ForceInside(b);
  VERIFY(dest.IsEqualEdges(Rect(5, 10, 3, 6)));
}

