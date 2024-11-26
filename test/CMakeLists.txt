# Source files
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

# Add executable if there are source files
if(TEST_SOURCES)
  include_directories(third_party/googletest/googletest/include)
  add_executable(runTests ${TEST_SOURCES})
  target_link_libraries(runTests gtest gtest_main)
  add_test(NAME runTests COMMAND runTests)

  # Copy DLLs to the output directory on WIN32 platform
  if(WIN32)
    add_custom_command(
      TARGET runTests
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest>
              $<TARGET_FILE_DIR:runTests>)
    add_custom_command(
      TARGET runTests
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest_main>
              $<TARGET_FILE_DIR:runTests>)
  endif()
endif()
