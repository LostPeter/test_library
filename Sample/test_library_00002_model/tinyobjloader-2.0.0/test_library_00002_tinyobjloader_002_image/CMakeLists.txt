# author: LostPeter
# time:   2022-10-29

CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
if (DEBUG)
    PROJECT(test_library_00002_tinyobjloader_002_image_d)
    SET(CMAKE_BUILD_TYPE "Debug")
    if (!WIN32)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif()

    MESSAGE(STATUS "Set Debug Build !")
else ()
    PROJECT(test_library_00002_tinyobjloader_002_image)
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

#SOURCE_FILES_C
FILE(GLOB_RECURSE SOURCE_FILES_C "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
MESSAGE(STATUS "SOURCE .c files: ${SOURCE_FILES_C}")

#SOURCE_FILES_MM
if (APPLE)
    FILE(GLOB_RECURSE SOURCE_FILES_MM "${CMAKE_CURRENT_SOURCE_DIR}/*.mm")
    MESSAGE(STATUS "SOURCE .mm files: ${SOURCE_FILES_MM}")
endif ()

#LIB_INCLUDE
if (WIN32)
    SET(LIB_INCLUDE "../../../../Include/tinyobjloader-2.0.0"
                    "../../../../Include/glm-0.9.9.8"
                    "../../../../Include/stb-0.02"
                    "../../../../Include/glfw-3.3.6"
                    "$ENV{VULKAN_SDK}/Include")

elseif (ANDROID)
    SET(LIB_INCLUDE "../../../../Include/tinyobjloader-2.0.0"
                    "../../../../Include/glm-0.9.9.8"
                    "../../../../Include/stb-0.02"
                    "../../../../Include/glfw-3.3.6"
                    "$ENV{VULKAN_SDK}/Include")

elseif (APPLE)
    SET(LIB_INCLUDE "../../../../Include/tinyobjloader-2.0.0"
                    "../../../../Include/glm-0.9.9.8"
                    "../../../../Include/stb-0.02"
                    "../../../../Include/glfw-3.3.6"
                    "$ENV{VULKAN_HOME}/macOS/include")

    SET(SYS_FRAMEWORKS_DIR /System/Library/Frameworks)
    SET(COCOA_LIBS ${SYS_FRAMEWORKS_DIR}/Cocoa.framework)
    SET(COREVIDEO_LIBS ${SYS_FRAMEWORKS_DIR}/CoreVideo.framework)
    SET(IOKIT_LIBS ${SYS_FRAMEWORKS_DIR}/IOKit.framework)
    SET(SYS_VULKAN_LIBS "$ENV{VULKAN_HOME}/macOS/Frameworks/vulkan.framework")

endif ()
MESSAGE(STATUS "LIB_INCLUDE: ${LIB_INCLUDE}")
INCLUDE_DIRECTORIES(${LIB_INCLUDE})

#LIB_VULKAN_DIR, PLUGINS_DIR
if (WIN32)
    SET(LIB_VULKAN_DIR "$ENV{VULKAN_SDK}/Lib")
    SET(PLUGINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Plugins/Windows")

elseif (ANDROID) 
    SET(PLUGINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Plugins/Android")

elseif (APPLE)
    SET(LIB_VULKAN_DIR "$ENV{VULKAN_HOME}/macOS/lib")
    SET(PLUGINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Plugins/MacOS")

endif ()
MESSAGE(STATUS "LIB_VULKAN_DIR dir: ${LIB_VULKAN_DIR}")
MESSAGE(STATUS "PLUGINS dir: ${PLUGINS_DIR}")


function(ASSIGN_SOURCE_GROUP)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            FILE(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else ()
            SET(_source_rel "${_source}")
        endif ()
        GET_FILENAME_COMPONENT(_source_path "${_source_rel}" PATH)
        STRING(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        SOURCE_GROUP("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(ASSIGN_SOURCE_GROUP)

function(MY_ADD_LIBRARY)
    foreach(_source IN ITEMS ${ARGN})
        ASSIGN_SOURCE_GROUP(${_source})
    endforeach()
    ADD_LIBRARY(${ARGV})
endfunction(MY_ADD_LIBRARY)


if (WIN32)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Win32 !")
    
    if(MSVC)
        ADD_DEFINITIONS(/wd4251 /wd4275)
    endif()
    if (DEBUG)
        LINK_LIBRARIES(shlwapi.lib 
                       ${PLUGINS_DIR}/glfw-3.3.6_d.lib
                       ${LIB_VULKAN_DIR}/vulkan-1.lib)
    else ()
        LINK_LIBRARIES(shlwapi.lib 
                       ${PLUGINS_DIR}/glfw-3.3.6.lib
                       ${LIB_VULKAN_DIR}/vulkan-1.lib)
    endif ()
    
    ADD_EXECUTABLE(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES_CPP})

elseif (ANDROID) 
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Android !")


elseif (APPLE)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom MacOS !")
    
    if (DEBUG)
        LINK_LIBRARIES(${PLUGINS_DIR}/libglfw-3.3.6_d.a)
    else ()
        LINK_LIBRARIES(${PLUGINS_DIR}/libglfw-3.3.6.a)
    endif ()
    ADD_EXECUTABLE(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES_CPP} ${SOURCE_FILES_MM})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                          ${COCOA_LIBS}
                          ${COREVIDEO_LIBS}
                          ${IOKIT_LIBS}
                          ${LIB_VULKAN_DIR}/libvulkan.1.dylib 
                          ${LIB_VULKAN_DIR}/libvulkan.1.2.189.dylib 
                          ${SYS_VULKAN_LIBS})
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup \
        -Wl,-no_pie \
        -Wl,-search_paths_first"
    )

endif ()