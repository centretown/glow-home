# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
# cmake_minimum_required (VERSION 2.8.11)

# Testing library
FetchContent_Declare(
  catch
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.6)
FetchContent_MakeAvailable(catch)

# Adds Catch2::Catch2

# Tests need to be added as executables first
add_executable(testgridlib
  test_grid.cpp)

add_executable(testchromalib
  test_chroma.cpp)

# Should be linked to the main library, as well as the Catch2 testing library
target_link_libraries(testgridlib PRIVATE GlowLib Catch2::Catch2)
target_link_libraries(testchromalib PRIVATE GlowLib Catch2::Catch2)

# enable testing functionality
enable_testing()

# If you register a test, then ctest and make test will run it.
# You can also run examples and check the output, as well.
add_test(NAME gridtest COMMAND testgridlib) # Command can be a target
add_test(NAME chromatest COMMAND testchromalib) # Command can be a target
