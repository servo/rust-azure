// recordbench.cpp : Defines the entry point for the console application.
//

#include <fstream>
#ifdef WIN32
#include <d3d10_1.h>
#endif
#if USE_NVPR
#include "nvpr/GL.h"
#endif
#include "2D.h"
#include "RecordedEvent.h"
#include "RawTranslator.h"
#include "perftest/TestBase.h"

#include <map>
#include <string>

using namespace mozilla;
using namespace mozilla::gfx;
using namespace std;

BackendType sTestedBackends[] =
{
#ifdef WIN32
  BackendType::DIRECT2D,
#endif
#ifdef USE_NVPR
  BackendType::NVPR,
#endif
#ifdef USE_SKIA
  BackendType::SKIA,
#endif
#ifdef USE_CAIRO
  BackendType::CAIRO,
#endif
};

string
GetBackendName(BackendType aType)
{
  switch(aType) {
  case BackendType::DIRECT2D:
    return "Direct2D";
  case BackendType::SKIA:
    return "Skia";
  case BackendType::CAIRO:
    return "Cairo";
  case BackendType::NVPR:
    return "NVpr";
  default:
    return "Unknown";
  }
}

void
FinishDrawing(DrawTarget* aDT)
{
#ifdef WIN32
  if (aDT->GetBackendType() == BackendType::DIRECT2D) {
    aDT->Flush();

    RefPtr<ID3D10Query> query;
    D3D10_QUERY_DESC desc;
    desc.Query = D3D10_QUERY_EVENT;
    desc.MiscFlags = 0;
    Factory::GetDirect3D10Device()->CreateQuery(&desc, byRef(query));
    query->End();
    while (query->GetData(nullptr, 0, 0) == S_FALSE) {}
    return;
  }
#endif
#if USE_NVPR
  if (aDT->GetBackendType() == BackendType::NVPR) {
    using nvpr::gl;
    gl->MakeCurrent();
    gl->Finish();
    return;
  }
#endif
}

struct RetainedDrawTargetData {
  RetainedDrawTargetData(int aCreationEvent)
    : mCreationEvent(aCreationEvent)
    , mEndClipDepth(0)
  {}
  int mCreationEvent;
  int mEndClipDepth;
};

static int sN = 10;
static bool sRetainDrawTargets;
static bool sRetainPaths;
static bool sRetainSourceSurfaces;
static bool sRetainGradientStops;

