/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ScaledFontSkia.h"
#include "Logging.h"

#include "SkTypeface.h"
#include "SkStream.h"

#include <string>

using namespace std;

namespace mozilla {
namespace gfx {

#ifdef USE_SKIA
static SkTypeface::Style
fontStyleToSkia(FontStyle aStyle)
{
  switch (aStyle) {
  case FontStyle::NORMAL:
    return SkTypeface::kNormal;
  case FontStyle::ITALIC:
    return SkTypeface::kItalic;
  case FontStyle::BOLD:
    return SkTypeface::kBold;
  case FontStyle::BOLD_ITALIC:
    return SkTypeface::kBoldItalic;
   }

  gfxWarning() << "Unknown font style";
  return SkTypeface::kNormal;
}
#endif

// Ideally we want to use FT_Face here but as there is currently no way to get
// an SkTypeface from an FT_Face we do this.
ScaledFontSkia::ScaledFontSkia(FontOptions* aFont, Float aSize)
  : ScaledFontBase(aSize)
{
  mTypeface = SkTypeface::CreateFromName(aFont->mName.c_str(), fontStyleToSkia(aFont->mStyle));
}

ScaledFontSkia::ScaledFontSkia(const uint8_t* aData, uint32_t aFileSize, uint32_t aIndex, Float aSize)
  : ScaledFontBase(aSize)
{
  SkStream *stream = new SkMemoryStream(aData, aFileSize, true);
  mTypeface = SkTypeface::CreateFromStream(stream);
}

ScaledFontSkia::~ScaledFontSkia()
{
}

}
}
