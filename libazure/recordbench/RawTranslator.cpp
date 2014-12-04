#include "RawTranslator.h"
#include "Filters.h"
#include <map>
#include <vector>

using namespace mozilla;
using namespace mozilla::gfx;
using namespace std;

enum RetainBehavior { RETAIN, NO_RETAIN };

template<RetainBehavior RetainDrawTargets, RetainBehavior RetainPaths,
         RetainBehavior RetainSourceSurfaces, RetainBehavior RetainGradientStops>
class RawTranslatorImpl : public RawTranslator
{
public:
  RawTranslatorImpl(DrawTarget *aBaseDT)
    : RawTranslator(aBaseDT)
  {}

private:
  template<typename T> inline T
  *LookupObject(map<void*, RefPtr<T> > &aMap, ReferencePtr aRefPtr)
  {
    typedef typename std::map<void*, RefPtr<T> > MapType;
    typename MapType::iterator iter = aMap.find(aRefPtr);
    return (iter != aMap.end()) ? iter->second : NULL;
  }

  template<typename T> inline void
  AddObject(map<void*, RefPtr<T> > &aMap, ReferencePtr aRefPtr, T *aObject)
  {
    aMap[aRefPtr] = aObject;
  }

  template<typename T> inline void
  RemoveObject(map<void*, RefPtr<T> > &aMap, ReferencePtr aRefPtr)
  {
    aMap.erase(aRefPtr);
  }

  template<typename T> struct RetainedObject {
    uint32_t startEvent;
    RefPtr<T> object;
  };

  template<typename T> inline RefPtr<T>
  &LookupObject(map<void*, vector<RetainedObject<T> > > &aMap, ReferencePtr aRefPtr)
  {
    typedef typename std::map<void*, vector<RetainedObject<T> > > MapType;
    typename MapType::iterator iter = aMap.find(aRefPtr);
    if (iter == aMap.end()) {
      printf("Attempted to look up a nonexistent object. Aborting.\n");
      exit(-1);
    }

    vector<RetainedObject<T> > &retainedObjects = iter->second;
    if (retainedObjects.front().startEvent > mEventNumber) {
      printf("Attempted to look up a nonexistent object. Aborting.\n");
      exit(-1);
    }

    size_t left = 0, right = retainedObjects.size();
    while (left + 1 != right) {
      size_t mid = (left + right) / 2;
      if (retainedObjects[mid].startEvent <= mEventNumber) {
        left = mid;
      } else {
        right = mid;
      }
    }

    return retainedObjects[left].object;
  }

  template<typename T> inline void
  AddObject(map<void*, vector<RetainedObject<T> > > &aMap, ReferencePtr aRefPtr, T *aObject)
  {
    RetainedObject<T> retainedObject = {mEventNumber, aObject};
    vector<RetainedObject<T> > &retainedObjects = aMap[aRefPtr];
    if (!retainedObjects.empty() &&
        retainedObjects.back().startEvent >= retainedObject.startEvent) {
      // Snapshots still get added every frame.
      RefPtr<T> &object = LookupObject(aMap, aRefPtr);
      object = aObject;
      return;
    }
    retainedObjects.push_back(retainedObject);
  }

  template<typename T> inline void
  RemoveObject(map<void*, vector<RetainedObject<T> > > &aMap, ReferencePtr aRefPtr)
  {
    RefPtr<T> &object = LookupObject(aMap, aRefPtr);
    object = nullptr;
  }

