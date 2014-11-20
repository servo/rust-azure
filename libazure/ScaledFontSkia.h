/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_SCALEDFONTSKIA_H_
#define MOZILLA_GFX_SCALEDFONTSKIA_H_

#include "ScaledFontBase.h"

namespace mozilla {
namespace gfx {

class ScaledFontSkia : public ScaledFontBase
{
public:

  ScaledFontSkia(FontOptions* aFont, Float aSize);
  ScaledFontSkia(const uint8_t* aData, uint32_t aFileSize, uint32_t aIndex, Float aSize);
  ~ScaledFontSkia();
};

}
}

#endif /* MOZILLA_GFX_SCALEDFONTSKIA_H_ */
