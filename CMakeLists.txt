project(azure)
cmake_minimum_required(VERSION 2.8)

function(set_prefix var prefix)
  string(REGEX REPLACE "(^|;)([^;]+)" "\\1${prefix}\\2" tmp "${ARGN}")
  set(${var} "${tmp}" PARENT_SCOPE)
endfunction()

# Use ccache if available
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Zi")
  # TODO(vlad): we need to update our copy of Moz2D!
  # VS2015/VC 14.0 needs this -- we should be using unordered_set instead of hash_set
  add_definitions(-D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

if("$ENV{CFG_ENABLE_DEBUG_SKIA}" STREQUAL "1")
  add_definitions(
    -DDSK_DEBUG
    -DGR_DEBUG=1
    -DGR_GL_LOG_CALLS=1
    -DGR_GL_LOG_CALLS_START=1
  )
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
  add_definitions(
    -DSK_RELEASE
    -DGR_RELEASE=1
  )
endif()

file(GLOB MOZ2D_H libazure/*.h)
file(GLOB MOZ2D_INTERNAL_H libazure/mozilla/*.h)
set(LIBAZURE_H ${MOZ2D_H} ${MOZ2D_INTERNAL_H})

# Pull in Skia headers
include_directories(
	$ENV{DEP_SKIA_OUTDIR}/include
	$ENV{DEP_SKIA_OUTDIR}/include/core
	$ENV{DEP_SKIA_OUTDIR}/include/config
	$ENV{DEP_SKIA_OUTDIR}/include/effects
	$ENV{DEP_SKIA_OUTDIR}/include/ports
	$ENV{DEP_SKIA_OUTDIR}/include/utils
	$ENV{DEP_SKIA_OUTDIR}/include/gpu
	$ENV{DEP_SKIA_OUTDIR}/include/gpu/gl
	$ENV{DEP_SKIA_OUTDIR}/src
)

if(NOT "$ENV{DEP_FREETYPE_OUTDIR}" STREQUAL "")
  include_directories($ENV{DEP_FREETYPE_OUTDIR}/include)
  # makefile.cargo never added any link flags or libs in this case,
  # so we don't either
else()
  # We need to work with older cmake -- pkg_check_modules was added in
  # a version of cmake newer than what Ubuntu 14.04 LTS ships with.
  #pkg_check_modules(FREETYPE REQUIRED freetype2)
  EXEC_PROGRAM(pkg-config ARGS --cflags freetype2 OUTPUT_VARIABLE FREETYPE_CFLAGS)
  EXEC_PROGRAM(pkg-config ARGS --libs freetype2 OUTPUT_VARIABLE FREETYPE_CLDFLAGS)
  add_definitions(${FREETYPE_CFLAGS})
  link_libraries(${FREETYPE_CLDFLAGS})
endif()

add_definitions(-DUSE_SKIA -DUSE_SKIA_GPU)
add_definitions(-DMOZ_WARN_UNUSED_RESULT="")

# Azure bindings
set_prefix(BINDINGS_SRC src/
	azure-c.cpp
	azure-c.h
)

# Core azure library
set_prefix(LIBAZURE_SRC libazure/
	Blur.cpp
	convolver.cpp
	DataSourceSurface.cpp
	DataSurfaceHelpers.cpp
	DrawEventRecorder.cpp
	DrawTargetCapture.cpp
	DrawTarget.cpp
	DrawTargetDual.cpp
	DrawTargetRecording.cpp
	DrawTargetSkia.cpp
	DrawTargetTiled.cpp
	Factory.cpp
	FilterNodeSoftware.cpp
	FilterProcessing.cpp
	FilterProcessingScalar.cpp
	ImageScaling.cpp
	Matrix.cpp
	Path.cpp
	PathHelpers.cpp
	PathRecording.cpp
	PathSkia.cpp
	RecordedEvent.cpp
	ScaledFontBase.cpp
	ScaledFontSkia.cpp
	SourceSurfaceRawData.cpp
	SourceSurfaceSkia.cpp
)

set_prefix(LIBAZURE_X86_SRC libazure/
  BlurSSE2.cpp
  FilterProcessingSSE2.cpp
  ImageScalingSSE2.cpp
  convolverSSE2.cpp
  )

if(APPLE)
  add_definitions(-DXP_MACOSX -DXP_UNIX -Dtypeof=__typeof__)
  set_prefix(LIBAZURE_PLATFORM_SRC libazure/
    DrawTargetCG.cpp
    MacIOSurface.cpp
    PathCG.cpp
    ScaledFontMac.cpp
    SourceSurfaceCG.cpp
    )
endif() # MacOS X

if(WIN32)
  add_definitions(-DWIN32 -DXP_WIN -D_WIN32_WINNT=0x0600 -DSK_BUILD_FOR_WIN32)
  
  if(MSVC)
    # Enable C++ exceptions; extern "c" defaults to nothrow
    add_definitions(-EHsc)
  else()
    # This is going to be MSYS2/MINGW64; assume freetype
    add_definitions(-DMOZ_ENABLE_FREETYPE)
  endif()

  set_prefix(LIBAZURE_PLATFORM_SRC libazure/
    DrawTargetD2D.cpp
    SourceSurfaceD2D.cpp
    SourceSurfaceD2DTarget.cpp
    ScaledFontWin.cpp
    ScaledFontDWrite.cpp
    PathD2D.cpp
    )
endif()

if(ENV{TARGET} MATCHES androideabi)
  add_definitions(-DXP_UNIX -DSK_BUILD_FOR_ANDROID -DMOZ_ENABLE_FREETYPE)
elseif(ENV{TARGET} MATCHES linux)
  add_definitions(-DXP_UNIX -DSK_BUILD_FOR_UNIX -DMOZ_ENABLE_FREETYPE)
endif()

set(ALL_SRC
	${BINDINGS_SRC}
	${LIBAZURE_SRC}
	${LIBAZURE_PLATFORM_SRC}
	${LIBAZURE_H}
)

if(NOT WIN32)
  # Should we use -fPIE instead?
  add_definitions(-fPIC)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  add_definitions(-std=gnu++11)
endif()

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "x86")
  set(ALL_SRC ${ALL_SRC} ${LIBAZURE_X86_SRC})
  add_definitions(-DUSE_SSE2)
  if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2 -mfpmath=sse")
  else()
    # /arch:SSE2 is invalid on x86-64 (SSE2 always present)
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
    endif()
  endif()
elseif(ENV{TARGET} MATCHES "arm")
  add_definitions(-mfpu=neon -D__ARM_HAVE_OPTIONAL_NEON_SUPPORT)
  # FIXME: Need more advanced detection of FP support
  if(ENV{TARGET} MATCHES "eabihf")
    add_definitions(-mfloat-abi=hard)
  else()
    add_definitions(-mfloat-abi=softfp)
  endif()
  if(NOT "$ENV{TARGET}" STREQUAL "$ENV{HOST}")
    # FIXME: Assumes armv7 for cross compile
    add_definitions(-march=armv7-a -mthumb)
  endif()
elseif(ENV{TARGET} MATCHES "aarch64")
  add_definitions(-D__ARM_HAVE_NEON)
endif()

include_directories(libazure)
add_library(azure STATIC ${ALL_SRC})
install(TARGETS azure ARCHIVE DESTINATION lib)
