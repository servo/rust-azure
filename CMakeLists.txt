project(azure)
cmake_minimum_required(VERSION 2.6)

# Needed to avoid error
set(DUMMY ${CMAKE_BUILD_TYPE})

function(set_prefix var prefix)
  string(REGEX REPLACE "(^|;)([^;]+)" "\\1${prefix}\\2" tmp "${ARGN}")
  set(${var} "${tmp}" PARENT_SCOPE)
endfunction()

message(STATUS "skia: " $ENV{DEP_SKIA_OUTDIR})
message(STATUS "target: " $ENV{TARGET})

file(GLOB MOZ2D_H libazure/*.h)
file(GLOB MOZ2D_INTERNAL_H libazure/mozilla/*.h)
set(LIBAZURE_H ${MOZ2D_H} ${MOZ2D_INTERNAL_H})

# Pull in Skia defs and headers
add_definitions(-DSK_RELEASE -DGR_RELEASE=1)
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

add_definitions(-DUSE_SKIA -DUSE_SKIA_GPU)
add_definitions(-DMOZ_DISABLE_D2D_D3D)

add_definitions(-DWIN32 -EHsc)

add_definitions(-DMOZ_WARN_UNUSED_RESULTS="")

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

if(WIN32)
  set(LIBAZURE_SRC ${LIBAZURE_SRC} libazure/ScaledFontWin.cpp)
endif()

set(ALL_SRC
	${BINDINGS_SRC}
	${LIBAZURE_SRC}
	${LIBAZURE_H}
)

include_directories(libazure)
add_library(azure STATIC ${ALL_SRC})
install(TARGETS azure ARCHIVE DESTINATION lib)
