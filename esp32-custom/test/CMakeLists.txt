# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required(VERSION 3.14)

project(
  Glow
  VERSION 0.1
  LANGUAGES CXX)

configure_file(GlowConfig.h.in GlowConfig.h)

add_library(GridLib src/grid.cpp include)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  include(FetchContent)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_subdirectory(tests)
endif()

add_executable(glow glow.cpp src/grid.cpp)

target_link_libraries(glow)

# target_include_directories(Glow PUBLIC
#   ${PROJECT_SOURCE_DIR}/include
# )
