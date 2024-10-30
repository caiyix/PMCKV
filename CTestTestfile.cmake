# CMake generated Testfile for 
# Source directory: /home/zjn/桌面/app/advance-leveldb/leveldb
# Build directory: /home/zjn/桌面/app/advance-leveldb/leveldb
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(leveldb_tests "/home/zjn/桌面/app/advance-leveldb/leveldb/leveldb_tests")
set_tests_properties(leveldb_tests PROPERTIES  _BACKTRACE_TRIPLES "/home/zjn/桌面/app/advance-leveldb/leveldb/CMakeLists.txt;363;add_test;/home/zjn/桌面/app/advance-leveldb/leveldb/CMakeLists.txt;0;")
add_test(c_test "/home/zjn/桌面/app/advance-leveldb/leveldb/c_test")
set_tests_properties(c_test PROPERTIES  _BACKTRACE_TRIPLES "/home/zjn/桌面/app/advance-leveldb/leveldb/CMakeLists.txt;389;add_test;/home/zjn/桌面/app/advance-leveldb/leveldb/CMakeLists.txt;392;leveldb_test;/home/zjn/桌面/app/advance-leveldb/leveldb/CMakeLists.txt;0;")
add_test(env_posix_test "/home/zjn/桌面/app/advance-leveldb/leveldb/env_posix_test")
set_tests_properties(env_posix_test PROPERTIES  _BACKTRACE_TRIPLES "/home/zjn/桌面/app/advance-leveldb/leveldb/CMakeLists.txt;389;add_test;/home/zjn/桌面/app/advance-leveldb/leveldb/CMakeLists.txt;400;leveldb_test;/home/zjn/桌面/app/advance-leveldb/leveldb/CMakeLists.txt;0;")
subdirs("third_party/googletest")
subdirs("third_party/benchmark")
