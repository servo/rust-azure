#ifndef PLAYBACKMANAGER_H
#define PLAYBACKMANAGER_H

#include <qobject.h>
#include "RecordedEvent.h"
#include "Filters.h"

#include <vector>
#ifdef __GNUC__
#include <ext/hash_set>
using __gnu_cxx::hash_set;
namespace __gnu_cxx {
#define DEFINE_TRIVIAL_HASH(integral_type) \
    template<> \
    struct hash<integral_type> { \
      std::size_t operator()(integral_type value) const { \
        return (std::size_t)(value); \
      } \
    }
DEFINE_TRIVIAL_HASH(void*);
}
#else
#include <hash_set>
using std::hash_set;
#endif
#include <map>

class PlaybackManager : public QObject, public mozilla::gfx::Translator
{
  Q_OBJECT
public:
  PlaybackManager();
  ~PlaybackManager();
  
  typedef mozilla::gfx::DrawTarget DrawTarget;
  typedef mozilla::gfx::Path Path;
  typedef mozilla::gfx::SourceSurface SourceSurface;
  typedef mozilla::gfx::FilterNode FilterNode;
  typedef mozilla::gfx::GradientStops GradientStops;
  typedef mozilla::gfx::ScaledFont ScaledFont;

  // Translator
  virtual DrawTarget *LookupDrawTarget(mozilla::gfx::ReferencePtr aRefPtr);
  virtual Path *LookupPath(mozilla::gfx::ReferencePtr aRefPtr);
  virtual SourceSurface *LookupSourceSurface(mozilla::gfx::ReferencePtr aRefPtr);
  virtual FilterNode *LookupFilterNode(mozilla::gfx::ReferencePtr aRefPtr);
  virtual GradientStops *LookupGradientStops(mozilla::gfx::ReferencePtr aRefPtr);
  virtual ScaledFont *LookupScaledFont(mozilla::gfx::ReferencePtr aRefPtr);
  virtual DrawTarget *GetReferenceDrawTarget() { return mBaseDT; }
  virtual mozilla::gfx::FontType GetDesiredFontType();
  virtual void AddDrawTarget(mozilla::gfx::ReferencePtr aRefPtr, DrawTarget *aDT) { mDrawTargets[aRefPtr] = aDT; }
  virtual void RemoveDrawTarget(mozilla::gfx::ReferencePtr aRefPtr) { mDrawTargets.erase(aRefPtr); }
  virtual void AddPath(mozilla::gfx::ReferencePtr aRefPtr, Path *aPath) { mPaths[aRefPtr] = aPath; }
  virtual void AddSourceSurface(mozilla::gfx::ReferencePtr aRefPtr, SourceSurface *aSurface) { mSourceSurfaces[aRefPtr] = aSurface; }
  virtual void RemoveSourceSurface(mozilla::gfx::ReferencePtr aRefPtr) { mSourceSurfaces.erase(aRefPtr); }
  virtual void RemovePath(mozilla::gfx::ReferencePtr aRefPtr) { mPaths.erase(aRefPtr); }
  virtual void AddGradientStops(mozilla::gfx::ReferencePtr aRefPtr, GradientStops *aStops) { mGradientStops[aRefPtr] = aStops; }
  virtual void RemoveGradientStops(mozilla::gfx::ReferencePtr aRefPtr) { mGradientStops.erase(aRefPtr); }
  virtual void AddScaledFont(mozilla::gfx::ReferencePtr aRefPtr, ScaledFont *aStops) { mScaledFonts[aRefPtr] = aStops; }
  virtual void RemoveScaledFont(mozilla::gfx::ReferencePtr aRefPtr) { mScaledFonts.erase(aRefPtr); }
  virtual void AddFilterNode(mozilla::gfx::ReferencePtr aRefPtr, FilterNode *aNode) { mFilterNodes[aRefPtr] = aNode; }
  virtual void RemoveFilterNode(mozilla::gfx::ReferencePtr aRefPtr) { mFilterNodes.erase(aRefPtr); }


  void SetBaseDT(DrawTarget *aBaseDT) { mBaseDT = aBaseDT; }
  void AddEvent(mozilla::gfx::RecordedEvent *aEvent) { mRecordedEvents.push_back(aEvent); }

  void PlaybackToEvent(int aID);

  void DisableEvent(uint32_t aID);
  void EnableEvent(uint32_t aID);
  void EnableAllEvents();
  bool IsEventDisabled(uint32_t aID);
  double GetEventTiming(uint32_t aID, bool aAllowBatching, bool aIgnoreFirst,
                        bool aDoFlush, bool aForceCompletion, double *aStdDev);

  uint32_t GetCurrentEvent() { return mCurrentEvent; }

  typedef std::map<void*, mozilla::RefPtr<DrawTarget> > DTMap;
  typedef std::map<void*, mozilla::RefPtr<Path> > PathMap;
  typedef std::map<void*, mozilla::RefPtr<SourceSurface> > SourceSurfaceMap;
  typedef std::map<void*, mozilla::RefPtr<GradientStops> > GradientStopsMap;
  typedef std::map<void*, mozilla::RefPtr<ScaledFont> > ScaledFontMap;
  typedef std::map<void*, mozilla::RefPtr<FilterNode> > FilterNodeMap;

  DTMap mDrawTargets;
  PathMap mPaths;
  SourceSurfaceMap mSourceSurfaces;
  GradientStopsMap mGradientStops;
  ScaledFontMap mScaledFonts;
  FilterNodeMap mFilterNodes;
  std::vector<mozilla::gfx::RecordedEvent*> mRecordedEvents;
  hash_set<uint32_t> mDisabledEvents;
signals:
  void EventDisablingUpdated(int32_t aID);
private:
  friend class PlaybackTranslator;

  bool IsClipPush(uint32_t aID, int32_t aRefID = -1);
  bool IsClipPop(uint32_t aID, int32_t aRefID = -1);
  bool FindCorrespondingClipID(uint32_t aID, uint32_t *aOtherID);

  void PlayToEvent(uint32_t aID);
  void PlaybackEvent(mozilla::gfx::RecordedEvent *aEvent);

  bool CanDisableEvent(mozilla::gfx::RecordedEvent *aEvent);

  void ForceCompletion();

  uint32_t mCurrentEvent;
  mozilla::RefPtr<mozilla::gfx::DrawTarget> mBaseDT;
};

#endif // PLAYBACKMANAGER_H
