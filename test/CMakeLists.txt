# Source files
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Add library or executable if there are source files
if (TEST_SOURCES)
  add_library(test_lib ${TEST_SOURCES})
endif()
