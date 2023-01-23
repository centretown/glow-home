# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required(VERSION 3.14)

project(
  Glow
  VERSION 0.1
  LANGUAGES CXX)

configure_file(GlowConfig.h.in GlowConfig.h)

include_directories(../src)

add_library(GlowLib
  ../src/esphome/components/light/esp_hsv_color.cpp
  ../src/esphome/components/light/esp_hsv_color.h
  ../src/esphome/core/color.h
  ../src/esphome/core/helpers.h
  ../src/esphome/core/optional.h
  ../src/glow/base.cpp
  ../src/glow/base.h
  ../src/glow/Properties.h
  ../src/glow/Grid.cpp
  ../src/glow/Grid.h
  ../src/glow/Chroma.cpp
  ../src/glow/Chroma.h
  ../src/glow/TestTimer.h
  ../src/glow/TestLight.h
  ../src/glow/TestLight.cpp
  ../src/glow/TestSpinner.h
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  include(FetchContent)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_subdirectory(tests)
endif()

add_executable(glow 
glow.cpp 
../src/esphome/components/light/esp_hsv_color.cpp
../src/glow/Chroma.cpp
../src/glow/Grid.cpp
../src/glow/base.cpp
../src/glow/TestLight.cpp
)

target_link_libraries(glow)
