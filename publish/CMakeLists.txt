cmake_minimum_required(VERSION 3.10)

# set the project name
project(redis_publish)


include_directories(~/workspace/hiredis/)

# add the executable
add_executable(redis_publish publish.c)

find_library(HIREDIS hiredis_static HINTS ~/workspace/hiredis/build/ )

target_link_libraries(redis_publish ${HIREDIS} pthread event)

