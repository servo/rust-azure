
#include "2D.h"
#include "RecordedEvent.h"

class RawTranslator : public mozilla::gfx::Translator
{
public:
  static RawTranslator
  *Create(mozilla::gfx::DrawTarget *aBaseDT, bool aRetainDrawTargets,
          bool aRetainPaths, bool aRetainSourceSurfaces, bool aRetainGradientStops);

  void SetEventNumber(uint32_t aEventNumber) { mEventNumber = aEventNumber; }
  virtual mozilla::gfx::DrawTarget *GetReferenceDrawTarget() { return mBaseDT; }
  virtual mozilla::gfx::FontType GetDesiredFontType();

protected:
  RawTranslator(mozilla::gfx::DrawTarget *aBaseDT)
    : mBaseDT(aBaseDT)
  {}
  uint32_t mEventNumber;
  mozilla::RefPtr<mozilla::gfx::DrawTarget> mBaseDT;
};
