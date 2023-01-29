# CMake generated Testfile for 
# Source directory: /home/dave/src/glow-home/esp32-custom/test/tests
# Build directory: /home/dave/src/glow-home/esp32-custom/test/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(GRID "/home/dave/src/glow-home/esp32-custom/test/tests/test_grid")
set_tests_properties(GRID PROPERTIES  _BACKTRACE_TRIPLES "/home/dave/src/glow-home/esp32-custom/test/tests/CMakeLists.txt;39;add_test;/home/dave/src/glow-home/esp32-custom/test/tests/CMakeLists.txt;0;")
add_test(CHROMA "/home/dave/src/glow-home/esp32-custom/test/tests/test_chroma")
set_tests_properties(CHROMA PROPERTIES  _BACKTRACE_TRIPLES "/home/dave/src/glow-home/esp32-custom/test/tests/CMakeLists.txt;40;add_test;/home/dave/src/glow-home/esp32-custom/test/tests/CMakeLists.txt;0;")
add_test(SPINNER "/home/dave/src/glow-home/esp32-custom/test/tests/test_spinner")
set_tests_properties(SPINNER PROPERTIES  _BACKTRACE_TRIPLES "/home/dave/src/glow-home/esp32-custom/test/tests/CMakeLists.txt;41;add_test;/home/dave/src/glow-home/esp32-custom/test/tests/CMakeLists.txt;0;")
add_test(PROPERTIES "/home/dave/src/glow-home/esp32-custom/test/tests/test_properties")
set_tests_properties(PROPERTIES PROPERTIES  _BACKTRACE_TRIPLES "/home/dave/src/glow-home/esp32-custom/test/tests/CMakeLists.txt;42;add_test;/home/dave/src/glow-home/esp32-custom/test/tests/CMakeLists.txt;0;")
subdirs("../_deps/catch-build")
