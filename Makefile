CC ?= gcc
CXX ?= g++
CXXFLAGS ?=
AR ?= ar
RUSTC ?= rustc
RUSTFLAGS ?=

UNAME=$(shell uname)

ifeq ($(UNAME),Darwin)
    OSTYPE=darwin
endif
ifeq ($(UNAME),Linux)
    OSTYPE=linux
endif

CXXFLAGS += -fPIC

MOZALLOC_CPP_SRC = \
	src/memory/mozalloc/mozalloc_abort.cpp \
	src/memory/mozalloc/mozalloc.cpp \
	src/memory/mozalloc/mozalloc_oom.cpp \
	$(NULL)

MOZALLOC_CXXFLAGS = \
	-Iinclude \
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
	-DMOZ_GFX -DUSE_CAIRO \
	-DNS_ATTR_MALLOC="" -DNS_WARN_UNUSED_RESULT="" \
	$(CXXFLAGS) \
	$(NULL)

ifeq ($(OSTYPE),darwin)
# The homebrew location of a particular version of cairo
# FIXME: This is not the right way to set up the lib location
RUSTFLAGS += -L /usr/local/Cellar/cairo/1.10.2/lib
MOZALLOC_CXXFLAGS += -DXP_MACOSX -DXP_UNIX
AZURE_CXXFLAGS += \
        -I/usr/local/Cellar/cairo/1.10.2/include/cairo \
	-DXP_MACOSX \
	$(NULL)
AZURE_CPP_SRC += \
	src/2d/ScaledFontMac.cpp \
	src/2d/DrawTargetCG.cpp \
	src/2d/PathCG.cpp \
	src/2d/SourceSurfaceCG.cpp \
	$(NULL)
endif

ifeq ($(OSTYPE),linux)
MOZALLOC_CXXFLAGS += -DXP_UNIX
AZURE_CXXFLAGS += \
	-I/usr/include/cairo \
	-DXP_UNIX \
	$(NULL)
endif

ALL_CPP_SRC = $(MOZALLOC_CPP_SRC) $(AZURE_CPP_SRC)
ALL_OBJS = $(ALL_CPP_SRC:%.cpp=%.o)

RUST_SRC=$(shell find . -type f -name '*.rs')

.PHONY: all
all: libazure.dummy

libazure.dummy: azure.rc $(RUST_SRC) libazure.a
	$(RUSTC) $(RUSTFLAGS) $< -o $@
	touch $@

azure-test: azure.rc $(RUST_SRC) libazure.a
	$(RUSTC) $(RUSTFLAGS) $< -o $@ --test

libazure.a: $(ALL_OBJS)
	$(AR) rcs libazure.a $(ALL_OBJS)

src/2d/%.o: src/2d/%.cpp
	$(CXX) $< -o $@ -c $(AZURE_CXXFLAGS)

src/memory/mozalloc/%.o: src/memory/mozalloc/%.cpp
	$(CXX) $< -o $@ -c $(MOZALLOC_CXXFLAGS)

./%.o: ./%.cpp
	$(CXX) $< -o $@ -c $(AZURE_CXXFLAGS)

check: azure-test
	./azure-test

.PHONY: clean
clean:
	rm -f azure-test *.o *.a */*/*.o *.so *.dylib *.dll *.dummy
