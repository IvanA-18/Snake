﻿cmake_minimum_required (VERSION 3.8)

project ("snake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
set(SFML_STATIC_LIBRARIES TRUE)

add_executable (snake "snake.cpp")

set(SFML_DIR "F:\\snake_C++\\lib\\SFML\\Visual C++ 15 (2017) - 64-bit\\SFML-2.5.1\\lib\\cmake\\SFML")

target_link_libraries(snake sfml-graphics)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML 2.5 REQUIRED graphics network audio window system)
