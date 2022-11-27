if (WIN32)
  message("Testing temporary not support Windows platform")
  return()
endif()

enable_testing()
include_directories(../zoz)

add_executable(test_token test_token.c)
target_link_libraries(test_token PUBLIC ${PROJECT_NAME}_test_lib)
add_test(NAME test_token COMMAND test_token)