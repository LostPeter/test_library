CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
if (APPLE_IOS)
    if (LUA_LIB)
        PROJECT(lua)
    elseif (LUA_EXEC)
        PROJECT(luaexec)
    elseif (LUA_C)
        PROJECT(luac)
    endif ()
else ()
    PROJECT(lua)
endif ()

#MESSAGE(STATUS "HOME dir: $ENV{HOME}")
MESSAGE(STATUS "Binary dir: ${PROJECT_BINARY_DIR}")
MESSAGE(STATUS "SOURCE dir: ${PROJECT_SOURCE_DIR}")
MESSAGE(STATUS "SYSTEM name: ${CMAKE_SYSTEM_NAME}")
MESSAGE(STATUS "SYSTEM version: ${CMAKE_SYSTEM_VERSION}")
MESSAGE(STATUS "SYSTEM processor: ${CMAKE_SYSTEM_PROCESSOR}")

#HEADER_FILES
FILE(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
MESSAGE(STATUS "HEADER files: ${HEADER_FILES}")

#SOURCE_FILES
FILE(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
if (LUA_LIB)
    LIST(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/lua.c;${CMAKE_CURRENT_SOURCE_DIR}/src/luac.c)
endif ()
MESSAGE(STATUS "SOURCE files: ${SOURCE_FILES}")

if (WIN32)
    MESSAGE(STATUS "Build lua platfrom Win32 !")
    ADD_LIBRARY(lua STATIC ${HEADER_FILES} ${SOURCE_FILES})

elseif (ANDROID)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIE -pie")

    if (ANDROID_ARMV7A)
        MESSAGE(STATUS "<1> Build lua platfrom Android armv7a !")

    elseif (ANDROID_ARMV8A)
        MESSAGE(STATUS "<1> Build lua platfrom Android armv8a !")

    else ()
        MESSAGE(FATAL_ERROR "<1> Build lua platfrom Android error, Unknown arch !")

    endif ()

    SET(CMAKE_C_COMPILER   ${NDK_STANDALONE_TOOLCHAIN}/bin/${NDK_TRIPLE}-clang)
    SET(CMAKE_CXX_COMPILER ${NDK_STANDALONE_TOOLCHAIN}/bin/${NDK_TRIPLE}-clang++)
    SET(CMAKE_AR           ${NDK_STANDALONE_TOOLCHAIN}/bin/${NDK_TRIPLE}-ar)
    SET(CMAKE_LINKER       ${NDK_STANDALONE_TOOLCHAIN}/bin/${NDK_TRIPLE}-ld)
    SET(CMAKE_NM           ${NDK_STANDALONE_TOOLCHAIN}/bin/${NDK_TRIPLE}-nm)
    SET(CMAKE_OBJCOPY      ${NDK_STANDALONE_TOOLCHAIN}/bin/${NDK_TRIPLE}-objcopy)
    SET(CMAKE_OBJDUMP      ${NDK_STANDALONE_TOOLCHAIN}/bin/${NDK_TRIPLE}-objdump)
    SET(CMAKE_STRIP        ${NDK_STANDALONE_TOOLCHAIN}/bin/${NDK_TRIPLE}-strip)
    SET(CMAKE_RANLIB       ${NDK_STANDALONE_TOOLCHAIN}/bin/${NDK_TRIPLE}-ranlib)
    
    SET(CMAKE_FIND_ROOT_PATH ${NDK_STANDALONE_TOOLCHAIN})
    ADD_DEFINITIONS("--sysroot=${NDK_STANDALONE_TOOLCHAIN}/sysroot -Wall")
    INCLUDE_DIRECTORIES(${NDK_STANDALONE_TOOLCHAIN}/sysroot/usr/include)
    LINK_DIRECTORIES(${NDK_STANDALONE_TOOLCHAIN}/sysroot/usr/lib)

    #<1> liblua
    ADD_LIBRARY(lua STATIC ${HEADER_FILES} ${SOURCE_FILES})

    #<2> luaexec
    MESSAGE(STATUS "<2> Build luaexec platfrom MacOS !")
    ADD_EXECUTABLE(luaexec ${CMAKE_CURRENT_SOURCE_DIR}/src/lua.c)
    TARGET_LINK_LIBRARIES(luaexec lua)
    TARGET_LINK_LIBRARIES(luaexec m)
    TARGET_LINK_LIBRARIES(luaexec c)
    SET_TARGET_PROPERTIES(luaexec PROPERTIES OUTPUT_NAME lua)

    #<3> luac
    MESSAGE(STATUS "<3> Build luac platfrom MacOS !")
    ADD_EXECUTABLE(luac ${CMAKE_CURRENT_SOURCE_DIR}/src/luac.c)
    TARGET_LINK_LIBRARIES(luac lua)
    TARGET_LINK_LIBRARIES(luac m)
    TARGET_LINK_LIBRARIES(luac c)
    TARGET_LINK_LIBRARIES(luac stdc++)

elseif (APPLE)
    if (APPLE_IOS)
        if (LUA_LIB)
            #<1> liblua
            MESSAGE(STATUS "Build lua platfrom iOS !")
            SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -Wno-implicit-function-declaration")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

            ADD_LIBRARY(lua STATIC ${HEADER_FILES} ${SOURCE_FILES})
        
        elseif (LUA_EXEC)
            #<2> luaexec
            MESSAGE(STATUS "<2> Build luaexec platfrom iOS !")
            SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-function-declaration")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

            SET(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../lib/iOS")
            LINK_LIBRARIES(${LIB_DIR}/liblua.a)
            ADD_EXECUTABLE(luaexec ${CMAKE_CURRENT_SOURCE_DIR}/src/lua.c)
            
            #SET_TARGET_PROPERTIES(luaexec PROPERTIES OUTPUT_NAME lua)

            #${HEADER_FILES}
        
        elseif (LUA_C)
             #<3> luac
            MESSAGE(STATUS "<3> Build luac platfrom iOS !")
            SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-function-declaration")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

            SET(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../lib/iOS")
            LINK_LIBRARIES(${LIB_DIR}/liblua.a)
            ADD_EXECUTABLE(luac ${HEADER_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/src/luac.c)

        endif ()

    else ()
        #<1> liblua
        MESSAGE(STATUS "<1> Build lua platfrom MacOS !")
        ADD_LIBRARY(lua STATIC ${HEADER_FILES} ${SOURCE_FILES})

        #<2> luaexec
        MESSAGE(STATUS "<2> Build luaexec platfrom MacOS !")
        ADD_EXECUTABLE(luaexec ${CMAKE_CURRENT_SOURCE_DIR}/src/lua.c)
        TARGET_LINK_LIBRARIES(luaexec lua)
        SET_TARGET_PROPERTIES(luaexec PROPERTIES OUTPUT_NAME lua)
        
        #<3> luac
        MESSAGE(STATUS "<3> Build luac platfrom MacOS !")
        ADD_EXECUTABLE(luac ${CMAKE_CURRENT_SOURCE_DIR}/src/luac.c)
        TARGET_LINK_LIBRARIES(luac lua)

    endif ()
    
endif ()

