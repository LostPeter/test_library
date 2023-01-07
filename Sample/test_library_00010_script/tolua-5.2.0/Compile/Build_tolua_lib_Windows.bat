@REM #****************************************************************************
@REM # test_library - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author: LostPeter
@REM # Time:   2022-10-29
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

@echo off
set debug=%1
echo %debug%

set name="tolua-5.2.0"
if "%debug%" == "debug" (
    set name_project=%name%"_d"
    set name_lib=%name%"_d.lib"
    set name_exe=%name%"_d.exe"
) else (
    set name_project=%name%
    set name_lib=%name%".lib"
    set name_exe=%name%".exe"
)

@rem build folder
set build_folder="..\Build\Windows\"%name_project%
if exist %build_folder% (
    rmdir /S/Q %build_folder%
)
mkdir %build_folder%


@rem Plugins folder/file
set plugins_folder="..\..\..\..\Plugins\Windows"
if not exist %plugins_folder% (
    mkdir %plugins_folder%
)
set plugins_libfile=%plugins_folder%"\"%name_lib%
if exist %plugins_libfile% (
    del /S/Q %plugins_libfile%
)


cd ..
cd Build
cd Windows
cd %name_project%

if "%debug%" == "debug" (
    cmake -DDEBUG=1 -DLUA_VERSION="lua-5.4.3" -DLUA_LIB="Windows/lua-5.4.3_d.lib" "../../../%name%/"
    msbuild tolua.sln /p:configuration=debug
    copy /Y ".\Debug\tolua_lib.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib%
    copy /Y ".\Debug\tolua.exe" "..\..\..\..\..\..\Plugins\Windows\"%name_exe%
) else (
    cmake -DLUA_VERSION="lua-5.4.3" -DLUA_LIB="Windows/lua-5.4.3.lib" "../../../%name%/"
    msbuild tolua.sln /p:configuration=release
    copy /Y ".\Release\tolua_lib.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib%
    copy /Y ".\Release\tolua.exe" "..\..\..\..\..\..\Plugins\Windows\"%name_exe%
)


cd ..
cd ..
cd ..
cd Compile

set include_folder="..\..\..\..\Include\"%name%
if exist %include_folder% (
    rmdir /S/Q %include_folder%
)
mkdir %include_folder%

xcopy /S /E /Y /F "..\%name%\include\*.h" %include_folder%"\"