# Source files
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

# Add executable if there are source files
if(TEST_SOURCES)
  include_directories(third_party/googletest/googletest/include)
  add_executable(runTests ${TEST_SOURCES})
  target_link_libraries(runTests gtest gtest_main)
  add_test(NAME runTests COMMAND runTests)
endif()
