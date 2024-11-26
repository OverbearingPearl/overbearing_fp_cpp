cmake_minimum_required(VERSION 3.10)

project(overbearing_fp_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR})

option(LOGGING "Enable logging" ON)
message(STATUS "LOGGING: ${LOGGING}")

if(LOGGING)
  add_definitions(-DLOGGING_ENABLED)
endif()

if(WIN32)
  add_definitions(-DPLATFORM_WINDOWS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od")
elseif(UNIX)
  add_definitions(-DPLATFORM_LINUX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
  set(CMAKE_EXPORT_COMPILE_COMMANDS True)
endif()

enable_testing()

set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "")
add_subdirectory(third_party/googletest)

add_subdirectory(src)
add_subdirectory(samples)
add_subdirectory(test)

set(MAIN_SOURCE "${CMAKE_SOURCE_DIR}/main.cc")

add_executable(overbearing_fp_cpp ${MAIN_SOURCE})

if(TARGET src_lib)
  target_link_libraries(overbearing_fp_cpp src_lib)
endif()

if(TARGET samples_lib)
  target_link_libraries(overbearing_fp_cpp samples_lib)
endif()

if(TARGET test_lib)
  target_link_libraries(overbearing_fp_cpp test_lib)
endif()
