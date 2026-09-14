cmake_minimum_required(VERSION 3.16)

project(snake_game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

add_executable(snake main.cpp)

target_include_directories(snake PRIVATE ${CURSES_INCLUDE_DIRS})
target_link_libraries(snake PRIVATE ${CURSES_LIBRARIES})

target_compile_options(snake PRIVATE -Wall -Wextra)
