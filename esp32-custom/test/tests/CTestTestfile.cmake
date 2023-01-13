# CMake generated Testfile for 
# Source directory: /home/dave/src/glow-home/esp32-custom/test/tests
# Build directory: /home/dave/src/glow-home/esp32-custom/test/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(gridtest "/home/dave/src/glow-home/esp32-custom/test/tests/testgridlib")
set_tests_properties(gridtest PROPERTIES  _BACKTRACE_TRIPLES "/home/dave/src/glow-home/esp32-custom/test/tests/CMakeLists.txt;31;add_test;/home/dave/src/glow-home/esp32-custom/test/tests/CMakeLists.txt;0;")
add_test(chromatest "/home/dave/src/glow-home/esp32-custom/test/tests/testchromalib")
set_tests_properties(chromatest PROPERTIES  _BACKTRACE_TRIPLES "/home/dave/src/glow-home/esp32-custom/test/tests/CMakeLists.txt;32;add_test;/home/dave/src/glow-home/esp32-custom/test/tests/CMakeLists.txt;0;")
subdirs("../_deps/catch-build")
