cmake_minimum_required(VERSION 3.16)
project(cw-cf-sim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Path to the clockface to preview (folder containing Clockface.cpp/.h).
# Defaults to the house face; override with -DFACE_DIR=... .
# All .cpp files under it (e.g. gfx/*.cpp) are built automatically.
set(FACE_DIR "$ENV{HOME}/git/clockwise_project/cw-cf-house"
    CACHE PATH "Clockface folder to render")
file(GLOB_RECURSE FACE_CPP CONFIGURE_DEPENDS "${FACE_DIR}/*.cpp")

# Shared libs inside the clockwise repo (picopixel.h, plus the real
# cw-gfx-engine Locator/EventBus/Sprite so event-driven faces link).
# Resolved relative to this file so the tool survives workspace moves.
get_filename_component(CLOCKWISE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
set(CW_COMMONS_DIR "${CLOCKWISE_ROOT}/firmware/lib/cw-commons"
    CACHE PATH "Clockwise cw-commons dir (for picopixel.h)")
set(CW_GFX_ENGINE_DIR "${CLOCKWISE_ROOT}/firmware/lib/cw-gfx-engine"
    CACHE PATH "Clockwise cw-gfx-engine dir")

add_library(simcore STATIC
  src/Adafruit_GFX.cpp
  src/SimRenderer.cpp
  src/SimClock.cpp
  src/HostEngine.cpp
  ${CW_GFX_ENGINE_DIR}/Locator.cpp
  ${CW_GFX_ENGINE_DIR}/EventBus.cpp
  ${CW_GFX_ENGINE_DIR}/Sprite.cpp
)
target_include_directories(simcore PUBLIC include src)
target_include_directories(simcore PUBLIC ${CW_COMMONS_DIR} ${CW_GFX_ENGINE_DIR})
target_compile_options(simcore PUBLIC -include Arduino.h -DARDUINO=100)

add_executable(sim
  src/main.cpp
  src/BMPWriter.cpp
  ${FACE_CPP}
)
target_include_directories(sim PRIVATE ${FACE_DIR} ${FACE_DIR}/gfx)
target_link_libraries(sim simcore)

add_executable(sim_test_renderer src/test_renderer.cpp)
target_link_libraries(sim_test_renderer simcore)

add_executable(sim_test_text src/test_text.cpp)
target_link_libraries(sim_test_text simcore)

enable_testing()
add_test(NAME renderer COMMAND sim_test_renderer)
add_test(NAME text COMMAND sim_test_text)
