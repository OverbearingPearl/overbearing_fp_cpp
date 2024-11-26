# Source files
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

# Add library or executable if there are source files
if(SOURCES)
  add_library(src_lib ${SOURCES})
endif()