int
main(int argc, char *argv[], char *envp[])
{
  if (argc < 2) {
    printf("No recording specified.");
    return 1;
  }

  for (int i = 1; i < argc - 1; i++) {
    if (sscanf(argv[i], "--n=%i", &sN)) {
      continue;
    }
    if (!strcmp(argv[i], "--retain-draw-targets")) {
      sRetainDrawTargets = true;
      continue;
    }
    if (!strcmp(argv[i], "--retain-paths")) {
      sRetainPaths = true;
      continue;
    }
    if (!strcmp(argv[i], "--retain-source-surfaces")) {
      sRetainSourceSurfaces = true;
      continue;
    }
    if (!strcmp(argv[i], "--retain-gradient-stops")) {
      sRetainGradientStops = true;
      continue;
    }
  }

  struct EventWithID {
    RecordedEvent* recordedEvent;
    uint32_t eventID;
  };

  vector<EventWithID > drawingEvents;
  vector<EventWithID > retainedObjectCreations;
  map<ReferencePtr, vector<RetainedDrawTargetData> > retainedDrawTargets;

  ifstream inputFile;

  inputFile.open(argv[argc - 1], istream::in | istream::binary);

  inputFile.seekg(0, ios::end);
  int length = inputFile.tellg();
  inputFile.seekg(0, ios::beg);

  uint32_t magicInt;
  ReadElement(inputFile, magicInt);
  if (magicInt != 0xc001feed) {
    printf("File is not a valid recording");
    return 1;
  }

  uint16_t majorRevision;
  uint16_t minorRevision;
  ReadElement(inputFile, majorRevision);
  ReadElement(inputFile, minorRevision);

  if (majorRevision != kMajorRevision) {
    printf("Recording was made with a different major revision");
    return 1;
  }

  if (minorRevision > kMinorRevision) {
    printf("Recording was made with a later minor revision");
    return 1;
  }

  uint32_t eventIndex = 0;
  while (inputFile.tellg() < length) {
    int32_t type;
    ReadElement(inputFile, type);

    EventWithID newEvent;
    newEvent.recordedEvent = RecordedEvent::LoadEventFromStream(inputFile, (RecordedEvent::EventType)type);
    newEvent.eventID = eventIndex++;

    RecordedEvent::EventType eventType = newEvent.recordedEvent->GetType();

    if ((sRetainDrawTargets && eventType == RecordedEvent::DRAWTARGETCREATION) ||
        (sRetainPaths && eventType == RecordedEvent::PATHCREATION) ||
        (sRetainSourceSurfaces && eventType == RecordedEvent::SOURCESURFACECREATION) ||
        (sRetainGradientStops && eventType == RecordedEvent::GRADIENTSTOPSCREATION) ||
        (eventType == RecordedEvent::SCALEDFONTCREATION)) {

      retainedObjectCreations.push_back(newEvent);

      if (eventType == RecordedEvent::DRAWTARGETCREATION) {
        ReferencePtr drawTargetId = newEvent.recordedEvent->GetObjectRef();
        RetainedDrawTargetData data(newEvent.eventID);
        retainedDrawTargets[drawTargetId].push_back(data);
      }

      continue;
    }

    if ((sRetainDrawTargets && eventType == RecordedEvent::DRAWTARGETDESTRUCTION) ||
        (sRetainPaths && eventType == RecordedEvent::PATHDESTRUCTION) ||
        (sRetainSourceSurfaces && eventType == RecordedEvent::SOURCESURFACEDESTRUCTION) ||
        (sRetainGradientStops && eventType == RecordedEvent::GRADIENTSTOPSDESTRUCTION) ||
        (eventType == RecordedEvent::SCALEDFONTDESTRUCTION)) {
      // Retained objects never get destroyed.
      continue;
    }

    if (sRetainDrawTargets && (eventType == RecordedEvent::PUSHCLIP ||
                               eventType == RecordedEvent::PUSHCLIPRECT)) {
      ReferencePtr drawTargetId = newEvent.recordedEvent->GetObjectRef();
      retainedDrawTargets[drawTargetId].back().mEndClipDepth++;
    }

    if (sRetainDrawTargets && eventType == RecordedEvent::POPCLIP) {
      ReferencePtr drawTargetId = newEvent.recordedEvent->GetObjectRef();
      retainedDrawTargets[drawTargetId].back().mEndClipDepth--;
    }

    if (sRetainSourceSurfaces && eventType == RecordedEvent::SNAPSHOT) {
      retainedObjectCreations.push_back(newEvent);
    }

    drawingEvents.push_back(newEvent);
  }

#ifdef WIN32
  RefPtr<ID3D10Device1> device;
  ::D3D10CreateDevice1(nullptr,
                       D3D10_DRIVER_TYPE_HARDWARE,
                       nullptr,
                       D3D10_CREATE_DEVICE_BGRA_SUPPORT |
                       D3D10_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS,
                       D3D10_FEATURE_LEVEL_10_0,
                       D3D10_1_SDK_VERSION,
                       byRef(device));

  Factory::SetDirect3D10Device(device);
#endif

  for (int i = 0; i < sizeof(sTestedBackends) / sizeof(BackendType); i++) {
    RefPtr<DrawTarget> dt = Factory::CreateDrawTarget(sTestedBackends[i], IntSize(1, 1), SurfaceFormat::B8G8R8A8);

    RawTranslator* translator =
      RawTranslator::Create(dt, sRetainDrawTargets, sRetainPaths,
                                sRetainSourceSurfaces, sRetainGradientStops);

    for (int c = 0; c < retainedObjectCreations.size(); c++) {
      translator->SetEventNumber(retainedObjectCreations[c].eventID);
      retainedObjectCreations[c].recordedEvent->PlayEvent(translator);
    }

    vector<double> data(sN + 1);
    double average = 0;

    for (int k = 0; k < (sN + 1); k++) {
      HighPrecisionMeasurement measurement;
      measurement.Start();

      for (int c = 0; c < drawingEvents.size(); c++) {
        translator->SetEventNumber(drawingEvents[c].eventID);
        drawingEvents[c].recordedEvent->PlayEvent(translator);
      }

      // Reset retained draw targets.
      for (const auto& drawTargetList : retainedDrawTargets) {
        for (const auto& drawTargetData : drawTargetList.second) {
          translator->SetEventNumber(drawTargetData.mCreationEvent);
          DrawTarget* drawTarget = translator->LookupDrawTarget(drawTargetList.first);
          for (int i = 0; i < drawTargetData.mEndClipDepth; i++) {
            drawTarget->PopClip();
          }
          drawTarget->SetTransform(Matrix());
          drawTarget->ClearRect(Rect(Point(), Size(drawTarget->GetSize())));
        }
      }

      if (k == 0 || k == sN) {
        // TODO: This skews the sqDiffSum.
        FinishDrawing(dt);
      }

      data[k] = measurement.Measure();

      if (k > 0) {
        average += data[k];
      }
    }
    average /= sN;

    double sqDiffSum = 0;
    for (int c = 1; c < sN + 1; c++) {
      sqDiffSum += pow(data[c] - average, 2);
    }

    sqDiffSum /= sN;

    printf("Rendering time (%s): %f +/- %f ms\n", GetBackendName(sTestedBackends[i]).c_str(), average, sqrt(sqDiffSum));

    delete translator;
  }

  return 0;
}
