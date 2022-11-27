if(WIN32)
  message(STATUS "Testing temporary not support Windows platform")
  return()
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/test)

include_directories(../zoz)

add_executable(test_token test_token.c)
target_link_libraries(test_token PUBLIC ${PROJECT_NAME}_test_lib)
add_test(NAME test_token COMMAND test_token)