# High level targets
.PHONY: release debug player2d check clean

OBJDIR_RELEASE=release
OBJDIR_DEBUG=debug
INCLUDES=.
CXXFLAGS=-std=gnu++0x -Wall
DEBUGFLAGS=-g -DDEBUG
RELEASEFLAGS=-O3
DEFINES=USE_SSE2

MOZ2D_CAIRO=true

ifeq ($(QMAKE_BIN),)
QMAKE_BIN=qmake
else
endif

# Mark these targets as phony because we don't know all the dependencies for
# player2d at this level, so invoke the player2d Makefiles every time and let
# the submake deal with it.
.PHONY: $(OBJDIR_RELEASE)/player2d/player2d $(OBJDIR_DEBUG)/player2d/player2d

# ================ OS FEATURES ================
UNAME = $(shell uname)
ifeq ($(UNAME),Darwin)
LIBS += -framework CoreFoundation -framework ApplicationServices
endif

# TODO: Files that depends on mozilla:
#convolver.cpp
#image_operations.cpp

MOZ2D_CPPSRCS_ALLPLATFORMS = \
  Blur.cpp \
  BlurSSE2.cpp \
  DataSourceSurface.cpp \
  DrawEventRecorder.cpp \
  DrawTargetDual.cpp \
  DrawTargetRecording.cpp \
  Factory.cpp \
  FilterNodeSoftware.cpp \
  FilterProcessing.cpp \
  FilterProcessingScalar.cpp \
  FilterProcessingSSE2.cpp \
  ImageScaling.cpp \
  ImageScalingSSE2.cpp \
  Matrix.cpp \
  Path.cpp \
  PathRecording.cpp \
  RecordedEvent.cpp \
  Scale.cpp \
  ScaledFontBase.cpp \
  SourceSurfaceRawData.cpp \
  $(NULL)

PERFTEST_CPPSRCS_ALLPLATFORMS = \
  perftest/Main.cpp \
  perftest/SanityChecks.cpp \
  perftest/TestBase.cpp \
  perftest/TestDrawTargetBase.cpp \
  $(NULL)

RECORDBENCH_CPPSRCS_ALLPLATFORMS = \
  recordbench/Main.cpp \
  recordbench/RawTranslator.cpp \
  $(NULL)

UNITTEST_CPPSRCS_ALLPLATFORMS = \
  unittest/SanityChecks.cpp \
  unittest/TestBase.cpp \
  unittest/TestPoint.cpp \
  unittest/TestRect.cpp \
  unittest/TestMatrix.cpp \
  unittest/TestScaling.cpp \
  unittest/Main.cpp \
  unittest/TestDrawTarget.cpp \
  unittest/TestPath.cpp \
  unittest/TestBugs.cpp \
  $(NULL)

ifeq ($(UNAME),Darwin)
LIBS += -framework CoreFoundation
endif

MOZ2D_CPPSRCS = $(MOZ2D_CPPSRCS_ALLPLATFORMS)
UNITTEST_CPPSRCS = $(UNITTEST_CPPSRCS_ALLPLATFORMS)
PERFTEST_CPPSRCS = $(PERFTEST_CPPSRCS_ALLPLATFORMS)
RECORDBENCH_CPPSRCS = $(RECORDBENCH_CPPSRCS_ALLPLATFORMS)

ifeq ($(UNAME),Linux)
DEFINES += MOZ_ENABLE_FREETYPE
INCLUDES += /usr/include/freetype2
LIBS += -lfreetype
MOZ2D_PLAYER2D_LIBS += -lfreetype
endif
ifeq ($(UNAME),Darwin)
DEFINES += MOZ_ENABLE_FREETYPE
CXXFLAGS += $(shell freetype-config --cflags)
LIBS += $(shell freetype-config --libs)
MOZ2D_PLAYER2D_LIBS += $(LIBS)
endif

# ================ INCLUDE FEATURE SPECIFIC MAKEFILES =================
# These will modify the following variables:
#   TESTING_CPPSRCS
#   MOZ2D_CPPSRCS
#   DEFINES
#
ifdef MOZ2D_D2D
 -include Makefile.d2d
endif
ifdef MOZ2D_CG
 -include Makefile.cg
endif
ifdef MOZ2D_CAIRO
 -include Makefile.cairo
  QMAKE_PARAMS += "MOZ2D_CAIRO=1"
