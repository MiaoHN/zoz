file(GLOB ZOZ_SRC *.c)

add_executable(${PROJECT_NAME} ${ZOZ_SRC})

list(REMOVE_ITEM ZOZ_SRC "main.c")

add_library(${PROJECT_NAME}_test_lib ${ZOZ_SRC})