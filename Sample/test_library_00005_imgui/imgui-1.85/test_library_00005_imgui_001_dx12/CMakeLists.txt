#****************************************************************************
# test_library - Copyright (C) 2022 by LostPeter
# 
# Author: LostPeter
# Time:   2022-10-29
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
if (DEBUG)
    PROJECT(test_library_00005_imgui_001_dx12_d)
    SET(CMAKE_BUILD_TYPE "Debug")
    if (!WIN32)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif()

    MESSAGE(STATUS "Set Debug Build !")
else ()
    PROJECT(test_library_00005_imgui_001_dx12)
endif ()

SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 11)

#MESSAGE(STATUS "HOME dir: $ENV{HOME}")
MESSAGE(STATUS "Binary dir: ${PROJECT_BINARY_DIR}")
MESSAGE(STATUS "SOURCE dir: ${PROJECT_SOURCE_DIR}")
MESSAGE(STATUS "SYSTEM name: ${CMAKE_SYSTEM_NAME}")
MESSAGE(STATUS "SYSTEM version: ${CMAKE_SYSTEM_VERSION}")
MESSAGE(STATUS "SYSTEM processor: ${CMAKE_SYSTEM_PROCESSOR}")

#HEADER_FILES
FILE(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
MESSAGE(STATUS "HEADER files: ${HEADER_FILES}")

#SOURCE_FILES_CPP
FILE(GLOB_RECURSE SOURCE_FILES_CPP "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
MESSAGE(STATUS "SOURCE .cpp files: ${SOURCE_FILES_CPP}")


#LIB_INCLUDE
SET(LIB_INCLUDE "../../../../Include/imgui-1.85_dx12"
                "$ENV{VULKAN_SDK}/Include")

MESSAGE(STATUS "LIB_INCLUDE: ${LIB_INCLUDE}")
INCLUDE_DIRECTORIES(${LIB_INCLUDE})

#PLUGINS_DIR
SET(PLUGINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Plugins/Windows")
MESSAGE(STATUS "PLUGINS dir: ${PLUGINS_DIR}")

#BUILD
MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Win32 !")

if(MSVC)
    ADD_DEFINITIONS(/wd4251 /wd4275)
endif()
if (DEBUG)
    LINK_LIBRARIES(shlwapi.lib 
                   d3d12.lib
                   dxgi.lib
                   d3dcompiler.lib
                   ${PLUGINS_DIR}/imgui-1.85_dx12_d.lib)
else ()
    LINK_LIBRARIES(shlwapi.lib 
                   d3d12.lib
                   dxgi.lib
                   d3dcompiler.lib
                   ${PLUGINS_DIR}/imgui-1.85_dx12.lib)
endif()
ADD_EXECUTABLE(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES_CPP})