  // Translator
  virtual DrawTarget *LookupDrawTarget(ReferencePtr aRefPtr) { return LookupObject(mDrawTargets, aRefPtr); }
  virtual Path *LookupPath(ReferencePtr aRefPtr) { return LookupObject(mPaths, aRefPtr); }
  virtual SourceSurface *LookupSourceSurface(ReferencePtr aRefPtr) { return LookupObject(mSourceSurfaces, aRefPtr); }
  virtual FilterNode *LookupFilterNode(ReferencePtr aRefPtr) { return LookupObject(mFilterNodes, aRefPtr); }
  virtual GradientStops *LookupGradientStops(ReferencePtr aRefPtr) { return LookupObject(mGradientStops, aRefPtr); }
  virtual ScaledFont *LookupScaledFont(ReferencePtr aRefPtr) { return LookupObject(mScaledFonts, aRefPtr); }
  virtual void AddDrawTarget(ReferencePtr aRefPtr, DrawTarget *aDT) { AddObject(mDrawTargets, aRefPtr, aDT); }
  virtual void AddPath(ReferencePtr aRefPtr, Path *aPath) { AddObject(mPaths, aRefPtr, aPath); }
  virtual void AddSourceSurface(ReferencePtr aRefPtr, SourceSurface *aSurface) { AddObject(mSourceSurfaces, aRefPtr, aSurface); }
  virtual void AddFilterNode(ReferencePtr aRefPtr, FilterNode *aFilter) { AddObject(mFilterNodes, aRefPtr, aFilter); }
  virtual void AddGradientStops(ReferencePtr aRefPtr, GradientStops *aStops) { AddObject(mGradientStops, aRefPtr, aStops); }
  virtual void AddScaledFont(ReferencePtr aRefPtr, ScaledFont *aScaledFont) { AddObject(mScaledFonts, aRefPtr, aScaledFont); }
  virtual void RemoveDrawTarget(ReferencePtr aRefPtr) { RemoveObject(mDrawTargets, aRefPtr); }
  virtual void RemovePath(ReferencePtr aRefPtr) { RemoveObject(mPaths, aRefPtr); }
  virtual void RemoveSourceSurface(ReferencePtr aRefPtr) { RemoveObject(mSourceSurfaces, aRefPtr); }
  virtual void RemoveFilterNode(ReferencePtr aRefPtr) { RemoveObject(mFilterNodes, aRefPtr); }
  virtual void RemoveGradientStops(ReferencePtr aRefPtr) { RemoveObject(mGradientStops, aRefPtr); }
  virtual void RemoveScaledFont(ReferencePtr aRefPtr) { RemoveObject(mScaledFonts, aRefPtr); }

private:
  template<typename T, RetainBehavior> struct MapType {
    typedef map<void*, RefPtr<T> > Type;
  };
  template<typename T> struct MapType<T, RETAIN> {
    typedef map<void*, vector<RetainedObject<T> > > Type;
  };
  typename MapType<DrawTarget, RetainDrawTargets>::Type mDrawTargets;
  typename MapType<Path, RetainPaths>::Type mPaths;
  typename MapType<SourceSurface, RetainSourceSurfaces>::Type mSourceSurfaces;
  typename MapType<FilterNode, RETAIN>::Type mFilterNodes;
  typename MapType<GradientStops, RetainGradientStops>::Type mGradientStops;
  typename MapType<ScaledFont, RETAIN>::Type mScaledFonts;
};

