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
add_executable(test_grid
  test_grid.cpp)

add_executable(test_chroma
  test_chroma.cpp)

add_executable(test_spinner
  test_spinner.cpp)

add_executable(test_properties
  test_properties.cpp)

# Should be linked to the main library, as well as the Catch2 testing library
target_link_libraries(test_grid PRIVATE GlowLib Catch2::Catch2)
target_link_libraries(test_chroma PRIVATE GlowLib Catch2::Catch2)
target_link_libraries(test_spinner PRIVATE GlowLib Catch2::Catch2)
target_link_libraries(test_properties PRIVATE GlowLib Catch2::Catch2)

# enable testing functionality
enable_testing()

# If you register a test, then ctest and make test will run it.
# You can also run examples and check the output, as well.
add_test(NAME GRID COMMAND test_grid) # Command can be a target
add_test(NAME CHROMA COMMAND test_chroma) # Command can be a target
add_test(NAME SPINNER COMMAND test_spinner) # Command can be a target
add_test(NAME PROPERTIES COMMAND test_properties) # Command can be a target
