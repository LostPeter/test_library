cmake_minimum_required(VERSION 2.8.12)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(CMAKE_COMPILER_IS_GNUCC)

set(PROJ_NAME rpmalloc)
project(${PROJ_NAME})

set (CMAKE_CXX_STANDARD 11)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory("${PROJECT_SOURCE_DIR}/rpmalloc")