RawTranslator
*RawTranslator::Create(DrawTarget *aBaseDT, bool aRetainDrawTargets, bool aRetainPaths,
                       bool aRetainSourceSurfaces, bool aRetainGradientStops)
{
  if (!aRetainDrawTargets && !aRetainPaths && !aRetainSourceSurfaces && !aRetainGradientStops) {
    return new RawTranslatorImpl<NO_RETAIN, NO_RETAIN, NO_RETAIN, NO_RETAIN>(aBaseDT);
  }
  if (!aRetainDrawTargets && !aRetainPaths && !aRetainSourceSurfaces && aRetainGradientStops) {
    return new RawTranslatorImpl<NO_RETAIN, NO_RETAIN, NO_RETAIN, RETAIN>(aBaseDT);
  }
  if (!aRetainDrawTargets && !aRetainPaths && aRetainSourceSurfaces && !aRetainGradientStops) {
    return new RawTranslatorImpl<NO_RETAIN, NO_RETAIN, RETAIN, NO_RETAIN>(aBaseDT);
  }
  if (!aRetainDrawTargets && !aRetainPaths && aRetainSourceSurfaces && aRetainGradientStops) {
    return new RawTranslatorImpl<NO_RETAIN, NO_RETAIN, RETAIN, RETAIN>(aBaseDT);
  }
  if (!aRetainDrawTargets && aRetainPaths && !aRetainSourceSurfaces && !aRetainGradientStops) {
    return new RawTranslatorImpl<NO_RETAIN, RETAIN, NO_RETAIN, NO_RETAIN>(aBaseDT);
  }
  if (!aRetainDrawTargets && aRetainPaths && !aRetainSourceSurfaces && aRetainGradientStops) {
    return new RawTranslatorImpl<NO_RETAIN, RETAIN, NO_RETAIN, RETAIN>(aBaseDT);
  }
  if (!aRetainDrawTargets && aRetainPaths && aRetainSourceSurfaces && !aRetainGradientStops) {
    return new RawTranslatorImpl<NO_RETAIN, RETAIN, RETAIN, NO_RETAIN>(aBaseDT);
  }
  if (!aRetainDrawTargets && aRetainPaths && aRetainSourceSurfaces && aRetainGradientStops) {
    return new RawTranslatorImpl<NO_RETAIN, RETAIN, RETAIN, RETAIN>(aBaseDT);
  }
  if (aRetainDrawTargets && !aRetainPaths && !aRetainSourceSurfaces && !aRetainGradientStops) {
    return new RawTranslatorImpl<RETAIN, NO_RETAIN, NO_RETAIN, NO_RETAIN>(aBaseDT);
  }
  if (aRetainDrawTargets && !aRetainPaths && !aRetainSourceSurfaces && aRetainGradientStops) {
    return new RawTranslatorImpl<RETAIN, NO_RETAIN, NO_RETAIN, RETAIN>(aBaseDT);
  }
  if (aRetainDrawTargets && !aRetainPaths && aRetainSourceSurfaces && !aRetainGradientStops) {
    return new RawTranslatorImpl<RETAIN, NO_RETAIN, RETAIN, NO_RETAIN>(aBaseDT);
  }
  if (aRetainDrawTargets && !aRetainPaths && aRetainSourceSurfaces && aRetainGradientStops) {
    return new RawTranslatorImpl<RETAIN, NO_RETAIN, RETAIN, RETAIN>(aBaseDT);
  }
  if (aRetainDrawTargets && aRetainPaths && !aRetainSourceSurfaces && !aRetainGradientStops) {
    return new RawTranslatorImpl<RETAIN, RETAIN, NO_RETAIN, NO_RETAIN>(aBaseDT);
  }
  if (aRetainDrawTargets && aRetainPaths && !aRetainSourceSurfaces && aRetainGradientStops) {
    return new RawTranslatorImpl<RETAIN, RETAIN, NO_RETAIN, RETAIN>(aBaseDT);
  }
  if (aRetainDrawTargets && aRetainPaths && aRetainSourceSurfaces && !aRetainGradientStops) {
    return new RawTranslatorImpl<RETAIN, RETAIN, RETAIN, NO_RETAIN>(aBaseDT);
  }
  if (aRetainDrawTargets && aRetainPaths && aRetainSourceSurfaces && aRetainGradientStops) {
    return new RawTranslatorImpl<RETAIN, RETAIN, RETAIN, RETAIN>(aBaseDT);
  }
  return NULL;
}

FontType
RawTranslator::GetDesiredFontType()
{
  switch (mBaseDT->GetBackendType()) {
    case BackendType::DIRECT2D:
      return FontType::DWRITE;
    case BackendType::CAIRO:
      return FontType::CAIRO;
    case BackendType::SKIA:
      return FontType::SKIA;
    case BackendType::NVPR:
      return FontType::NVPR;
    default:
      MOZ_ASSERT(false);
      return FontType::DWRITE;
  }
}
