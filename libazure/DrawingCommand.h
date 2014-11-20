/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_DRAWINGCOMMAND_H_
#define MOZILLA_GFX_DRAWINGCOMMAND_H_

#include "2D.h"

namespace mozilla {
namespace gfx {

class DrawingCommand
{
public:
  virtual ~DrawingCommand() { }

  virtual void ReplayToDT(DrawTarget *aDT) = 0;
};

class DrawingCommandPattern
{
public:
  DrawingCommandPattern(const Pattern& aPattern)
  {
    mType = aPattern.GetType();

    switch (mType) {
    case PatternType::COLOR:
      new (mColorPattern.addr()) ColorPattern(static_cast<const ColorPattern&>(aPattern));
      break;
    case PatternType::SURFACE:
      new (mSurfacePattern.addr()) SurfacePattern(static_cast<const SurfacePattern&>(aPattern));
      break;
    case PatternType::LINEAR_GRADIENT:
      new (mLinearGradientPattern.addr()) LinearGradientPattern(static_cast<const LinearGradientPattern&>(aPattern));
      break;
    case PatternType::RADIAL_GRADIENT:
      new (mRadialGradientPattern.addr()) RadialGradientPattern(static_cast<const RadialGradientPattern&>(aPattern));
      break;
    }
  }

  ~DrawingCommandPattern()
  {
    mPattern.addr()->~Pattern();
  }

  operator Pattern& ()
  {
    return *mPattern.addr();
  }

private:
  PatternType mType;
  union {
    ClassStorage<Pattern> mPattern;
    ClassStorage<ColorPattern> mColorPattern;
    ClassStorage<SurfacePattern> mSurfacePattern;
    ClassStorage<LinearGradientPattern> mLinearGradientPattern;
    ClassStorage<RadialGradientPattern> mRadialGradientPattern;
  };
};

class DrawSurfaceCommand : public DrawingCommand
{
public:
  DrawSurfaceCommand(SourceSurface* aSurface,
                     const Rect& aDest,
                     const Rect& aSource,
                     const DrawSurfaceOptions& aSurfOptions = DrawSurfaceOptions(),
                     const DrawOptions& aOptions = DrawOptions())
    : mSurface(aSurface)
    , mDest(aDest)
    , mSource(aSource)
    , mSurfOptions(aSurfOptions)
    , mOptions(aOptions)
  {
  }

  void ReplayToDT(DrawTarget* aDT)
  {
    aDT->DrawSurface(mSurface, mDest, mSource, mSurfOptions, mOptions);
  }

private:
  RefPtr<SourceSurface> mSurface;
  Rect mDest;
  Rect mSource;
  DrawSurfaceOptions mSurfOptions;
  DrawOptions mOptions;
};

class ClearRectCommand : public DrawingCommand
{
public:
  ClearRectCommand(const Rect &aRect)
    : mRect(aRect)
  { }

  void ReplayToDT(DrawTarget *aDT)
  {
    aDT->ClearRect(mRect);
  }
private:
  Rect mRect;
};

class CopySurfaceCommand : public DrawingCommand
{
public:
  CopySurfaceCommand(SourceSurface *aSurface,
                     const IntRect &aSourceRect,
                     const IntPoint &aDestination)
    : mSurface(aSurface)
    , mSourceRect(aSourceRect)
    , mDestination(aDestination)
  {
  }

  void ReplayToDT(DrawTarget* aDT)
  {
    aDT->CopySurface(mSurface, mSourceRect, mDestination);
  }

private:
  RefPtr<SourceSurface> mSurface;
  IntRect mSourceRect;
  IntPoint mDestination;
};

class FillRectCommand : public DrawingCommand
{
public:
  FillRectCommand(const Rect &aRect,
                  const Pattern &aPattern,
                  const DrawOptions &aOptions)
    : mRect(aRect)
    , mPattern(aPattern)
    , mOptions(aOptions)
  {
  }

  void ReplayToDT(DrawTarget* aDT)
  {
    aDT->FillRect(mRect, mPattern, mOptions);
  }

private:
  Rect mRect;
  DrawingCommandPattern mPattern;
  DrawOptions mOptions;
};

}
}

#endif
