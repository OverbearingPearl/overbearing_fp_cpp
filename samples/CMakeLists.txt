# Source files
file(GLOB_RECURSE SAMPLE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

# Add library or executable if there are source files
if (SAMPLE_SOURCES)
  add_library(samples_lib ${SAMPLE_SOURCES})
endif()
