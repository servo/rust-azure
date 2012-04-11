CXX ?= g++
CXXFLAGS ?=
AR ?= ar

AZURE_CPP_SRC = \
	src/2d/Factory.cpp \
        src/2d/Matrix.cpp \
        src/2d/DrawTargetCairo.cpp \
        src/2d/SourceSurfaceCairo.cpp \
        src/2d/PathCairo.cpp \
        src/2d/Blur.cpp \
        src/2d/ScaledFontBase.cpp \
        src/2d/DrawTargetDual.cpp \
        $(NULL)

AZURE_OBJS = $(AZURE_CPP_SRC:%.cpp=%.o)
AZURE_CXXFLAGS = \
	-Iinclude \
	-I/usr/include/cairo \
	-DMOZ_GFX -DUSE_CAIRO \
	-DNS_ATTR_MALLOC="" -DNS_WARN_UNUSED_RESULT="" \

.PHONY: all
all: libazure.a

libazure.a: $(AZURE_OBJS)
	$(AR) rcs libazure.a $(AZURE_OBJS)

src/2d/%.o: src/2d/%.cpp
	$(CXX) $< -o $@ -c $(AZURE_CXXFLAGS)
