#-------------------------------------------------
#
# Project created by QtCreator 2012-08-06T16:20:06
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = player2d
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    drawtargetwidget.cpp \
    playbackmanager.cpp \
    displaymanager.cpp \
    drawtargetview.cpp \
    TreeItems.cpp \
    surfaceview.cpp \
    sourcesurfaceview.cpp \
    gradientstopsview.cpp \
    redundancyanalysis.cpp \
    calltiminganalysis.cpp

HEADERS  += mainwindow.h \
    drawtargetwidget.h \
    playbackmanager.h \
    TreeItems.h \
    displaymanager.h \
    drawtargetview.h \
    surfaceview.h \
    sourcesurfaceview.h \
    gradientstopsview.h \
    redundancyanalysis.h \
    timer.h \
    calltiminganalysis.h

FORMS    += mainwindow.ui \
    drawtargetview.ui \
    sourcesurfaceview.ui \
    gradientstopsview.ui \
    redundancyanalysis.ui \
    calltiminganalysis.ui

!isEmpty(MOZ2D_CAIRO) {
  DEFINES += USE_CAIRO
}
!isEmpty(MOZ2D_SKIA) {
  DEFINES += USE_SKIA
}
!isEmpty(MOZ2D_NVPR) {
  DEFINES += USE_NVPR
}
!isEmpty(MOZ2D_DEBUG) {
  CONFIG += debug
}

CONFIG(release, debug|release) {
  CONFIG_PREFIX = "Release"
} else {
  CONFIG_PREFIX = "Debug"
}

win32 {
  !isEmpty(MOZ2D_CAIRO) {
    isEmpty(MOZ2D_SKIA) {
      error("Can only build with both Skia and Cairo or neither on windows.")
    }
    DIR_SUFFIX = " (With Skia)"

    LIBS += -L"$$PWD/../../skia/out/$$CONFIG_PREFIX" -L"$$PWD/../../cairo/src/$$CONFIG_PREFIX"
    LIBS += -lskia_core -lskia_images -lskia_effects -lskia_utils -lskia_ports -lskia_opts -lskia_skgpu -lskia_opts_ssse3 -lskia_sfnt -lusp10 -lopengl32 -lcairo-static
  }

  LIBS += -L"$$PWD/../$$CONFIG_PREFIX$$DIR_SUFFIX/" -lgfx2d

} else:symbian: LIBS += -lgfx2d
else:unix {
  isEmpty(MOZ2D_PATH) {
    MOZ2D_PATH = "$$PWD/.."
  }

  CONFIG(release, debug|release) {
    LIBS += -L$$MOZ2D_PATH/release -lmoz2d $$(MOZ2D_PLAYER2D_LIBS)
  } else {
    LIBS += -L$$MOZ2D_PATH/debug -lmoz2d $$(MOZ2D_PLAYER2D_LIBS)
  }

  !macx: GROUP_START = -Wl,--start-group
  !macx: GROUP_END = -Wl,--end-group
  !isEmpty(MOZ2D_SKIA) {
    LIBS += -L$$MOZ2D_SKIA/out/$$CONFIG_PREFIX/ $$GROUP_START -lskia_images -lskia_effects -lskia_sfnt -lskia_utils -lskia_core -lskia_skgpu -lskia_opts -lskia_opts_ssse3 -lskia_ports -lfreetype -lfontconfig -lGL $$GROUP_END
  }
  !isEmpty(MOZ2D_NVPR) {
    LIBS += -ldl -lX11
  }
}

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

static: QTPLUGIN += qico
static: DEFINES += QT_STATIC

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../release/gfx2d.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../debug/gfx2d.lib

unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../release/libmoz2d.a
else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../debug/libmoz2d.a

win32: LIBS += -ld3d10_1 -ld3d11
win32: DEFINES += INITGUID USE_D2D1_1

RESOURCES += \
    resources.qrc
