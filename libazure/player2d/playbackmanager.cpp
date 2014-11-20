#include "playbackmanager.h"

#include "timer.h"

#ifdef WIN32
#include <d3d10_1.h>

#endif

using namespace mozilla;
using namespace mozilla::gfx;

PlaybackManager::PlaybackManager()
  : mCurrentEvent(0)
{
}

PlaybackManager::~PlaybackManager()
{
}

DrawTarget*
PlaybackManager::LookupDrawTarget(ReferencePtr aRefPtr)
{
  DTMap::iterator iter = mDrawTargets.find(aRefPtr);

  if (iter != mDrawTargets.end()) {
    return iter->second;
  }

  return NULL;
}

Path*
PlaybackManager::LookupPath(ReferencePtr aRefPtr)
{
  PathMap::iterator iter = mPaths.find(aRefPtr);

  if (iter != mPaths.end()) {
    return iter->second;
  }

  return NULL;
}

FilterNode*
PlaybackManager::LookupFilterNode(ReferencePtr aRefPtr)
{
  FilterNodeMap::iterator iter = mFilterNodes.find(aRefPtr);

  if (iter != mFilterNodes.end()) {
    return iter->second;
  }

  return NULL;
}

SourceSurface*
PlaybackManager::LookupSourceSurface(ReferencePtr aRefPtr)
{
  SourceSurfaceMap::iterator iter = mSourceSurfaces.find(aRefPtr);

  if (iter != mSourceSurfaces.end()) {
    return iter->second;
  }

  return NULL;
}

GradientStops*
PlaybackManager::LookupGradientStops(ReferencePtr aRefPtr)
{
  GradientStopsMap::iterator iter = mGradientStops.find(aRefPtr);

  if (iter != mGradientStops.end()) {
    return iter->second;
  }

  return NULL;
}

ScaledFont*
PlaybackManager::LookupScaledFont(ReferencePtr aRefPtr)
{
  ScaledFontMap::iterator iter = mScaledFonts.find(aRefPtr);

  if (iter != mScaledFonts.end()) {
    return iter->second;
  }

  return NULL;
}

