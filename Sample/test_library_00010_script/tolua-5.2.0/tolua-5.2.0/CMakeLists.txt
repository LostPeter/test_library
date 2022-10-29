# Copyright (C) 2007-2014 LuaDist.
# Created by Peter Drahoš
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
# Please note that the package source code is licensed under its own license.

project ( tolua C )
cmake_minimum_required ( VERSION 3.0 )
include ( cmake/dist.cmake )

set (path_lua_include "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Include/")
set (path_lua_lib "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Plugins/")

# find_package ( Lua REQUIRED )
include_directories (include src/lib 
                     ${path_lua_include}${LUA_VERSION} )

# Build lib
# if ( MSVC )
#   add_definitions ( -DTOLUA5_DLL )
# endif ()

file ( GLOB SRC_LIBTOLUA src/lib/*.c )

add_library ( tolua_lib STATIC ${SRC_LIBTOLUA} )
target_link_libraries ( tolua_lib ${path_lua_lib}${LUA_LIB} )
# set_target_properties ( tolua_lib PROPERTIES OUTPUT_NAME tolua CLEAN_DIRECT_OUTPUT
#   1 )

# Build app
include_directories ( src/bin )
set ( SRC_TOLUA src/bin/tolua.c src/bin/toluabind.c )
# LINK_LIBRARIES ( tolua_lib  ${LUA_LIBRARIES}) 
add_executable ( tolua ${SRC_TOLUA} )
target_link_libraries (tolua tolua_lib ${path_lua_lib}${LUA_LIB})

# Install
# install_library ( tolua_lib )
# install_executable ( tolua )
# install_header ( include/ )
# install_data ( README INSTALL )