endif
ifdef MOZ2D_SKIA
 -include Makefile.skia
  QMAKE_PARAMS += "MOZ2D_SKIA=$(MOZ2D_SKIA)"
endif
ifdef MOZ2D_NVPR
 -include Makefile.nvpr
  QMAKE_PARAMS += "MOZ2D_NVPR=$(MOZ2D_NVPR)"
endif

QMAKE_PARAMS += "MOZ2D_PATH=$(PWD)"

CXXFLAGS += $(addprefix -I,$(INCLUDES))

CPPSRCS = $(MOZ2D_CPPSRCS) $(UNITTEST_CPPSRCS) $(PERFTEST_CPPSRCS) $(RECORDBENCH_CPPSRCS)
PRE_PERFTEST_CPPSRCS = $(MOZ2D_CPPSRCS) $(PERFTEST_CPPSRCS)
PRE_UNITTEST_CPPSRCS = $(MOZ2D_CPPSRCS) $(UNITTEST_CPPSRCS)
PRE_RECORDBENCH_CPPSRCS = $(MOZ2D_CPPSRCS) $(RECORDBENCH_CPPSRCS)

RELEASE_CPPSRCS = $(addprefix $(OBJDIR_RELEASE)/,$(CPPSRCS))
DEBUG_CPPSRCS = $(addprefix $(OBJDIR_DEBUG)/,$(CPPSRCS))
RELEASE_PERFTEST_CPPSRCS = $(addprefix $(OBJDIR_RELEASE)/,$(PRE_PERFTEST_CPPSRCS))
DEBUG_PERFTEST_CPPSRCS = $(addprefix $(OBJDIR_DEBUG)/,$(PRE_PERFTEST_CPPSRCS))
RELEASE_UNITTEST_CPPSRCS = $(addprefix $(OBJDIR_RELEASE)/,$(PRE_UNITTEST_CPPSRCS))
DEBUG_UNITTEST_CPPSRCS = $(addprefix $(OBJDIR_DEBUG)/,$(PRE_UNITTEST_CPPSRCS))
RELEASE_RECORDBENCH_CPPSRCS = $(addprefix $(OBJDIR_RELEASE)/,$(PRE_RECORDBENCH_CPPSRCS))
DEBUG_RECORDBENCH_CPPSRCS = $(addprefix $(OBJDIR_DEBUG)/,$(PRE_RECORDBENCH_CPPSRCS))
COMPILER_DEFINES = $(addprefix -D,$(DEFINES))

-include $(RELEASE_CPPSRCS:.cpp=.d)
-include $(DEBUG_CPPSRCS:.cpp=.d)

release: \
  $(OBJDIR_RELEASE)/libmoz2d.a \
  $(OBJDIR_RELEASE)/unittest/unittest \
  $(OBJDIR_RELEASE)/perftest/perftest \
  $(OBJDIR_RELEASE)/recordbench/recordbench \
  $(OBJDIR_RELEASE)/.mkdir.done \
	$(NULL)

debug: \
  $(OBJDIR_DEBUG)/libmoz2d.a \
  $(OBJDIR_DEBUG)/unittest/unittest \
  $(OBJDIR_DEBUG)/perftest/perftest \
  $(OBJDIR_DEBUG)/recordbench/recordbench \
  $(OBJDIR_DEBUG)/.mkdir.done \
	$(NULL)

player2d: $(OBJDIR_RELEASE)/player2d/player2d $(OBJDIR_DEBUG)/player2d/player2d

$(OBJDIR_RELEASE)/unittest/unittest: $(RELEASE_UNITTEST_CPPSRCS:.cpp=.o)
	$(CXX) $(RELEASE_UNITTEST_CPPSRCS:.cpp=.o) $(LIBS) -o $(OBJDIR_RELEASE)/unittest/unittest

$(OBJDIR_DEBUG)/unittest/unittest: $(DEBUG_UNITTEST_CPPSRCS:.cpp=.o)
	$(CXX) $(DEBUG_UNITTEST_CPPSRCS:.cpp=.o) $(LIBS) -o $(OBJDIR_DEBUG)/unittest/unittest

$(OBJDIR_RELEASE)/perftest/perftest: $(RELEASE_PERFTEST_CPPSRCS:.cpp=.o)
	$(CXX) $(RELEASE_PERFTEST_CPPSRCS:.cpp=.o) $(LIBS) -o $(OBJDIR_RELEASE)/perftest/perftest

