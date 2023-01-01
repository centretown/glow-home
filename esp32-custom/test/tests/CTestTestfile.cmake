# CMake generated Testfile for 
# Source directory: /home/dave/src/glow-test/tests
# Build directory: /home/dave/src/glow-test/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(testlibtest "/home/dave/src/glow-test/tests/testlib")
set_tests_properties(testlibtest PROPERTIES  _BACKTRACE_TRIPLES "/home/dave/src/glow-test/tests/CMakeLists.txt;25;add_test;/home/dave/src/glow-test/tests/CMakeLists.txt;0;")
subdirs("../_deps/catch-build")
