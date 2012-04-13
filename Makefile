CC ?= gcc
CXX ?= g++
CXXFLAGS ?=
AR ?= ar
RUSTC ?= rustc
RUSTFLAGS ?=

CXXFLAGS += -fPIC

MOZALLOC_CPP_SRC = \
	src/memory/mozalloc/mozalloc_abort.cpp \
	src/memory/mozalloc/mozalloc.cpp \
	src/memory/mozalloc/mozalloc_oom.cpp \
	$(NULL)

MOZALLOC_CXXFLAGS = \
	-Iinclude \
	-DXP_UNIX \
	-DNS_ATTR_MALLOC="" -DNS_WARN_UNUSED_RESULT="" \
	$(CXXFLAGS) \
	$(NULL)

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

AZURE_CPP_SRC += azure-c.cpp

AZURE_CXXFLAGS = \
	-Iinclude \
	-I/usr/include/cairo \
	-DMOZ_GFX -DUSE_CAIRO \
	-DNS_ATTR_MALLOC="" -DNS_WARN_UNUSED_RESULT="" \
	$(CXXFLAGS) \
	$(NULL)

ALL_CPP_SRC = $(MOZALLOC_CPP_SRC) $(AZURE_CPP_SRC)
ALL_OBJS = $(ALL_CPP_SRC:%.cpp=%.o)

RUST_SRC=$(shell find . -type f -name '*.rs')

.PHONY: all
all: libazure.dummy

libazure.dummy: azure.rc $(RUST_SRC) libazure.a
	$(RUSTC) $(RUSTFLAGS) -o $@ $<
	touch $@

libazure.a: $(ALL_OBJS)
	$(AR) rcs libazure.a $(ALL_OBJS)

src/2d/%.o: src/2d/%.cpp
	$(CXX) $< -o $@ -c $(AZURE_CXXFLAGS)

src/memory/mozalloc/%.o: src/memory/mozalloc/%.cpp
	$(CXX) $< -o $@ -c $(MOZALLOC_CXXFLAGS)

./%.o: ./%.cpp
	$(CXX) $< -o $@ -c $(AZURE_CXXFLAGS)

.PHONY: clean
clean:
	rm -f *.o *.a azure */*/*.o *.so
