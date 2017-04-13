cmake_minimum_required(VERSION 2.8)
project(strings_stack)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc")

add_library(sstack STATIC sstack.c)
add_executable(main main.c)
target_link_libraries(main sstack)
