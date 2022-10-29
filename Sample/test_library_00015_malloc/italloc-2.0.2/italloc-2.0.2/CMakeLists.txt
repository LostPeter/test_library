cmake_minimum_required(VERSION 2.8.12)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(CMAKE_COMPILER_IS_GNUCC)

set(PROJ_NAME italloc)
project(${PROJ_NAME})

set (CMAKE_CXX_STANDARD 11)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(SOURCES
    ltalloc.cc
    )

set(HEADERS
    ltalloc.h
    ltalloc.hpp
    )

add_library(italloc STATIC ${SOURCES} ${HEADERS})
target_include_directories(italloc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})