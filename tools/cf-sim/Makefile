# Host clockface render-preview. See README.md.
#   make [FACE_DIR=...] [CW_COMMONS_DIR=...]
CXX := g++
AR := ar

CLOCKWISE_ROOT := $(abspath $(CURDIR)/../..)
FACE_DIR ?= $(HOME)/git/clockwise_project/cw-cf-house
CW_COMMONS_DIR ?= $(CLOCKWISE_ROOT)/firmware/lib/cw-commons
CW_GFX_ENGINE_DIR ?= $(CLOCKWISE_ROOT)/firmware/lib/cw-gfx-engine

CXXFLAGS := -std=c++17 -Wall -Wextra -DARDUINO=100 -include Arduino.h \
  -Iinclude -Isrc -I$(CW_COMMONS_DIR) -I$(CW_GFX_ENGINE_DIR) \
  -I$(FACE_DIR) -I$(FACE_DIR)/gfx

SIMCORE_OBJS := src/Adafruit_GFX.o src/SimRenderer.o src/SimClock.o \
  src/HostEngine.o src/Locator.o src/EventBus.o src/Sprite.o
# Face sources compile into ./obj/ (mirroring face subdirs) so face repos
# stay clean. All .cpp files under FACE_DIR are built – just point at the
# clockface folder, no file lists needed.
OBJDIR := obj
FACE_CPP := $(shell find $(FACE_DIR) -name '*.cpp')
FACE_OBJS := $(patsubst $(FACE_DIR)/%.cpp,$(OBJDIR)/%.o,$(FACE_CPP))

all: sim sim_test_renderer sim_test_text

libsimcore.a: $(SIMCORE_OBJS)
	$(AR) rcs $@ $^

src/Adafruit_GFX.o: src/Adafruit_GFX.cpp src/SimRenderer.h include/Adafruit_GFX.h include/gfxfont.h src/glcdfont.c include/Arduino.h include/Print.h
	$(CXX) $(CXXFLAGS) -c -o $@ src/Adafruit_GFX.cpp

src/SimRenderer.o: src/SimRenderer.cpp src/SimRenderer.h include/Arduino.h include/Print.h
	$(CXX) $(CXXFLAGS) -c -o $@ src/SimRenderer.cpp

src/SimClock.o: src/SimClock.cpp src/SimRenderer.h include/Arduino.h
	$(CXX) $(CXXFLAGS) -c -o $@ src/SimClock.cpp

# Real cw-gfx-engine sources (Locator/EventBus/Sprite) so event-driven
# faces (Mario/Pacman) link on host.
src/Locator.o: $(CW_GFX_ENGINE_DIR)/Locator.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(CW_GFX_ENGINE_DIR)/Locator.cpp

src/EventBus.o: $(CW_GFX_ENGINE_DIR)/EventBus.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(CW_GFX_ENGINE_DIR)/EventBus.cpp

src/Sprite.o: $(CW_GFX_ENGINE_DIR)/Sprite.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(CW_GFX_ENGINE_DIR)/Sprite.cpp

src/HostEngine.o: src/HostEngine.cpp include/Arduino.h
	$(CXX) $(CXXFLAGS) -c -o $@ src/HostEngine.cpp

src/main.o: src/main.cpp src/SimRenderer.h src/BMPWriter.h src/FakeDateTime.h include/Arduino.h $(FACE_DIR)/Clockface.h
	$(CXX) $(CXXFLAGS) -c -o $@ src/main.cpp

src/BMPWriter.o: src/BMPWriter.cpp src/BMPWriter.h
	$(CXX) $(CXXFLAGS) -c -o $@ src/BMPWriter.cpp

$(OBJDIR)/%.o: $(FACE_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

sim: src/main.o src/BMPWriter.o $(FACE_OBJS) libsimcore.a
	$(CXX) $(CXXFLAGS) -o $@ src/main.o src/BMPWriter.o $(FACE_OBJS) libsimcore.a

sim_test_renderer: libsimcore.a src/test_renderer.cpp src/SimRenderer.h include/Arduino.h
	$(CXX) $(CXXFLAGS) -o $@ src/test_renderer.cpp libsimcore.a

sim_test_text: libsimcore.a src/test_text.cpp src/SimRenderer.h include/Arduino.h
	$(CXX) $(CXXFLAGS) -o $@ src/test_text.cpp libsimcore.a

.PHONY: run
run: sim_test_renderer
	./sim_test_renderer

.PHONY: run_text
run_text: sim_test_text
	./sim_test_text

.PHONY: clean
clean:
	rm -rf sim sim_test_renderer sim_test_text libsimcore.a obj Clockface.o $(SIMCORE_OBJS) src/main.o src/BMPWriter.o