$(OBJDIR_DEBUG)/perftest/perftest: $(DEBUG_PERFTEST_CPPSRCS:.cpp=.o)
	$(CXX) $(DEBUG_PERFTEST_CPPSRCS:.cpp=.o) $(LIBS) -o $(OBJDIR_DEBUG)/perftest/perftest

$(OBJDIR_RELEASE)/recordbench/recordbench: $(RELEASE_RECORDBENCH_CPPSRCS:.cpp=.o)
	$(CXX) $(RELEASE_RECORDBENCH_CPPSRCS:.cpp=.o) $(LIBS) -o $(OBJDIR_RELEASE)/recordbench/recordbench

$(OBJDIR_DEBUG)/recordbench/recordbench: $(DEBUG_RECORDBENCH_CPPSRCS:.cpp=.o)
	$(CXX) $(DEBUG_RECORDBENCH_CPPSRCS:.cpp=.o) $(LIBS) -o $(OBJDIR_DEBUG)/recordbench/recordbench

$(OBJDIR_RELEASE)/libmoz2d.a: $(RELEASE_CPPSRCS:.cpp=.o)
	ar rcs $(OBJDIR_RELEASE)/libmoz2d.a $(RELEASE_CPPSRCS:.cpp=.o)

$(OBJDIR_DEBUG)/libmoz2d.a: $(DEBUG_CPPSRCS:.cpp=.o)
	ar rcs $(OBJDIR_DEBUG)/libmoz2d.a $(DEBUG_CPPSRCS:.cpp=.o)

$(OBJDIR_RELEASE)/player2d/player2d: $(OBJDIR_RELEASE)/.mkdir.done $(OBJDIR_RELEASE)/libmoz2d.a
	@hash $(QMAKE_BIN) || "echo Please install Qt for your platform: http://qt-project.org/"
	export MOZ2D_PLAYER2D_LIBS="$(MOZ2D_PLAYER2D_LIBS)" && cd $(OBJDIR_RELEASE)/player2d && $(QMAKE_BIN) ../../player2d $(QMAKE_PARAMS) && make

$(OBJDIR_DEBUG)/player2d/player2d: $(OBJDIR_DEBUG)/.mkdir.done $(OBJDIR_DEBUG)/libmoz2d.a
	@hash $(QMAKE_BIN) || "echo Please install Qt for your platform: http://qt-project.org/"
	export MOZ2D_PLAYER2D_LIBS="$(MOZ2D_PLAYER2D_LIBS)" && cd $(OBJDIR_DEBUG)/player2d && $(QMAKE_BIN) ../../player2d $(QMAKE_PARAMS) MOZ2D_DEBUG=1 && make

check: $(OBJDIR_RELEASE)/unittest/unittest $(OBJDIR_RELEASE)/perftest/perftest
	$(OBJDIR_RELEASE)/unittest/unittest
	$(OBJDIR_RELEASE)/perftest/perftest

clean:
	rm -rf $(OBJDIR_RELEASE) $(OBJDIR_DEBUG)




# ================== IMPLICIT RULES ===========================

%/.mkdir.done:
	mkdir -p $(dir $@)/perftest
	mkdir -p $(dir $@)/unittest
	mkdir -p $(dir $@)/recordbench
	mkdir -p $(dir $@)/player2d
	mkdir -p $(dir $@)/nvpr
	mkdir -p $@

$(OBJDIR_RELEASE)/%.o: %.cpp $(OBJDIR_RELEASE)/.mkdir.done
	$(CXX) -c $(COMPILER_DEFINES) $(RELEASEFLAGS) $(CXXFLAGS) -o $@ $<
	$(CXX) -MM -MT $@ $(COMPILER_DEFINES) $(RELEASEFLAGS) $(CXXFLAGS) -o $(@:.o=.d) $<

$(OBJDIR_DEBUG)/%.o: %.cpp $(OBJDIR_DEBUG)/.mkdir.done
	$(CXX) $(COMPILER_DEFINES) $(DEBUGFLAGS) $(CXXFLAGS) -c -o $@ $<
	$(CXX) -MM -MT $@ $(COMPILER_DEFINES) $(DEBUGFLAGS) $(CXXFLAGS) -o $(@:.o=.d) $<

