/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifdef WIN32
#include "Windows.h"
#endif

#include "ScaledFontNVpr.h"
#include <map>
#include <sstream>
#include <fstream>

#define MAX_UNICODE_INDEX 0x10ffff

using namespace mozilla::gfx::nvpr;
using namespace std;

namespace mozilla {
namespace gfx {

bool operator <(const FontOptions& aLeft, const FontOptions& aRight)
{
  if (aLeft.mStyle != aRight.mStyle) {
    return aLeft.mStyle < aRight.mStyle;
  }
  return aLeft.mName < aRight.mName;
}

TemporaryRef<ScaledFontNVpr>
ScaledFontNVpr::Create(const FontOptions* aFont, GLfloat aSize)
{
  struct FontCache
    : public map<FontOptions, RefPtr<FontNVpr> >
    , public nvpr::UserData::Object
  {};

  FontCache& fontCache = gl->GetUserObject<FontCache>(&nvpr::UserData::mFonts);

  RefPtr<FontNVpr>& font = fontCache[*aFont];
  if (!font) {
    font = FontNVpr::Create(aFont);
  }

  return new ScaledFontNVpr(font, aSize);
}

TemporaryRef<ScaledFontNVpr> 
ScaledFontNVpr::Create(const uint8_t* aData, uint32_t aFileSize,
                       uint32_t aIndex, GLfloat aSize)
{
  ostringstream tempFontFile;
#ifdef WIN32
  // XXX - Terrible hacking time! We need a way to get this into a file for
  // the API to be able to accept this.
  CHAR buf [MAX_PATH];
  ::GetTempPathA(MAX_PATH, buf);
  tempFontFile << buf << "nvpr-font-" << rand() << rand() << rand() << rand();
#else
  tempFontFile << "/tmp/nvpr-font-" << rand() << rand() << rand() << rand();
#endif

  ofstream outputFile(tempFontFile.str(), ofstream::binary);
  outputFile.write((const char*)aData, aFileSize);

  FontOptions fontOptions;
  fontOptions.mName = tempFontFile.str();
  fontOptions.mStyle = FontStyle::NORMAL;

  return Create(&fontOptions, aSize);
}

FontNVpr::FontNVpr(const FontOptions* aFont)
{
  GLenum fontStyle;
  switch (aFont->mStyle) {
    default:
      MOZ_ASSERT(!"Invalid font style");
    case FontStyle::NORMAL:
      fontStyle = 0;
      break;
    case FontStyle::ITALIC:
      fontStyle = GL_ITALIC_BIT_NV;
      break;
    case FontStyle::BOLD:
      fontStyle = GL_BOLD_BIT_NV;
      break;
    case FontStyle::BOLD_ITALIC:
      fontStyle = GL_BOLD_BIT_NV | GL_ITALIC_BIT_NV;
      break;
  }

  gl->MakeCurrent();

  GLuint templatePath = gl->GenPathsNV(1);
  gl->PathCommandsNV(templatePath, 0, nullptr, 0, GL_FLOAT, nullptr);

  mGlyphPaths = gl->GenPathsNV(MAX_UNICODE_INDEX + 1);
  gl->PathGlyphRangeNV(mGlyphPaths, GL_FILE_NAME_NV, aFont->mName.c_str(),
                       fontStyle, 0, MAX_UNICODE_INDEX + 1,
                       GL_SKIP_MISSING_GLYPH_NV, templatePath, 1);

  struct {GLfloat x1, y1, x2, y2;} bounds;
  gl->GetPathMetricRangeNV(GL_FONT_X_MIN_BOUNDS_BIT_NV | GL_FONT_Y_MIN_BOUNDS_BIT_NV
                           | GL_FONT_X_MAX_BOUNDS_BIT_NV | GL_FONT_Y_MAX_BOUNDS_BIT_NV,
                           mGlyphPaths, 1, 0, &bounds.x1);

  // HACK! The driver currently seems to return all -1's.
  bounds.x1 = -.25;
  bounds.y1 = -1.25;
  bounds.x2 = 1;
  bounds.y2 = .5;

  mGlyphsBoundingBox = Rect(bounds.x1, bounds.y1,
                            bounds.x2 - bounds.x1, bounds.y2 - bounds.y1);
}

FontNVpr::~FontNVpr()
{
}

ScaledFontNVpr::ScaledFontNVpr(TemporaryRef<FontNVpr> aFont, GLfloat aSize)
  : mFont(aFont)
  , mSize(aSize)
  , mInverseSize(1 / aSize)
{
  mGlyphsBoundingBox = mFont->GlyphsBoundingBox();
  mGlyphsBoundingBox.Scale(aSize, aSize);
}

}
}
