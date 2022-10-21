cmake_minimum_required(VERSION 3.23)
project(t9search C)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -DDEBUG)
#add_compile_options(-Werror)

add_executable(t9search t9search.c)
