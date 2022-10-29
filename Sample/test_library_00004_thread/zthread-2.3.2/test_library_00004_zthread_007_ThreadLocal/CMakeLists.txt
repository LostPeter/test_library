# author: LostPeter
# time:   2022-10-29

CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
if (DEBUG)
    PROJECT(test_library_00004_zthread_007_ThreadLocal_d)
    SET(CMAKE_BUILD_TYPE "Debug")
    if (!WIN32)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif()

    MESSAGE(STATUS "Set Debug Build !")
else ()
    PROJECT(test_library_00004_zthread_007_ThreadLocal)
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
if (WIN32)
    SET(LIB_INCLUDE "../../../../Include/zthread-2.3.2")

elseif (ANDROID)        
    SET(LIB_INCLUDE "../../../../Include/zthread-2.3.2")

elseif (APPLE)
    SET(LIB_INCLUDE "../../../../Include/zthread-2.3.2")
   
endif ()
MESSAGE(STATUS "LIB_INCLUDE: ${LIB_INCLUDE}")
INCLUDE_DIRECTORIES(${LIB_INCLUDE})


#PLUGINS_DIR
if (WIN32)
    SET(PLUGINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Plugins/Windows")

elseif (ANDROID) 
    SET(PLUGINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Plugins/Android")

elseif (APPLE)
    SET(SYS_FRAMEWORKS_DIR /System/Library/Frameworks)
    SET(CARBON_LIBS ${SYS_FRAMEWORKS_DIR}/Carbon.framework)
    SET(PLUGINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Plugins/MacOS")

endif ()
MESSAGE(STATUS "PLUGINS dir: ${PLUGINS_DIR}")


if (WIN32)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Win32 !")
    
    if (DEBUG)
        LINK_LIBRARIES(${PLUGINS_DIR}/zthread-2.3.2_d.lib)
    else ()
        LINK_LIBRARIES(${PLUGINS_DIR}/zthread-2.3.2.lib)
    endif()
    ADD_EXECUTABLE(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES_CPP})

elseif (ANDROID)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Android !")


elseif (APPLE)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom MacOS !")
    
    ADD_EXECUTABLE(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES_CPP} ${SOURCE_FILES_MM})
    if (DEBUG)
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                              ${CARBON_LIBS}
                              ${PLUGINS_DIR}/libzthread-2.3.2_d.a)
    else ()
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                              ${CARBON_LIBS}
                              ${PLUGINS_DIR}/libzthread-2.3.2.a)
    endif()
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup \
        -Wl,-no_pie \
        -Wl,-search_paths_first"
    )
    
endif ()