# author: LostPeter
# time:   2022-10-29

CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
if (DEBUG)
    PROJECT(test_library_00005_imgui_006_vulkan_glfw_user_d)
    SET(CMAKE_BUILD_TYPE "Debug")
    if (!WIN32)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif()

    MESSAGE(STATUS "Set Debug Build !")
else ()
    PROJECT(test_library_00005_imgui_006_vulkan_glfw_user)
endif ()

SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 17)

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

#SOURCE_FILES_MM
if (APPLE)
    FILE(GLOB_RECURSE SOURCE_FILES_MM "${CMAKE_CURRENT_SOURCE_DIR}/*.mm")
    MESSAGE(STATUS "SOURCE .mm files: ${SOURCE_FILES_MM}")
endif ()

#LIB_INCLUDE
if (WIN32)
    SET(LIB_INCLUDE "$ENV{GLM_HOME}"
                    "$ENV{GLFW_HOME}/include"
                    "$ENV{STB_HOME}"
                    "../../../../Include/imgui-1.85_vulkan_glfw"
                    "$ENV{VULKAN_SDK}/Include")

elseif (ANDROID)        


elseif (APPLE)
    SET(LIB_INCLUDE "$ENV{GLM_HOME}/include" 
                    "$ENV{GLFW_HOME}/include"
                    "$ENV{STB_HOME}"
                    "../../../../Include/imgui-1.85_vulkan_glfw"
                    "$ENV{VULKAN_HOME}/macOS/include")

    SET(SYS_FRAMEWORKS_DIR /System/Library/Frameworks)
    SET(COCOA_LIBS ${SYS_FRAMEWORKS_DIR}/Cocoa.framework)
    SET(COREVIDEO_LIBS ${SYS_FRAMEWORKS_DIR}/CoreVideo.framework)
    SET(IOKIT_LIBS ${SYS_FRAMEWORKS_DIR}/IOKit.framework)
    SET(SYS_VULKAN_LIBS "$ENV{VULKAN_HOME}/macOS/Frameworks/vulkan.framework")
   
endif ()
MESSAGE(STATUS "LIB_INCLUDE: ${LIB_INCLUDE}")
INCLUDE_DIRECTORIES(${LIB_INCLUDE})

#LIB_GLFW_DIR, LIB_VULKAN_DIR, PLUGINS_DIR, STATIC_DIR
if (WIN32)
    SET(LIB_GLFW_DIR "$ENV{GLFW_HOME}/lib")
    SET(LIB_VULKAN_DIR "$ENV{VULKAN_SDK}/Lib")
    SET(PLUGINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Plugins/Windows")

elseif (ANDROID) 


elseif (APPLE)
    SET(LIB_GLFW_DIR "$ENV{GLFW_HOME}/lib")
    SET(LIB_VULKAN_DIR "$ENV{VULKAN_HOME}/macOS/lib")
    SET(PLUGINS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Plugins/MacOS")

endif ()
MESSAGE(STATUS "LIB_GLFW_DIR dir: ${LIB_GLFW_DIR}")
MESSAGE(STATUS "LIB_VULKAN_DIR dir: ${LIB_VULKAN_DIR}")
MESSAGE(STATUS "PLUGINS dir: ${PLUGINS_DIR}")

if (WIN32)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Win32 !")
    
    if(MSVC)
        ADD_DEFINITIONS(/wd4251 /wd4275)
    endif()
    if (DEBUG)
        LINK_LIBRARIES(shlwapi.lib 
                       ${LIB_GLFW_DIR}/glfw3.lib
                       ${LIB_VULKAN_DIR}/vulkan-1.lib
                       ${PLUGINS_DIR}/imgui-1.85_vulkan_glfw_d.lib)
    else ()
        LINK_LIBRARIES(shlwapi.lib 
                       ${LIB_GLFW_DIR}/glfw3.lib
                       ${LIB_VULKAN_DIR}/vulkan-1.lib
                       ${PLUGINS_DIR}/imgui-1.85_vulkan_glfw.lib)
    endif()
    ADD_EXECUTABLE(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES_CPP})

elseif (ANDROID)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom Android !")


elseif (APPLE)
    MESSAGE(STATUS "Build ${PROJECT_NAME} platfrom MacOS !")
    
    ADD_EXECUTABLE(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES_CPP} ${SOURCE_FILES_MM})
    if (DEBUG)
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                              ${COCOA_LIBS}
                              ${COREVIDEO_LIBS}
                              ${IOKIT_LIBS}
                              ${LIB_VULKAN_DIR}/libvulkan.1.dylib
                              ${LIB_VULKAN_DIR}/libvulkan.1.2.189.dylib
                              ${SYS_VULKAN_LIBS}
                              ${PLUGINS_DIR}/libimgui-1.85_vulkan_glfw_d.dylib)
    else ()
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                              ${COCOA_LIBS}
                              ${COREVIDEO_LIBS}
                              ${IOKIT_LIBS}
                              ${LIB_VULKAN_DIR}/libvulkan.1.dylib
                              ${LIB_VULKAN_DIR}/libvulkan.1.2.189.dylib
                              ${SYS_VULKAN_LIBS}
                              ${PLUGINS_DIR}/libimgui-1.85_vulkan_glfw.dylib)
    endif()
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup \
        -Wl,-no_pie \
        -Wl,-search_paths_first"
    )
    
endif ()