FontType
PlaybackManager::GetDesiredFontType()
{
  switch (mBaseDT->GetType()) {
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

void
PlaybackManager::PlaybackToEvent(int aID)
{
  PlayToEvent(aID);
}

bool
PlaybackManager::IsClipPush(uint32_t aID, int32_t aRefID)
{
  if (aRefID != -1 && mRecordedEvents[aID]->GetObjectRef() != mRecordedEvents[aRefID]->GetObjectRef()) {
    return false;
  }
  return mRecordedEvents[aID]->GetType() == RecordedEvent::PUSHCLIP ||
    mRecordedEvents[aID]->GetType() == RecordedEvent::PUSHCLIPRECT;
}

bool
PlaybackManager::IsClipPop(uint32_t aID, int32_t aRefID)
{
  if (aRefID != -1 && mRecordedEvents[aID]->GetObjectRef() != mRecordedEvents[aRefID]->GetObjectRef()) {
    return false;
  }
  return mRecordedEvents[aID]->GetType() == RecordedEvent::POPCLIP;
}

bool
PlaybackManager::FindCorrespondingClipID(uint32_t aID, uint32_t *aOtherID)
{
  if (IsClipPush(aID)) {
    int32_t clipRecord = 1;
    uint32_t id = aID;

    while (++id < mRecordedEvents.size()) {
      if (IsClipPush(id, aID)) {
        clipRecord++;
      }
      if (IsClipPop(id, aID)) {
        clipRecord--;
      }
      if (!clipRecord) {
        *aOtherID = id;
        return true;
      }
    }
  }
  if (IsClipPop(aID)) {
    int32_t clipRecord = 1;
    uint32_t id = aID;

    while (--id >= 0) {
      if (IsClipPush(id, aID)) {
        clipRecord--;
      }
      if (IsClipPop(id, aID)) {
        clipRecord++;
      }
      if (!clipRecord) {
        *aOtherID = id;
        return true;
      }
    }
  }

  return false;
}

void
PlaybackManager::DisableEvent(uint32_t aID)
{
  mDisabledEvents.insert(aID);
  EventDisablingUpdated(int32_t(aID));

  if (!IsClipPush(aID) && !IsClipPop(aID)) {
    return;
  }

  uint32_t correspondingID;
  if (FindCorrespondingClipID(aID, &correspondingID)) {
    mDisabledEvents.insert(correspondingID);
    EventDisablingUpdated(int32_t(correspondingID));
  }
}

void
PlaybackManager::EnableEvent(uint32_t aID)
{
  mDisabledEvents.erase(aID);
  EventDisablingUpdated(int32_t(aID));
  if (!IsClipPush(aID) && !IsClipPop(aID)) {
    return;
  }

  uint32_t correspondingID;
  if (FindCorrespondingClipID(aID, &correspondingID)) {
    mDisabledEvents.erase(correspondingID);
    EventDisablingUpdated(int32_t(correspondingID));
  }
}

void
PlaybackManager::EnableAllEvents()
{
  mDisabledEvents.clear();
  EventDisablingUpdated(-1);
}

bool
PlaybackManager::IsEventDisabled(uint32_t aID)
{
  return mDisabledEvents.find(aID) != mDisabledEvents.end() && CanDisableEvent(mRecordedEvents[aID]);
}

void
PlaybackManager::PlayToEvent(uint32_t aID)
{
  if (mCurrentEvent > aID) {
    mDrawTargets.clear();
    mSourceSurfaces.clear();
    mPaths.clear();
    mGradientStops.clear();
    mCurrentEvent = 0;
  }
  for (int i = mCurrentEvent; i < aID; i++) {
    if (!IsEventDisabled(i) || !CanDisableEvent(mRecordedEvents[i])) {
      PlaybackEvent(mRecordedEvents[i]);
    }
  }

  mCurrentEvent = aID;
}

void
PlaybackManager::PlaybackEvent(RecordedEvent *aEvent)
{
  aEvent->PlayEvent(this);
}

double
PlaybackManager::GetEventTiming(uint32_t aID, bool aAllowBatching, bool aIgnoreFirst, 
                                bool aDoFlush, bool aForceCompletion, double *aStdDev)
{
  if (!aID) {
    return 0;
  }

  uint32_t currentEvent = mCurrentEvent;

  HighPrecisionMeasurement timer;
  const int sIterations = 50;
  const int N = 10;

  double results[sIterations];
  
  DrawTarget *destinedDT = nullptr;

  // We need to ensure clips still always get pushed and popped in pairs!
  RecordedEvent *popEvent = nullptr;
  RecordedEvent *pushEvent = nullptr;
  if (IsClipPop(aID)) {
    uint32_t pushID;
    if (!FindCorrespondingClipID(aID, &pushID)) {
      // EEP! This is bad.
      MOZ_ASSERT(false);
    }
    pushEvent = mRecordedEvents[pushID];
  } else if (IsClipPush(aID)) {
    uint32_t popID;
    if (!FindCorrespondingClipID(aID, &popID)) {
      // EEP! This is bad.
      MOZ_ASSERT(false);
    }
    popEvent = mRecordedEvents[popID];
  }

  for (int c = 0; c < N; c++) {
    PlayToEvent(0);
    PlayToEvent(aID);

    if (mRecordedEvents[aID]->GetDestinedDT()) {
      destinedDT = LookupDrawTarget(mRecordedEvents[aID]->GetDestinedDT());
    }

    if (aIgnoreFirst) {
      // Execute the call once to ignore the first execution.
      if (pushEvent) {
        PlaybackEvent(pushEvent);
      }

      PlaybackEvent(mRecordedEvents[aID]);

      if (popEvent) {
        PlaybackEvent(popEvent);
      }
    }

    timer.Start();
    for (int i = 0; i < sIterations; i++) {

      if (pushEvent) {
        PlaybackEvent(pushEvent);
      }

      PlaybackEvent(mRecordedEvents[aID]);

      if (popEvent) {
        PlaybackEvent(popEvent);
      }

      if (!aAllowBatching) {
        if (destinedDT) {
          destinedDT->Flush();
        }
      }
    }
    if (destinedDT && aDoFlush) {
      destinedDT->Flush();
    }
    if (aForceCompletion) {
      ForceCompletion();
    }
    results[c] = timer.Measure();
  }

  double average = 0;
  for (int i = 0; i < N; i++) {
    average += results[i];
  }

  // Bjacob suggested we work with the median here instead of the mean,
  // making us less sensitive to outliers. This is probably a good idea and
  // should be looked in to.

  average /= double(N);
  
  double sqDiffSum = 0;
  for (int i = 0; i < N; i++) {
    sqDiffSum += pow(results[i] - average, 2);
  }

  sqDiffSum /= double(N);

  *aStdDev = sqrt(sqDiffSum) / double(sIterations);

  PlayToEvent(0);
  PlayToEvent(currentEvent + 1);  

  return average / double(sIterations);
}

bool
PlaybackManager::CanDisableEvent(RecordedEvent *aEvent)
{
  switch (aEvent->GetType()) {
  case RecordedEvent::CLEARRECT:
  case RecordedEvent::COPYSURFACE:
  case RecordedEvent::DRAWSURFACE:
  case RecordedEvent::DRAWSURFACEWITHSHADOW:
  case RecordedEvent::DRAWFILTER:
  case RecordedEvent::FILL:
  case RecordedEvent::FILLGLYPHS:
  case RecordedEvent::FILLRECT:
  case RecordedEvent::STROKE:
  case RecordedEvent::SETTRANSFORM:
  case RecordedEvent::STROKERECT:
  case RecordedEvent::STROKELINE:
  case RecordedEvent::MASK:
  case RecordedEvent::PUSHCLIP:
  case RecordedEvent::PUSHCLIPRECT:
  case RecordedEvent::POPCLIP:
    return true;
  default:
    return false;
  }
}

void
PlaybackManager::ForceCompletion()
{
#ifdef WIN32
  if (mBaseDT->GetType() == BackendType::DIRECT2D) {
    ID3D10Device1 *device = Factory::GetDirect3D10Device();
    RefPtr<ID3D10Query> query;
    D3D10_QUERY_DESC desc;
    desc.Query = D3D10_QUERY_EVENT;
    desc.MiscFlags = 0;
    device->CreateQuery(&desc, byRef(query));
    query->End();
    while (query->GetData(nullptr, 0, 0) == S_FALSE) {}
  }
#endif
}
